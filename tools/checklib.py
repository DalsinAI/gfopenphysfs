#!/usr/bin/env python3
"""Check a linked physfs.library before it goes anywhere.

  checklib.py physfs.library

Reads the AmigaOS hunk file and checks what ramlib and physfs_libinit.c
need, things a link can lose without an error:
- the first code is "moveq #-1,d0; rts", so the file does nothing when run;
- there is a RomTag (0x4AFC whose rt_MatchTag, relocated, points at itself),
  RTF_AUTOINIT, NT_LIBRARY, named physfs.library, whose init table gives a
  56-byte base and a vector table of four entries ended by -1;
- the code holds the table of the data hunk's pointers into itself
  (ld -amiga-datadata-reloc, from -resident), matching the data hunk's own
  relocations, so every opener's copy of the data can be relocated.

Pure Python, no third-party modules. zlib licence, Copyright (c) 2026
AmiCradle Team.
"""
import struct
import sys

HUNK_HEADER, HUNK_CODE, HUNK_DATA, HUNK_BSS = 0x3F3, 0x3E9, 0x3EA, 0x3EB
HUNK_RELOC32, HUNK_SYMBOL, HUNK_END, HUNK_RELOC32SHORT = 0x3EC, 0x3F0, 0x3F2, 0x3F7


def parse(data):
    pos = 0

    def long():
        nonlocal pos
        value = struct.unpack(">I", data[pos:pos + 4])[0]
        pos += 4
        return value

    def word():
        nonlocal pos
        value = struct.unpack(">H", data[pos:pos + 2])[0]
        pos += 2
        return value

    if long() != HUNK_HEADER:
        raise SystemExit("checklib: not an AmigaOS hunk file")
    while long():
        pass
    long()
    first, last = long(), long()
    for _ in range(last - first + 1):
        long()
    hunks, cur = [], None
    while pos < len(data):
        kind = long() & 0x3FFFFFFF
        if kind in (HUNK_CODE, HUNK_DATA):
            n = long()
            cur = {"kind": kind, "data": data[pos:pos + 4 * n], "relocs": {}}
            pos += 4 * n
            hunks.append(cur)
        elif kind == HUNK_BSS:
            long()
            cur = {"kind": kind, "data": b"", "relocs": {}}
            hunks.append(cur)
        elif kind == HUNK_RELOC32:
            while True:
                n = long()
                if not n:
                    break
                target = long()
                cur["relocs"].setdefault(target, set()).update(long() for _ in range(n))
        elif kind == HUNK_RELOC32SHORT:
            start = pos
            while True:
                n = word()
                if not n:
                    break
                target = word()
                cur["relocs"].setdefault(target, set()).update(word() for _ in range(n))
            if (pos - start) % 4:
                pos += 2
        elif kind == HUNK_SYMBOL:
            while True:
                n = long()
                if not n:
                    break
                pos += 4 * n + 4
        elif kind == HUNK_END:
            pass
        else:
            raise SystemExit("checklib: hunk type 0x%x at %d" % (kind, pos - 4))
    return hunks


def be32(buf, off):
    return struct.unpack(">I", buf[off:off + 4])[0]


def pointer(hunks, h, off):
    """The (hunk, offset) a relocated long at hunk h, offset off points at."""
    for target, offs in hunks[h]["relocs"].items():
        if off in offs:
            return target, be32(hunks[h]["data"], off)
    return None


def main():
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    hunks = parse(open(sys.argv[1], "rb").read())
    errors = []
    code = hunks[0]["data"]
    if code[:4] != b"\x70\xff\x4e\x75":
        errors.append("the first code is not moveq #-1,d0; rts")

    tag = None
    for h, hunk in enumerate(hunks):
        buf = hunk["data"]
        for off in range(0, len(buf) - 26, 2):
            if buf[off:off + 2] == b"\x4a\xfc" and pointer(hunks, h, off + 2) == (h, off):
                tag = (h, off)
                break
        if tag:
            break
    if not tag:
        errors.append("no RomTag")
    else:
        h, off = tag
        buf = hunks[h]["data"]
        flags, version, rtype = buf[off + 10], buf[off + 11], buf[off + 12]
        if flags & 0x80 == 0 or rtype != 9:
            errors.append("RomTag: flags 0x%02x type %d, wanted RTF_AUTOINIT and NT_LIBRARY" % (flags, rtype))
        name = pointer(hunks, h, off + 14)
        if not name or not hunks[name[0]]["data"][name[1]:].startswith(b"physfs.library\0"):
            errors.append("RomTag: rt_Name is not physfs.library")
        init = pointer(hunks, h, off + 22)
        if not init:
            errors.append("RomTag: no rt_Init")
        else:
            ih, ioff = init
            ibuf = hunks[ih]["data"]
            if be32(ibuf, ioff) != 56:
                errors.append("init table: base size %d, wanted 56" % be32(ibuf, ioff))
            vec = pointer(hunks, ih, ioff + 4)
            func = pointer(hunks, ih, ioff + 12)
            if not vec or not func:
                errors.append("init table: no vectors or no init function")
            else:
                vh, voff = vec
                ok = all(pointer(hunks, vh, voff + 4 * i) for i in range(4))
                if not ok or be32(hunks[vh]["data"], voff + 16) != 0xFFFFFFFF:
                    errors.append("parent vectors: not four entries and -1")
            print("checklib: RomTag in hunk %d at 0x%x, version %d" % (h, off, version))

    data_hunks = [i for i, hk in enumerate(hunks) if hk["kind"] == HUNK_DATA]
    if data_hunks:
        d = data_hunks[0]
        own = sorted(hunks[d]["relocs"].get(d, ()))
        want = struct.pack(">I", len(own)) + b"".join(struct.pack(">I", o) for o in own)
        if own and want not in code:
            errors.append("no __datadata_relocs table for the data's %d pointers into itself"
                          " (link with -resident)" % len(own))
        else:
            print("checklib: %d data pointers into the data, table present" % len(own))

    if errors:
        for e in errors:
            print("checklib: " + e)
        raise SystemExit(1)
    print("checklib: PASS")


if __name__ == "__main__":
    main()
