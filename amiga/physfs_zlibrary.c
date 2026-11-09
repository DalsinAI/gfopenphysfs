/*
 * physfs.library on AmigaOS: zlib's inflate from z.library, through its
 * jump table in the Aminet library's order and registers (its z_lib.sfd),
 * which GFOpenZlib follows from 53.13:
 *   -60 Inflate a0,d0   -66 InflateEnd a0   -102 InflateInit2 a0,d0
 *   -114 InflateReset a0
 * The same calls as GFOpenCompress's zlibrary/zlibrary.c (gfopencompress
 * 0.3.3), for the reads PhysicsFS's zip reader makes.
 *
 * In physfs.library (libnix libinitr, -fbaserel) ZBase is in each opener's
 * own data, so every program opens z.library for itself and
 * __UserLibCleanup closes it. In libphysfs.a it is the program's, closed at
 * exit.
 *
 * zlib licence, Copyright (c) 2026 AmiCradle Team.
 */
#include "physfs_zlibrary.h"

#ifndef PHYSFS_ZLIBRARY_HOSTED

#include <exec/types.h>
#include <exec/libraries.h>
#include <proto/exec.h>

static const char no_zlib[] = "z.library 53 is needed in LIBS: (GFOpenZlib, or the Aminet z.library)";

static struct Library *ZBase;
static int tried;

#ifndef PHYSFS_ZLIBRARY_IN_LIBRARY
#include <stdlib.h>
static void zl_atexit(void) { physfs_zlibrary_close(); }
#endif

void physfs_zlibrary_close(void)
{
    if (ZBase)
        CloseLibrary(ZBase);
    ZBase = NULL;
    tried = 0;
}

static int is_gfopenzlib(const char *id)
{
    const char *w = "GFOpenZlib";
    for (; id && *id; id++) {
        int i = 0;
        while (w[i] && id[i] == w[i])
            i++;
        if (!w[i])
            return 1;
    }
    return 0;
}

static struct Library *zl(void)
{
    if (!tried) {
        struct Library *b;
        tried = 1;
        b = OpenLibrary((CONST_STRPTR) "z.library", 53);
        if (b && b->lib_Version == 53 && b->lib_Revision == 12 && is_gfopenzlib((const char *) b->lib_IdString)) {
            CloseLibrary(b);
            b = NULL;
        }
        ZBase = b;
#ifndef PHYSFS_ZLIBRARY_IN_LIBRARY
        if (ZBase)
            atexit(zl_atexit);
#endif
    }
    return ZBase;
}

#define CALL(lvo)                                                                 \
    __asm volatile("jsr %%a6@(" #lvo ":W)"                                        \
                   : "+r"(d0), "+r"(d1), "+r"(a0), "+r"(a1)                       \
                   : "r"(a6)                                                      \
                   : "cc", "memory")

#define REGS(base)                                                                \
    register struct Library *a6 __asm("a6") = (base);                             \
    register ULONG d0 __asm("d0") = 0;                                            \
    register ULONG d1 __asm("d1") = 0;                                            \
    register ULONG a0 __asm("a0") = 0;                                            \
    register ULONG a1 __asm("a1") = 0

int inflateInit2(z_stream *s, int windowBits)
{
    struct Library *b = zl();
    s->zalloc = 0;          /* z.library allocates for itself */
    s->zfree = 0;
    s->opaque = 0;
    if (!b) {
        s->msg = no_zlib;
        return Z_VERSION_ERROR;
    }
    {
        REGS(b);
        a0 = (ULONG) s;
        d0 = (ULONG) windowBits;
        CALL(-102);
        (void) d1;
        (void) a1;
        return (int) (LONG) d0;
    }
}

int inflate(z_stream *s, int flush)
{
    if (!ZBase)
        return Z_STREAM_ERROR;
    {
        REGS(ZBase);
        a0 = (ULONG) s;
        d0 = (ULONG) flush;
        CALL(-60);
        return (int) (LONG) d0;
    }
}

int inflateEnd(z_stream *s)
{
    if (!ZBase)
        return Z_STREAM_ERROR;
    {
        REGS(ZBase);
        a0 = (ULONG) s;
        CALL(-66);
        return (int) (LONG) d0;
    }
}

int inflateReset(z_stream *s)
{
    if (!ZBase)
        return Z_STREAM_ERROR;
    {
        REGS(ZBase);
        a0 = (ULONG) s;
        CALL(-114);
        return (int) (LONG) d0;
    }
}

#endif
