# GFOpenPhysFS: physfs.library

**PhysicsFS for AmigaOS 3.x, as one shared library for every port.**

[PhysicsFS](https://icculus.org/physfs/) gives a program one tree of files
built from drawers and archives (zip, 7z, Doom WADs, Quake PAKs, Descent
HOGs, Build GRPs and more), and a safe place to write. Many of the games we
port use it, among them Hedgewars, Warzone 2100, Naev, SuperTux,
DXX-Rebirth, Blobby Volley, Block Attack, C-Dogs SDL, Neverball, Open Surge
(through Allegro 5), Lincity-NG, Trigger Rally, VVVVVV and LÖVE.

Until now each port carried its own copy, built into the program. This
repository makes one copy, `LIBS:physfs.library`:

- **Each program has its own PhysicsFS.** Every program that opens the library gets its own copy of PhysicsFS's state: search path, write drawer, open files, errors. It behaves as if PhysicsFS were linked in (libnix's `libinitr`, one data segment per opener).
- **Ports build unchanged.** Build against the SDK's `physfs.h` and link with `-lphysfs_auto`. The calls go through `PhysFSBase`; nothing else in the port changes.
- **Smaller programs.** The test program is 19 KB through the library and 142 KB with PhysicsFS linked in.
- **Safe with more than one task.** Mutexes are exec semaphores, so a program's sound task may stream from a PhysicsFS file beside its main task. Memory comes from a pool per program, so whatever a program leaves open is freed when it closes the library.
- **All ten archivers**, on dos.library: zip, 7z, GRP, HOG, MVL, WAD, QPAK, SLB, ISO9660 and VDF.
- **Zip members inflate through `LIBS:z.library`** (GFOpenZlib 53.13 or later, or the Aminet z.library 53), not a copy of miniz inside the library: one inflate on the machine for every program, and the one AmiCradle runs as host code. GFOpenUp installs z.library. Without it, stored zip members and every other archive type still read; a deflated member reports itself corrupt. 7z keeps PhysicsFS's LZMA SDK for now.

## Status (8 November 2026)

| | |
|---|---|
| PhysicsFS | 3.2.0 as released (`physfs/`), with the Team's AmigaOS platform (`amiga/`) and two patches (`patches/`: the header, and zip's inflate from z.library) |
| physfs.library | 1.0: builds with OpenAmigaGCC 16.2.0-open4; not yet run on an Amiga |
| Host check | `tests/host/run.sh`: PhysFSTest on Linux, 39 of 39 with PhysicsFS's miniz and 39 of 39 with the zip reader as the library builds it, on zlib |
| Next | PhysFSTest in a Cradle lab (one copy, then two at once), then Hedgewars 1.0.3 on it |

## Using it in a port

    CFLAGS  += -I$(GFOPENPHYSFS)/sdk/include
    LDLIBS  += -L$(GFOPENPHYSFS)/sdk/lib -lphysfs_auto

`libphysfs_auto.a` opens `physfs.library` 1 before `main()`. A program
started without it stops with "This program needs physfs.library 1 or newer
in LIBS:". To open it yourself, leave out the stub and set `PhysFSBase`.

A port that would rather link PhysicsFS in uses `sdk/include-static` and
`sdk/lib/libphysfs.a`. It is the same code, built without the library glue.

**Callbacks:** PhysicsFS calls a program's callbacks (enumerate, `PHYSFS_Io`,
`PHYSFS_Allocator`) with the library's `a4`. Ordinary programs don't care.
A program built `-fbaserel` must mark its callbacks `__saveds`.

## Building

    STOVE=~/AmiCradle/stoves/os32-gcc16 ./build-amiga.sh    # or Cradle's stove_build, target .
    sh tests/host/run.sh                                     # the host check

`build-amiga.sh` lists what it makes. `NDK=` names the OS headers when a
stove has none of its own.

## How the library is made

`tools/gen.py` reads PhysicsFS's own `physfs.h` and `library/functions.txt`.
That file is the call table in LVO order: the library's ABI, only ever
appended to. From them it makes:
- each call's library entry, which points `a4` at the caller's data and calls PhysicsFS's function directly;
- the inline calls a program uses;
- the SDK's `physfs.h`;
- an `.fd` file.

[docs/DESIGN.md](docs/DESIGN.md) has the register rules and the reasons.

## Licence

zlib, like PhysicsFS: PhysicsFS is Copyright (c) 2001-2022 Ryan C. Gordon
and others (`physfs/LICENSE.txt`). The AmigaOS platform and the library are
Copyright (c) 2026 AmiCradle Team (`LICENSE`). The platform file was written
by Team 2 for the DXX-Rebirth port and moved here by Team 5.

Designed, tested and delivered by The AmiCradle Team.
