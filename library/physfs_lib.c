/*
 * physfs.library: PhysicsFS 3.2.0 as one shared AmigaOS library.
 *
 * Built with -fbaserel and started by physfs_libinit.c: every OpenLibrary()
 * makes a new library base holding its own copy of PhysicsFS's data, so each program
 * has its own search path, write dir, open files and error state, as if it
 * had PhysicsFS linked in. Each call's stub (physfs_stubs.c, made by
 * tools/gen.py) points a4 at the caller's copy before calling PhysicsFS.
 *
 * Memory comes from an exec pool per opener, behind a semaphore (a program's
 * sound task may call in beside its main task), so whatever a program leaves
 * behind goes when it closes the library.
 *
 * zlib licence, like PhysicsFS. Copyright (c) 2026 AmiCradle Team.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/semaphores.h>
#include <exec/execbase.h>
#include <dos/dosextens.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include "physfs_libinit.h"
#include <string.h>
#include <stddef.h>

#include "physfs.h"

struct ExecBase *SysBase;
struct DosLibrary *DOSBase;

static APTR pool;
static struct SignalSemaphore poolLock;

/* The size sits in front of every block; 8 keeps the blocks 8-byte aligned. */
#define HEAD 8

void *malloc(size_t size)
{
    ULONG *block;
    if (size == 0)
        size = 1;
    ObtainSemaphore(&poolLock);
    block = (ULONG *) AllocPooled(pool, size + HEAD);
    ReleaseSemaphore(&poolLock);
    if (!block)
        return NULL;
    block[0] = (ULONG) size;
    return (UBYTE *) block + HEAD;
}

void free(void *ptr)
{
    ULONG *block;
    if (!ptr)
        return;
    block = (ULONG *) ((UBYTE *) ptr - HEAD);
    ObtainSemaphore(&poolLock);
    FreePooled(pool, block, block[0] + HEAD);
    ReleaseSemaphore(&poolLock);
}

void *realloc(void *ptr, size_t size)
{
    void *bigger;
    size_t old;
    if (!ptr)
        return malloc(size);
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }
    old = (size_t) ((ULONG *) ((UBYTE *) ptr - HEAD))[0];
    if (size <= old)
        return ptr;
    bigger = malloc(size);
    if (!bigger)
        return NULL;
    memcpy(bigger, ptr, old);
    free(ptr);
    return bigger;
}

void *calloc(size_t n, size_t size)
{
    void *ptr;
    if (size && n > ((size_t) -1) / size)
        return NULL;
    ptr = malloc(n * size);
    if (ptr)
        memset(ptr, 0, n * size);
    return ptr;
}

/* Nothing in a library may end the program: libnix's raise() and stdio
 * refer to exit(), which PhysicsFS never reaches. Should it ever be called,
 * the opener is told by an alert and the library goes on. */
void exit(int code)
{
    (void) code;
    Alert(0x35000000UL);  /* AG_BadParm, as a recoverable alert */
    for (;;)
        Wait(0);
}

void abort(void)
{
    exit(20);
}

/* physfs_libinit.c calls these with a4 on this opener's data: 0 is success. */
LONG __stdargs __UserLibInit(struct Library *base, REG(a4, APTR a4))
{
    (void) a4;
    SysBase = ((struct PhysFSLibBase *) base)->sys;
    if (SysBase->LibNode.lib_Version < 39)
        return 1;
    DOSBase = (struct DosLibrary *) OpenLibrary((CONST_STRPTR) "dos.library", 39);
    if (!DOSBase)
        return 1;
    InitSemaphore(&poolLock);
    pool = CreatePool(MEMF_ANY, 16384, 4096);
    if (!pool)
    {
        CloseLibrary((struct Library *) DOSBase);
        DOSBase = NULL;
        return 1;
    }
    return 0;
}

VOID __stdargs __UserLibCleanup(REG(a4, APTR a4))
{
    (void) a4;
    if (PHYSFS_isInit())
        PHYSFS_deinit();  /* a program that forgot: its files are closed */
    if (pool)
        DeletePool(pool);
    pool = NULL;
    if (DOSBase)
        CloseLibrary((struct Library *) DOSBase);
    DOSBase = NULL;
}
