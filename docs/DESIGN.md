# physfs.library: design

Team 5, 8 November 2026.

## Why a library

PhysicsFS is used by many of the games in the Port Atlas. Each port so far
compiled its own copy in: DXX-Rebirth and Block Attack carry Team 2's AmigaOS
platform file, and Blobby Volley wrote a shim instead. One shared library
gives every port the same fixed code and keeps programs smaller. It also
lets a fix reach every game at once.

## One PhysicsFS per program

PhysicsFS keeps its state in static variables: whether it is initialised, the
search path, the write drawer, open files, per-thread errors. A library with
one data segment would share all of that between programs: one program's
`PHYSFS_deinit()` would end another's game.

So PhysicsFS and the call entries are compiled `-fbaserel` (data addressed
through `a4`), and `library/physfs_libinit.c`, linked first, starts the
library:
- **The RomTag** (RTF_AUTOINIT, NT_LIBRARY) is what ramlib looks for after it `LoadSeg()`s the file. Its init makes the parent base, the one in exec's library list. The parent's data hunk is never written: it is the pattern for the copies.
- **The parent's Open** makes a new base for each `OpenLibrary()` (`MakeLibrary` with the call table and room for the data and bss), copies the data hunk's initialised part behind it and relocates its pointers into itself (the table ld makes with `-amiga-datadata-reloc`, which the link's `-resident` turns on). The base's offset 40 points at that copy (`a4`).
- **`__UserLibInit`** then runs with `a4` on the copy; 0 means success. The program gets the new base.
- **The opener's Close** runs `__UserLibCleanup` with `a4` on its copy and frees that base. The parent expunges once nothing is open (`Avail FLUSH`), or at the last close if an expunge was asked for meanwhile.

Version 1.0 used libnix's `libinitr.o` for this. That object has no RomTag,
so ramlib could not start the library (`OpenLibrary()` gave NULL), and in
OpenAmigaGCC 16.2.0-open4's link its Open passes `MakeLibrary` a table that
starts at the first PhysicsFS entry instead of Open/Close/Expunge/ExtFunc.
`tools/checklib.py` now checks every build for the RomTag, the parent's
vectors and the data relocation table.

Our `__UserLibInit` opens dos.library 39 and makes an exec pool.
`__UserLibCleanup` calls `PHYSFS_deinit()` if the program forgot, then
deletes the pool, so nothing a program leaves behind stays allocated.

## A call, end to end

**In the program:** `PHYSFS_readBytes(f, buf, len)` is a static inline function from `<inline/physfs.h>`. It:
1. loads `a0 = f`, `a1 = buf`, `d0 = len >> 32` and `d1 = len & 0xffffffff`;
2. jumps to `PhysFSBase - 606`;
3. reads the 64-bit result back from `d0` (high word) and `d1` (low word).

**In the library:** the entry `LIBSTUB_PHYSFS_readBytes`:
1. saves `a4`;
2. pushes `d1`, `d0`, `a1`, `a0` (the call's registers, last first);
3. loads `a4` from `a6 + 40`;
4. calls PhysicsFS's own `PHYSFS_readBytes`;
5. drops the arguments, restores `a4` and returns.

Pushed in that order, the registers form exactly the stack frame the C
prototype expects. A 64-bit argument's two registers go high word first,
which on the 68k is the order a `PHYSFS_uint64` has in memory. So no wrapper
functions are needed. GCC returns 64-bit values in `d0`/`d1`, and the entry
leaves them alone.

**Register rules:** pointer arguments take `a0`, `a1`, `a2`, `a3` in turn.
Other arguments take `d0` to `d7` in turn, and a 64-bit one takes two. The
largest call, `PHYSFS_mountMemory`, uses `a0`, `d0-d1`, `a1`, `a2`, `a3`
and `d2`. `a4`, `a5` and `a6` are never arguments. `d2`-`d7` and `a2`-`a5`
come back unchanged, because PhysicsFS's functions follow GCC's rules.

## The ABI

`library/functions.txt` holds all 108 calls of PhysicsFS 3.2.0 in LVO order,
from -30 to -672, deprecated ones included, so older ports still build.
`tools/gen.py` refuses to run if `physfs.h` and the table disagree. A later
PhysicsFS adds its new calls at the end and raises the library's version.
Nothing is ever reordered or removed.

## Threads and callbacks

- **Mutexes:** PhysicsFS's mutexes are exec SignalSemaphores, and they nest for the task holding them. The pool allocator has its own semaphore.
- **Callbacks:** callbacks run with the library's `a4`. That is harmless for ordinary programs (absolute data) but not for `-fbaserel` programs, which must use `__saveds`.

## Where files go

- `PHYSFS_getBaseDir()`, `PHYSFS_getUserDir()` and `PHYSFS_getPrefDir()` are the program's own drawer (`PROGDIR:`), so settings and saves stay with the game, as Amiga users expect. A later version may offer `ENVARC:` for settings.
- Names are AmigaDOS names with "/" as the separator. The platform drops a trailing "/" and one right after a ":".

## Inflate from z.library (9 November 2026)

PhysicsFS carries its own inflate (`physfs_miniz.h`) and the LZMA SDK
(`physfs_lzmasdk.h`). On the Amiga we already have a shared inflate,
z.library, which GFOpenCompress and GFOffice use; and on AmiCradle that is
the inflate to run as host code (the host-run libraries plan). So
physfs.library takes zip's inflate from z.library (Dale, 9 Nov: "we have
opencompress, so what's physfs adding by having its own code").

- `patches/0002-zip-inflate-from-zlibrary.patch` includes
  `amiga/physfs_zlibrary.h` in place of miniz when `PHYSFS_ZLIBRARY` is
  defined, and starts a stream again with `inflateReset` when a program
  seeks back in a deflated member. PhysicsFS copied a fresh z_stream over
  the old one there; zlib's state points back at its z_stream (zlib 1.2.9
  on), so the copy would be refused.
- `amiga/physfs_zlibrary.c` calls z.library's jump table as GFOpenCompress
  does (InflateInit2 -102, Inflate -60, InflateEnd -66, InflateReset -114;
  GFOpenZlib 53.12 refused). z.library allocates for itself: PhysicsFS's
  zalloc and zfree are cleared.
- Each opener opens z.library for itself, the first time a deflated member
  is read; `__UserLibCleanup` closes it. `libphysfs.a` closes it at exit.
- 7z stays on PhysicsFS's LZMA SDK until there is one shared LZMA decoder.

## Still to do

1. **PhysFSTest in a Cradle lab:** done 9 November 2026 on the Team's OpenROM (see the pull request for 1.1): one copy, two at once, 20 opens and closes, and the expunge.
2. **Hedgewars 1.0.3 built on the SDK**, then DXX-Rebirth and Block Attack moved off their own copies.
3. **An OS4 and AROS form** (AROS has its own library tools).
4. **Offer the AmigaOS platform file upstream** to PhysicsFS.
