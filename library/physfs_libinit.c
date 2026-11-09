/*
 * physfs.library's start: its RomTag, the parent base ramlib makes, and one
 * base with its own copy of the data for each OpenLibrary().
 *
 * This replaces libnix's libinitr.o, which the library was linked with until
 * 1.1. That object has the per-opener design but no RomTag, so ramlib could
 * never start the library (OpenLibrary() gave NULL), and in the stove's link
 * its LibOpen hands MakeLibrary a table that starts at the first PhysicsFS
 * entry instead of Open/Close/Expunge/ExtFunc.
 *
 * How it works:
 * - ramlib LoadSeg()s the file, finds the RomTag below and InitResident()s it:
 *   RTF_AUTOINIT makes the parent base with parentVectors and ParentInit()
 *   fills it in. The parent's data hunk is never written: it is the pattern.
 * - OpenLibrary() calls the parent's Open, ParentOpen(). It makes a new base
 *   with the call table (openerVectors), copies the data hunk behind it,
 *   relocates the pointers in it that point into the data (the table ld makes
 *   with -amiga-datadata-reloc, i.e. the link's -resident), and runs
 *   __UserLibInit() with a4 on the copy. The program gets this base.
 * - Every call entry (gen.py) loads a4 from base + 40, so each program runs
 *   PhysicsFS on its own data.
 * - CloseLibrary() calls the opener's Close, OpenerClose(): __UserLibCleanup()
 *   with a4 on its data, then the base and its data are freed. When the last
 *   one goes and an expunge was asked for, the parent expunges.
 *
 * Built without -fbaserel: nothing here uses a4 or writes a global.
 *
 * zlib licence, like PhysicsFS. Copyright (c) 2026 AmiCradle Team.
 */
#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/resident.h>
#include <exec/memory.h>
#include <proto/exec.h>

#include "physfs_libinit.h"

/* Run as a program, the file just returns: the first code in the file. */
__asm__(".text\n\t.even\n\t.globl __physfs_start\n__physfs_start:\n\tmoveq #-1,d0\n\trts");

static const char LibName[] = "physfs.library";
static const char LibIdString[] = "physfs.library 1.1 (9.11.2026) PhysicsFS 3.2.0";
const char __physfs_VersionTag[] = "$VER: physfs.library 1.1 (9.11.2026) PhysicsFS 3.2.0, AmiCradle Team";

/* Made by the linker. The data hunk starts at __a4_init - 0x7ffe. */
extern char __a4_init[];
extern char __data_size[];
extern char __bss_size[];
extern char _edata[];                    /* the end of the initialised data */
extern const ULONG __datadata_relocs[];  /* count, then offsets into the data */

#define DATA_START ((UBYTE *) ((ULONG) __a4_init - 0x7ffe))
#define DATA_SIZE  ((ULONG) __data_size + (ULONG) __bss_size)

static struct PhysFSLibBase *ParentInit(REG(d0, struct PhysFSLibBase *b), REG(a0, BPTR seglist),
                                        REG(a6, struct ExecBase *sys));
static struct PhysFSLibBase *ParentOpen(REG(a6, struct PhysFSLibBase *b));
static BPTR ParentClose(REG(a6, struct PhysFSLibBase *b));
static BPTR ParentExpunge(REG(a6, struct PhysFSLibBase *b));
static ULONG ExtFunc(void);
static BPTR OpenerClose(REG(a6, struct PhysFSLibBase *b));

static const APTR parentVectors[] =
{
    (APTR) ParentOpen,
    (APTR) ParentClose,
    (APTR) ParentExpunge,
    (APTR) ExtFunc,
    (APTR) -1
};

/* One entry per line of library/functions.txt, made by tools/gen.py. */
#define PHYSFS_VECTOR(name) extern void name(void);
#include "physfs_vectors.h"
#undef PHYSFS_VECTOR

static const APTR openerVectors[] =
{
    (APTR) ExtFunc,      /* Open: an opener's base is in no list */
    (APTR) OpenerClose,
    (APTR) ExtFunc,      /* Expunge: the parent's business */
    (APTR) ExtFunc,
#define PHYSFS_VECTOR(name) (APTR) name,
#include "physfs_vectors.h"
#undef PHYSFS_VECTOR
    (APTR) -1
};

static const ULONG initTable[4] =
{
    sizeof(struct PhysFSLibBase),
    (ULONG) parentVectors,
    0,
    (ULONG) ParentInit
};

const struct Resident __physfs_RomTag =
{
    RTC_MATCHWORD,
    (struct Resident *) &__physfs_RomTag,
    (APTR) (&__physfs_RomTag + 1),
    RTF_AUTOINIT,
    PHYSFS_LIB_VERSION,
    NT_LIBRARY,
    0,
    (char *) LibName,
    (char *) LibIdString,
    (APTR) initTable
};

static void set_node(struct PhysFSLibBase *b)
{
    b->lib.lib_Node.ln_Type = NT_LIBRARY;
    b->lib.lib_Node.ln_Name = (char *) LibName;
    b->lib.lib_Flags = LIBF_SUMUSED | LIBF_CHANGED;
    b->lib.lib_Version = PHYSFS_LIB_VERSION;
    b->lib.lib_Revision = PHYSFS_LIB_REVISION;
    b->lib.lib_IdString = (APTR) LibIdString;
}

static struct PhysFSLibBase *ParentInit(REG(d0, struct PhysFSLibBase *b), REG(a0, BPTR seglist),
                                        REG(a6, struct ExecBase *sys))
{
    set_node(b);
    b->seglist = seglist;
    b->a4 = __a4_init;   /* never used to run code: openers have their own */
    b->sys = sys;
    b->datasize = DATA_SIZE;
    b->parent = b;
    return b;
}

/* __UserLibInit(base) and __UserLibCleanup() with a4 on an opener's data. */
static LONG user_init(struct PhysFSLibBase *b)
{
    register LONG d0 __asm("d0");
    register struct PhysFSLibBase *a0 __asm("a0") = b;
    __asm volatile (
        "movel a4,sp@-\n\t"
        "movel a0@(40:W),a4\n\t"
        "movel a0,sp@-\n\t"
        "jsr ___UserLibInit\n\t"
        "addql #4,sp\n\t"
        "movel sp@+,a4"
        : "=r"(d0), "+r"(a0) : : "d1", "a1", "fp0", "fp1", "cc", "memory");
    return d0;
}

static void user_cleanup(struct PhysFSLibBase *b)
{
    register struct PhysFSLibBase *a0 __asm("a0") = b;
    __asm volatile (
        "movel a4,sp@-\n\t"
        "movel a0@(40:W),a4\n\t"
        "jsr ___UserLibCleanup\n\t"
        "movel sp@+,a4"
        : "+r"(a0) : : "d0", "d1", "a1", "fp0", "fp1", "cc", "memory");
}

static void free_base(struct ExecBase *SysBase, struct PhysFSLibBase *b)
{
    UBYTE *mem = (UBYTE *) b - b->lib.lib_NegSize;
    FreeMem(mem, (ULONG) b->lib.lib_NegSize + b->lib.lib_PosSize);
}

/* Called by exec under Forbid(). */
static struct PhysFSLibBase *ParentOpen(REG(a6, struct PhysFSLibBase *p))
{
    struct ExecBase *SysBase = p->sys;
    struct PhysFSLibBase *b;
    UBYTE *data;
    ULONG copy, n;
    const ULONG *reloc;
    LONG delta;

    p->lib.lib_OpenCnt++;              /* no expunge while memory is found */
    p->lib.lib_Flags &= ~LIBF_DELEXP;
    b = (struct PhysFSLibBase *) MakeLibrary((CONST_APTR) openerVectors, NULL, NULL,
                                             sizeof(struct PhysFSLibBase) + p->datasize, 0);
    if (!b)
    {
        p->lib.lib_OpenCnt--;
        return NULL;
    }
    set_node(b);
    b->lib.lib_OpenCnt = 1;
    b->sys = SysBase;
    b->datasize = p->datasize;
    b->parent = p;
    data = (UBYTE *) (b + 1);
    /* The data hunk's initialised part; MakeLibrary cleared the rest (bss). */
    copy = (ULONG) (_edata - (char *) DATA_START);
    if (copy > p->datasize)
        copy = p->datasize;
    CopyMem(DATA_START, data, copy);
    /* Its pointers into the data must point into this copy. */
    delta = (LONG) (data - DATA_START);
    reloc = __datadata_relocs;
    for (n = *reloc++; n; n--)
        *(LONG *) (data + *reloc++) += delta;
    b->a4 = data + 0x7ffe;
    if (user_init(b) != 0)
    {
        free_base(SysBase, b);
        p->lib.lib_OpenCnt--;
        return NULL;
    }
    p->lib.lib_Flags &= ~LIBF_DELEXP;
    return b;
}

static BPTR ParentClose(REG(a6, struct PhysFSLibBase *p))
{
    (void) p;
    return 0;  /* programs close their own base: see OpenerClose */
}

static BPTR ParentExpunge(REG(a6, struct PhysFSLibBase *p))
{
    struct ExecBase *SysBase = p->sys;
    BPTR seglist;
    if (p->lib.lib_OpenCnt)
    {
        p->lib.lib_Flags |= LIBF_DELEXP;
        return 0;
    }
    seglist = p->seglist;
    Remove(&p->lib.lib_Node);
    free_base(SysBase, p);
    return seglist;
}

static ULONG ExtFunc(void)
{
    return 0;
}

static BPTR OpenerClose(REG(a6, struct PhysFSLibBase *b))
{
    struct ExecBase *SysBase = b->sys;
    struct PhysFSLibBase *p = b->parent;
    user_cleanup(b);
    free_base(SysBase, b);
    if (--p->lib.lib_OpenCnt == 0 && (p->lib.lib_Flags & LIBF_DELEXP))
        return ParentExpunge(p);
    return 0;
}
