# physfs.library 1.1

PhysicsFS 3.2.0 as one shared library, `LIBS:physfs.library`, for AmigaOS
3.x on a 68040 or 68060. Repository: DalsinAI/gfopenphysfs.

## What a program gets

All of PhysicsFS 3.2.0's calls (`PHYSFS_init`, `PHYSFS_mount`,
`PHYSFS_openRead`, `PHYSFS_readBytes`, `PHYSFS_enumerate` and the rest, 108
in all), with PhysicsFS's own header and documentation: <physfs.h> is
PhysicsFS's header with each call made an inline call through `PhysFSBase`.
https://icculus.org/physfs/docs/html/ documents every call.

- **One PhysicsFS per program.** Each `OpenLibrary()` gets its own copy of PhysicsFS's state (search path, write drawer, open files, errors), as if PhysicsFS were linked in.
- **Archives:** zip, 7z, GRP, HOG, MVL, WAD, QPAK, SLB, ISO9660, VDF.
- **Deflated zip members inflate through `LIBS:z.library`** (GFOpenZlib 53.13 or later, or Aminet's z.library 53). GFOpenUp installs it. Without it, a deflated member reads as corrupt; everything else still works.
- **Threads:** mutexes are exec semaphores, so a second task (a sound task, say) may read a PhysicsFS file.
- **Memory** comes from a pool per program; what a program leaves open is freed when it closes the library.

## Building a program

    #include <physfs.h>          /* or <proto/physfs.h> */

    m68k-amigaos-gcc -noixemul -m68040 -I$(SDK)/Include ... -L$(SDK)/Lib/build/<prefix> -lphysfs_auto

`-lphysfs_auto` (`Lib/build.sh` builds `libphysfs_auto.a` beside
`libopensdk.a`) opens physfs.library 1 before `main()` and closes it after.
A program started without the library stops with "This program needs
physfs.library 1 or newer in LIBS:" (rc 20). To open it yourself, leave
out `-lphysfs_auto` and define `struct Library *PhysFSBase`.

A port's own copy of PhysicsFS goes: drop its `physfs*.c` from the build,
point its include path at the SDK, link `-lphysfs_auto`. The port's code
does not change.

## Things to know

- **Callbacks** (enumerate, `PHYSFS_Io`, `PHYSFS_Allocator`, `PHYSFS_Archiver`) are called with the library's `a4`. A program built `-fbaserel` must mark them `__saveds`; others need nothing.
- `PHYSFS_getBaseDir()`, `PHYSFS_getUserDir()` and `PHYSFS_getPrefDir()` all give the program's own drawer (PROGDIR:'s full name, ending in "/"), so settings and saves live beside the program.
- 64-bit arguments (`PHYSFS_seek`'s position, for one) go in a register pair such as d0-d1; `<inline/physfs.h>` does this, and `FD/physfs_lib.fd` lists the registers.
- `<physfs.h>` works from C and from C++ (Block Attack, a C++ program, builds against it).

## Licence

zlib (PhysicsFS: Copyright (c) 2001-2022 Ryan C. Gordon and others). The
AmigaOS platform and the library: Copyright (c) 2026 AmiCradle Team.
