#!/usr/bin/env python3
"""Generate physfs.library's glue from PhysicsFS's own physfs.h.

  gen.py OUTDIR

Reads physfs/physfs.h (every PHYSFS_DECL call) and library/functions.txt (the
call table, in LVO order, which is the library's ABI) and writes:

  OUTDIR/physfs_stubs.c          library side: one entry per call, in table
                                 order. Each entry saves a4, pushes the call's
                                 registers as C arguments, loads this opener's
                                 data pointer (base + 40, libnix libinitr) into
                                 a4 and calls PhysicsFS's own function.
  OUTDIR/include/inline/physfs.h caller side: a static inline function per call
                                 that loads the registers and jumps through
                                 PhysFSBase.
  OUTDIR/include/proto/physfs.h  the usual proto header.
  OUTDIR/include/physfs.h        PhysicsFS's header with its prototypes taken
                                 out and the inline calls put in, so a port
                                 builds unchanged against the library.
  OUTDIR/fd/physfs_lib.fd        the call table as an .fd file.

Registers: pointer arguments take a0, a1, a2, a3 in turn; other arguments
take d0 to d7 in turn, a 64-bit one taking two (high word first). A 64-bit
result comes back in d0 (high) and d1 (low), as GCC returns it. Pushed in
reverse, those registers make exactly the stack frame of the C prototype, so
the stub calls PhysicsFS's function directly, with no wrapper.

Pure Python, no third-party modules. zlib licence, Copyright (c) 2026
AmiCradle Team.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / "physfs" / "physfs.h"
TABLE = ROOT / "library" / "functions.txt"
FIRST_LVO = 30
AREGS = ["a0", "a1", "a2", "a3"]
DREGS = ["d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7"]
SCRATCH = ["d0", "d1", "a0", "a1"]
WIDE = {"PHYSFS_uint64", "PHYSFS_sint64"}
POINTER_TYPES = {"PHYSFS_StringCallback", "PHYSFS_EnumFilesCallback", "PHYSFS_EnumerateCallback"}
DECL = re.compile(r"PHYSFS_DECL\s+([^;{]*?\([^;{]*?\))\s*(PHYSFS_DEPRECATED)?\s*;", re.S)


def strip_comments(text):
    return re.sub(r"/\*.*?\*/", "", text, flags=re.S)


def split_params(params):
    out, depth, cur = [], 0, ""
    for ch in params:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return [] if out == ["void"] else out


def parse_param(p):
    """(C declaration, name, kind) where kind is ptr, wide or int."""
    m = re.match(r"(.*)\(\s*\*\s*(\w+)\s*\)\s*\((.*)\)$", p)
    if m:  # a function pointer
        return p, m.group(2), "ptr"
    m = re.match(r"(.*?)(\w+)$", p)
    ctype, name = m.group(1).strip(), m.group(2)
    base = ctype.replace("const", "").strip()
    if "*" in ctype or base in POINTER_TYPES:
        kind = "ptr"
    elif base in WIDE:
        kind = "wide"
    else:
        kind = "int"
    return p, name, kind


def parse_header():
    calls = {}
    for decl, deprecated in DECL.findall(strip_comments(HEADER.read_text())):
        decl = " ".join(decl.split())
        m = re.match(r"(.*?)\b(PHYSFS_\w+)\s*\((.*)\)$", decl)
        ret, name, params = m.group(1).strip(), m.group(2), m.group(3)
        calls[name] = dict(name=name, ret=ret, params=[parse_param(p) for p in split_params(params)],
                           deprecated=bool(deprecated))
    return calls


def read_table():
    return [l.strip() for l in TABLE.read_text().splitlines() if l.strip() and not l.startswith("#")]


def assign(call):
    """The registers for each argument, in order: a list of lists."""
    a, d, regs = list(AREGS), list(DREGS), []
    for _, name, kind in call["params"]:
        if kind == "ptr":
            if not a:
                raise SystemExit(call["name"] + ": more than 4 pointer arguments")
            regs.append([a.pop(0)])
        else:
            need = 2 if kind == "wide" else 1
            if len(d) < need:
                raise SystemExit(call["name"] + ": out of data registers")
            regs.append([d.pop(0) for _ in range(need)])
    return regs


def ret_kind(ret):
    base = ret.replace("const", "").strip()
    if base == "void":
        return "void"
    if "*" in ret:
        return "ptr"
    if base in WIDE:
        return "wide"
    return "int"


def stub(call, regs):
    flat = [r for rr in regs for r in rr]
    label = "_LIBSTUB_" + call["name"]
    lines = ['asm(".globl %s");' % label, 'asm("%s:\\tmovel a4,sp@-");' % label]
    for r in reversed(flat):
        lines.append('asm("\\tmovel %s,sp@-");' % r)
    lines.append('asm("\\tmovel a6@(40:W),a4");')
    lines.append('asm("\\tjsr _%s");' % call["name"])
    if flat:
        lines.append('asm("\\taddaw #%d,sp");' % (4 * len(flat)))
    lines.append('asm("\\tmovel sp@+,a4");')
    lines.append('asm("\\trts");')
    lines.append("ADD2LIST(LIBSTUB_%s,__FuncTable__,22);" % call["name"])
    return "\n".join(lines)


def inline(call, regs, lvo):
    params = ", ".join(p[0] for p in call["params"]) or "void"
    body = ["    register struct Library *_base __asm(\"a6\") = PhysFSBase;"]
    inputs = {}
    for (decl, name, kind), rr in zip(call["params"], regs):
        if kind == "wide":
            inputs[rr[0]] = "(ULONG)((PHYSFS_uint64)(%s) >> 32)" % name
            inputs[rr[1]] = "(ULONG)(PHYSFS_uint64)(%s)" % name
        elif kind == "ptr":
            inputs[rr[0]] = "(ULONG)(%s)" % name
        else:
            inputs[rr[0]] = "(ULONG)(LONG)(%s)" % name
    for r in SCRATCH:
        body.append("    register ULONG _%s __asm(\"%s\")%s;" % (r, r, (" = " + inputs[r]) if r in inputs else ""))
    for r in sorted(inputs):
        if r not in SCRATCH:
            body.append("    register ULONG _%s __asm(\"%s\") = %s;" % (r, r, inputs[r]))
    outs = ", ".join(('"+r"' if r in inputs else '"=r"') + "(_%s)" % r for r in SCRATCH)
    ins = ", ".join(['"r"(_base)'] + ['"r"(_%s)' % r for r in sorted(inputs) if r not in SCRATCH])
    body.append('    __asm volatile ("jsr a6@(-%d:W)" : %s : %s : "fp0", "fp1", "cc", "memory");' % (lvo, outs, ins))
    rk = ret_kind(call["ret"])
    if rk == "wide":
        body.append("    return (%s)(((PHYSFS_uint64)_d0 << 32) | _d1);" % call["ret"])
    elif rk == "ptr":
        body.append("    return (%s)_d0;" % call["ret"])
    elif rk == "int":
        body.append("    return (%s)(LONG)_d0;" % call["ret"])
    return "static __inline__ %s %s(%s)\n{\n%s\n}\n" % (call["ret"], call["name"], params, "\n".join(body))


def fd_line(call, regs):
    names = ",".join(p[1] for p in call["params"])
    rl = ",".join("".join(r) if len(r) == 1 else "-".join(r) for r in regs)
    return "%s(%s)(%s)" % (call["name"], names, rl)


def sdk_header():
    text = HEADER.read_text()
    text = re.sub(r"PHYSFS_DECL\s+[^;{]*?\([^;{]*?\)\s*(?:PHYSFS_DEPRECATED)?\s*;",
                  "/* (a physfs.library call: see <inline/physfs.h>) */", text, flags=re.S)
    marker = "#ifdef __cplusplus\n}\n#endif"
    pos = text.rfind(marker)
    if pos < 0:
        raise SystemExit("physfs.h: no closing extern \"C\"")
    return (text[:pos] + "#include <inline/physfs.h>\n\n" + text[pos:]).replace(
        "#ifndef _INCLUDE_PHYSFS_H_\n#define _INCLUDE_PHYSFS_H_\n",
        "#ifndef _INCLUDE_PHYSFS_H_\n#define _INCLUDE_PHYSFS_H_\n"
        "/* physfs.library's form of PhysicsFS 3.2.0's header, made by gfopenphysfs\n"
        " * tools/gen.py: the calls go through PhysFSBase. Link -lphysfs_auto to\n"
        " * have the library opened for you, or open it yourself into PhysFSBase. */\n"
        "#define PHYSFS_LIBRARY_CALLS 1\n", 1)


def main():
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    out = Path(sys.argv[1])
    calls, table = parse_header(), read_table()
    missing = [n for n in calls if n not in table]
    unknown = [n for n in table if n not in calls]
    if missing or unknown or len(set(table)) != len(table):
        raise SystemExit("functions.txt and physfs.h disagree: missing %s, unknown %s" % (missing, unknown))
    for d in ("include/inline", "include/proto", "fd"):
        (out / d).mkdir(parents=True, exist_ok=True)
    stubs, inlines, fd = [], [], ["##base _PhysFSBase", "##bias %d" % FIRST_LVO, "##public"]
    for i, name in enumerate(table):
        call = calls[name]
        regs = assign(call)
        stubs.append(stub(call, regs))
        inlines.append(inline(call, regs, FIRST_LVO + 6 * i))
        fd.append(fd_line(call, regs))
    fd.append("##end")
    (out / "physfs_stubs.c").write_text(
        "/* physfs.library's call table, made by tools/gen.py from library/functions.txt. */\n"
        "#include <stabs.h>\n\n" + "\n\n".join(stubs) + "\n\nADDTABL_END();\n")
    (out / "include/inline/physfs.h").write_text(
        "/* physfs.library calls, made by gfopenphysfs tools/gen.py. Included by <physfs.h>. */\n"
        "#ifndef _INLINE_PHYSFS_H\n#define _INLINE_PHYSFS_H\n#include <exec/types.h>\n"
        "struct Library;\nextern struct Library *PhysFSBase;\n\n" + "\n".join(inlines) + "\n#endif\n")
    (out / "include/proto/physfs.h").write_text(
        "#ifndef _PROTO_PHYSFS_H\n#define _PROTO_PHYSFS_H\n#include <exec/types.h>\n"
        "extern struct Library *PhysFSBase;\n#define PHYSFSNAME \"physfs.library\"\n"
        "#include <physfs.h>\n#endif\n")
    (out / "include/physfs.h").write_text(sdk_header())
    (out / "fd/physfs_lib.fd").write_text("\n".join(fd) + "\n")
    print("gen.py: %d calls, LVO -%d to -%d" % (len(table), FIRST_LVO, FIRST_LVO + 6 * (len(table) - 1)))


if __name__ == "__main__":
    main()
