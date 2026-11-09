/* physfs.library calls, made by gfopenphysfs tools/gen.py. Included by <physfs.h>. */
#ifndef _INLINE_PHYSFS_H
#define _INLINE_PHYSFS_H
#include <exec/types.h>
struct Library;
extern struct Library *PhysFSBase;

static __inline__ void PHYSFS_getLinkedVersion(PHYSFS_Version *ver)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(ver);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-30:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ int PHYSFS_init(const char *argv0)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(argv0);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-36:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_deinit(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-42:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ const PHYSFS_ArchiveInfo ** PHYSFS_supportedArchiveTypes(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-48:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const PHYSFS_ArchiveInfo **)_d0;
}

static __inline__ void PHYSFS_freeList(void *listVar)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(listVar);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-54:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ const char * PHYSFS_getLastError(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-60:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ const char * PHYSFS_getDirSeparator(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-66:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ void PHYSFS_permitSymbolicLinks(int allow)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(allow);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-72:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ char ** PHYSFS_getCdRomDirs(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-78:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (char **)_d0;
}

static __inline__ const char * PHYSFS_getBaseDir(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-84:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ const char * PHYSFS_getUserDir(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-90:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ const char * PHYSFS_getWriteDir(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-96:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ int PHYSFS_setWriteDir(const char *newDir)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(newDir);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-102:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_addToSearchPath(const char *newDir, int appendToPath)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(appendToPath);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(newDir);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-108:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_removeFromSearchPath(const char *oldDir)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(oldDir);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-114:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ char ** PHYSFS_getSearchPath(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-120:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (char **)_d0;
}

static __inline__ int PHYSFS_setSaneConfig(const char *organization, const char *appName, const char *archiveExt, int includeCdRoms, int archivesFirst)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(includeCdRoms);
    register ULONG _d1 __asm("d1") = (ULONG)(LONG)(archivesFirst);
    register ULONG _a0 __asm("a0") = (ULONG)(organization);
    register ULONG _a1 __asm("a1") = (ULONG)(appName);
    register ULONG _a2 __asm("a2") = (ULONG)(archiveExt);
    __asm volatile ("jsr a6@(-126:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base), "r"(_a2) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_mkdir(const char *dirName)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(dirName);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-132:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_delete(const char *filename)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(filename);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-138:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ const char * PHYSFS_getRealDir(const char *filename)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(filename);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-144:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ char ** PHYSFS_enumerateFiles(const char *dir)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(dir);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-150:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (char **)_d0;
}

static __inline__ int PHYSFS_exists(const char *fname)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(fname);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-156:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_isDirectory(const char *fname)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(fname);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-162:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_isSymbolicLink(const char *fname)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(fname);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-168:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ PHYSFS_sint64 PHYSFS_getLastModTime(const char *filename)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(filename);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-174:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ PHYSFS_File * PHYSFS_openWrite(const char *filename)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(filename);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-180:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_File *)_d0;
}

static __inline__ PHYSFS_File * PHYSFS_openAppend(const char *filename)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(filename);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-186:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_File *)_d0;
}

static __inline__ PHYSFS_File * PHYSFS_openRead(const char *filename)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(filename);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-192:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_File *)_d0;
}

static __inline__ int PHYSFS_close(PHYSFS_File *handle)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-198:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ PHYSFS_sint64 PHYSFS_read(PHYSFS_File *handle, void *buffer, PHYSFS_uint32 objSize, PHYSFS_uint32 objCount)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(objSize);
    register ULONG _d1 __asm("d1") = (ULONG)(LONG)(objCount);
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1") = (ULONG)(buffer);
    __asm volatile ("jsr a6@(-204:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ PHYSFS_sint64 PHYSFS_write(PHYSFS_File *handle, const void *buffer, PHYSFS_uint32 objSize, PHYSFS_uint32 objCount)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(objSize);
    register ULONG _d1 __asm("d1") = (ULONG)(LONG)(objCount);
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1") = (ULONG)(buffer);
    __asm volatile ("jsr a6@(-210:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ int PHYSFS_eof(PHYSFS_File *handle)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-216:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ PHYSFS_sint64 PHYSFS_tell(PHYSFS_File *handle)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-222:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ int PHYSFS_seek(PHYSFS_File *handle, PHYSFS_uint64 pos)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(pos) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(pos);
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-228:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ PHYSFS_sint64 PHYSFS_fileLength(PHYSFS_File *handle)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-234:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ int PHYSFS_setBuffer(PHYSFS_File *handle, PHYSFS_uint64 bufsize)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(bufsize) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(bufsize);
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-240:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_flush(PHYSFS_File *handle)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-246:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ PHYSFS_sint16 PHYSFS_swapSLE16(PHYSFS_sint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-252:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint16)(LONG)_d0;
}

static __inline__ PHYSFS_uint16 PHYSFS_swapULE16(PHYSFS_uint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-258:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_uint16)(LONG)_d0;
}

static __inline__ PHYSFS_sint32 PHYSFS_swapSLE32(PHYSFS_sint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-264:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint32)(LONG)_d0;
}

static __inline__ PHYSFS_uint32 PHYSFS_swapULE32(PHYSFS_uint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-270:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_uint32)(LONG)_d0;
}

static __inline__ PHYSFS_sint64 PHYSFS_swapSLE64(PHYSFS_sint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-276:W)" : "+r"(_d0), "+r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ PHYSFS_uint64 PHYSFS_swapULE64(PHYSFS_uint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-282:W)" : "+r"(_d0), "+r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_uint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ PHYSFS_sint16 PHYSFS_swapSBE16(PHYSFS_sint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-288:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint16)(LONG)_d0;
}

static __inline__ PHYSFS_uint16 PHYSFS_swapUBE16(PHYSFS_uint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-294:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_uint16)(LONG)_d0;
}

static __inline__ PHYSFS_sint32 PHYSFS_swapSBE32(PHYSFS_sint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-300:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint32)(LONG)_d0;
}

static __inline__ PHYSFS_uint32 PHYSFS_swapUBE32(PHYSFS_uint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-306:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_uint32)(LONG)_d0;
}

static __inline__ PHYSFS_sint64 PHYSFS_swapSBE64(PHYSFS_sint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-312:W)" : "+r"(_d0), "+r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ PHYSFS_uint64 PHYSFS_swapUBE64(PHYSFS_uint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-318:W)" : "+r"(_d0), "+r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_uint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ int PHYSFS_readSLE16(PHYSFS_File *file, PHYSFS_sint16 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-324:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readULE16(PHYSFS_File *file, PHYSFS_uint16 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-330:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readSBE16(PHYSFS_File *file, PHYSFS_sint16 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-336:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readUBE16(PHYSFS_File *file, PHYSFS_uint16 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-342:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readSLE32(PHYSFS_File *file, PHYSFS_sint32 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-348:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readULE32(PHYSFS_File *file, PHYSFS_uint32 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-354:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readSBE32(PHYSFS_File *file, PHYSFS_sint32 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-360:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readUBE32(PHYSFS_File *file, PHYSFS_uint32 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-366:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readSLE64(PHYSFS_File *file, PHYSFS_sint64 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-372:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readULE64(PHYSFS_File *file, PHYSFS_uint64 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-378:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readSBE64(PHYSFS_File *file, PHYSFS_sint64 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-384:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_readUBE64(PHYSFS_File *file, PHYSFS_uint64 *val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(val);
    __asm volatile ("jsr a6@(-390:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeSLE16(PHYSFS_File *file, PHYSFS_sint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-396:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeULE16(PHYSFS_File *file, PHYSFS_uint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-402:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeSBE16(PHYSFS_File *file, PHYSFS_sint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-408:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeUBE16(PHYSFS_File *file, PHYSFS_uint16 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-414:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeSLE32(PHYSFS_File *file, PHYSFS_sint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-420:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeULE32(PHYSFS_File *file, PHYSFS_uint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-426:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeSBE32(PHYSFS_File *file, PHYSFS_sint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-432:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeUBE32(PHYSFS_File *file, PHYSFS_uint32 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(val);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-438:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeSLE64(PHYSFS_File *file, PHYSFS_sint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-444:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeULE64(PHYSFS_File *file, PHYSFS_uint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-450:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeSBE64(PHYSFS_File *file, PHYSFS_sint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-456:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_writeUBE64(PHYSFS_File *file, PHYSFS_uint64 val)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(val) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(val);
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-462:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_isInit(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-468:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_symbolicLinksPermitted(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-474:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_setAllocator(const PHYSFS_Allocator *allocator)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(allocator);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-480:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_mount(const char *newDir, const char *mountPoint, int appendToPath)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(appendToPath);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(newDir);
    register ULONG _a1 __asm("a1") = (ULONG)(mountPoint);
    __asm volatile ("jsr a6@(-486:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ const char * PHYSFS_getMountPoint(const char *dir)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(dir);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-492:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ void PHYSFS_getCdRomDirsCallback(PHYSFS_StringCallback c, void *d)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(c);
    register ULONG _a1 __asm("a1") = (ULONG)(d);
    __asm volatile ("jsr a6@(-498:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_getSearchPathCallback(PHYSFS_StringCallback c, void *d)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(c);
    register ULONG _a1 __asm("a1") = (ULONG)(d);
    __asm volatile ("jsr a6@(-504:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_enumerateFilesCallback(const char *dir, PHYSFS_EnumFilesCallback c, void *d)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(dir);
    register ULONG _a1 __asm("a1") = (ULONG)(c);
    register ULONG _a2 __asm("a2") = (ULONG)(d);
    __asm volatile ("jsr a6@(-510:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base), "r"(_a2) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_utf8FromUcs4(const PHYSFS_uint32 *src, char *dst, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(src);
    register ULONG _a1 __asm("a1") = (ULONG)(dst);
    __asm volatile ("jsr a6@(-516:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_utf8ToUcs4(const char *src, PHYSFS_uint32 *dst, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(src);
    register ULONG _a1 __asm("a1") = (ULONG)(dst);
    __asm volatile ("jsr a6@(-522:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_utf8FromUcs2(const PHYSFS_uint16 *src, char *dst, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(src);
    register ULONG _a1 __asm("a1") = (ULONG)(dst);
    __asm volatile ("jsr a6@(-528:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_utf8ToUcs2(const char *src, PHYSFS_uint16 *dst, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(src);
    register ULONG _a1 __asm("a1") = (ULONG)(dst);
    __asm volatile ("jsr a6@(-534:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_utf8FromLatin1(const char *src, char *dst, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(src);
    register ULONG _a1 __asm("a1") = (ULONG)(dst);
    __asm volatile ("jsr a6@(-540:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ int PHYSFS_caseFold(const PHYSFS_uint32 from, PHYSFS_uint32 *to)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(from);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(to);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-546:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_utf8stricmp(const char *str1, const char *str2)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(str1);
    register ULONG _a1 __asm("a1") = (ULONG)(str2);
    __asm volatile ("jsr a6@(-552:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_utf16stricmp(const PHYSFS_uint16 *str1, const PHYSFS_uint16 *str2)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(str1);
    register ULONG _a1 __asm("a1") = (ULONG)(str2);
    __asm volatile ("jsr a6@(-558:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_ucs4stricmp(const PHYSFS_uint32 *str1, const PHYSFS_uint32 *str2)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(str1);
    register ULONG _a1 __asm("a1") = (ULONG)(str2);
    __asm volatile ("jsr a6@(-564:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_enumerate(const char *dir, PHYSFS_EnumerateCallback c, void *d)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(dir);
    register ULONG _a1 __asm("a1") = (ULONG)(c);
    register ULONG _a2 __asm("a2") = (ULONG)(d);
    __asm volatile ("jsr a6@(-570:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base), "r"(_a2) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_unmount(const char *oldDir)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(oldDir);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-576:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ const PHYSFS_Allocator * PHYSFS_getAllocator(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-582:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const PHYSFS_Allocator *)_d0;
}

static __inline__ int PHYSFS_stat(const char *fname, PHYSFS_Stat *stat)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(fname);
    register ULONG _a1 __asm("a1") = (ULONG)(stat);
    __asm volatile ("jsr a6@(-588:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ void PHYSFS_utf8FromUtf16(const PHYSFS_uint16 *src, char *dst, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(src);
    register ULONG _a1 __asm("a1") = (ULONG)(dst);
    __asm volatile ("jsr a6@(-594:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ void PHYSFS_utf8ToUtf16(const char *src, PHYSFS_uint16 *dst, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(src);
    register ULONG _a1 __asm("a1") = (ULONG)(dst);
    __asm volatile ("jsr a6@(-600:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ PHYSFS_sint64 PHYSFS_readBytes(PHYSFS_File *handle, void *buffer, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1") = (ULONG)(buffer);
    __asm volatile ("jsr a6@(-606:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ PHYSFS_sint64 PHYSFS_writeBytes(PHYSFS_File *handle, const void *buffer, PHYSFS_uint64 len)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(handle);
    register ULONG _a1 __asm("a1") = (ULONG)(buffer);
    __asm volatile ("jsr a6@(-612:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_sint64)(((PHYSFS_uint64)_d0 << 32) | _d1);
}

static __inline__ int PHYSFS_mountIo(PHYSFS_Io *io, const char *newDir, const char *mountPoint, int appendToPath)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(appendToPath);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(io);
    register ULONG _a1 __asm("a1") = (ULONG)(newDir);
    register ULONG _a2 __asm("a2") = (ULONG)(mountPoint);
    __asm volatile ("jsr a6@(-618:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base), "r"(_a2) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_mountMemory(const void *buf, PHYSFS_uint64 len, void (*del)(void *), const char *newDir, const char *mountPoint, int appendToPath)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)((PHYSFS_uint64)(len) >> 32);
    register ULONG _d1 __asm("d1") = (ULONG)(PHYSFS_uint64)(len);
    register ULONG _a0 __asm("a0") = (ULONG)(buf);
    register ULONG _a1 __asm("a1") = (ULONG)(del);
    register ULONG _a2 __asm("a2") = (ULONG)(newDir);
    register ULONG _a3 __asm("a3") = (ULONG)(mountPoint);
    register ULONG _d2 __asm("d2") = (ULONG)(LONG)(appendToPath);
    __asm volatile ("jsr a6@(-624:W)" : "+r"(_d0), "+r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base), "r"(_a2), "r"(_a3), "r"(_d2) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_mountHandle(PHYSFS_File *file, const char *newDir, const char *mountPoint, int appendToPath)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(appendToPath);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(file);
    register ULONG _a1 __asm("a1") = (ULONG)(newDir);
    register ULONG _a2 __asm("a2") = (ULONG)(mountPoint);
    __asm volatile ("jsr a6@(-630:W)" : "+r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base), "r"(_a2) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ PHYSFS_ErrorCode PHYSFS_getLastErrorCode(void)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-636:W)" : "=r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (PHYSFS_ErrorCode)(LONG)_d0;
}

static __inline__ const char * PHYSFS_getErrorByCode(PHYSFS_ErrorCode code)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(code);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-642:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ void PHYSFS_setErrorCode(PHYSFS_ErrorCode code)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0") = (ULONG)(LONG)(code);
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0");
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-648:W)" : "+r"(_d0), "=r"(_d1), "=r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
}

static __inline__ const char * PHYSFS_getPrefDir(const char *org, const char *app)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(org);
    register ULONG _a1 __asm("a1") = (ULONG)(app);
    __asm volatile ("jsr a6@(-654:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (const char *)_d0;
}

static __inline__ int PHYSFS_registerArchiver(const PHYSFS_Archiver *archiver)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(archiver);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-660:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_deregisterArchiver(const char *ext)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(ext);
    register ULONG _a1 __asm("a1");
    __asm volatile ("jsr a6@(-666:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "=r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

static __inline__ int PHYSFS_setRoot(const char *archive, const char *subdir)
{
    register struct Library *_base __asm("a6") = PhysFSBase;
    register ULONG _d0 __asm("d0");
    register ULONG _d1 __asm("d1");
    register ULONG _a0 __asm("a0") = (ULONG)(archive);
    register ULONG _a1 __asm("a1") = (ULONG)(subdir);
    __asm volatile ("jsr a6@(-672:W)" : "=r"(_d0), "=r"(_d1), "+r"(_a0), "+r"(_a1) : "r"(_base) : "fp0", "fp1", "cc", "memory");
    return (int)(LONG)_d0;
}

#endif
