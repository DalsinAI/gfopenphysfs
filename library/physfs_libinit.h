/*
 * physfs.library's own start (physfs_libinit.c), shared with physfs_lib.c.
 *
 * zlib licence, like PhysicsFS. Copyright (c) 2026 AmiCradle Team.
 */
#ifndef PHYSFS_LIBINIT_H
#define PHYSFS_LIBINIT_H

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/execbase.h>

#ifndef __stdargs
#define __stdargs
#endif
#if !defined(REG)
#define REG(reg, arg) arg __asm(#reg)
#endif

/*
 * The library base. The one ramlib makes (the parent, in LIBS: list) and the
 * one each OpenLibrary() gets (an opener's own, in no list) look the same; an
 * opener's base is followed by its own copy of the data hunk. The offsets
 * are fixed: the call entries load a4 from base + 40 (gen.py), and they are
 * the offsets libnix's libinitr.o used.
 */
struct PhysFSLibBase
{
    struct Library        lib;       /*  0 */
    UWORD                 pad;       /* 34 */
    BPTR                  seglist;   /* 36: the parent's; 0 in an opener's */
    APTR                  a4;        /* 40: this base's data + 0x7ffe */
    struct ExecBase      *sys;       /* 44 */
    ULONG                 datasize;  /* 48: data + bss, in bytes */
    struct PhysFSLibBase *parent;    /* 52: itself in the parent */
};                                   /* 56 */

#define PHYSFS_LIB_VERSION  1
#define PHYSFS_LIB_REVISION 1

/* physfs_lib.c: run with a4 on the new opener's data; 0 is success. */
LONG __stdargs __UserLibInit(struct Library *base, REG(a4, APTR a4));
VOID __stdargs __UserLibCleanup(REG(a4, APTR a4));

#endif
