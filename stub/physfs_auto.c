/*
 * libphysfs_auto.a: opens physfs.library for a program before main() and
 * closes it after, so a port needs only <physfs.h> from the SDK and
 * -lphysfs_auto on its link line. A program without the library stops with
 * a message saying so, before main() runs.
 *
 * zlib licence, like PhysicsFS. Copyright (c) 2026 AmiCradle Team.
 */
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <stabs.h>
#include <stdlib.h>

#define PHYSFS_LIB_MIN_VERSION 1

struct Library *PhysFSBase;

void __physfs_open_library(void)
{
    PhysFSBase = OpenLibrary((CONST_STRPTR) "physfs.library", PHYSFS_LIB_MIN_VERSION);
    if (!PhysFSBase)
    {
        PutStr((CONST_STRPTR) "This program needs physfs.library 1 or newer in LIBS:.\n");
        exit(20);
    }
}

void __physfs_close_library(void)
{
    if (PhysFSBase)
        CloseLibrary(PhysFSBase);
    PhysFSBase = NULL;
}

/* Not static: the init and exit lists name them. Priorities as libnix's own auto-opened libraries use: after dos.library. */
ADD2INIT(__physfs_open_library, -40);
ADD2EXIT(__physfs_close_library, -40);
