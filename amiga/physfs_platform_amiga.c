/*
 * AmigaOS 3.x support routines for PhysicsFS, on dos.library.
 *
 * AmigaDOS names a volume or assign with "name:" and a drawer with "/",
 * but a "/" at the end of a name (or right after the ":") means the
 * parent drawer, so every native name PhysicsFS hands us is tidied first:
 * "Work:Games/Descent/" opens "Work:Games/Descent" and "DH0:/data" opens
 * "DH0:data". The base, user and pref dirs are all the program's own
 * drawer (PROGDIR:), so settings and saves live beside the program.
 * Mutexes are exec SignalSemaphores (below).
 *
 * Written for AmigaOS by the AmiCradle Team (Team 2, for the DXX-Rebirth
 *  port), moved here by Team 5 on 8 Nov 2026; zlib licence, like PhysicsFS.
 */

#define __PHYSICSFS_INTERNAL__
#include "physfs_platforms.h"

#ifdef PHYSFS_PLATFORM_AMIGA

#include <exec/types.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <exec/semaphores.h>
#include <string.h>

#include "physfs_internal.h"

/* Seconds from 1970-01-01 to AmigaDOS's epoch, 1978-01-01. */
#define AMIGA_EPOCH_OFFSET 252460800

static PHYSFS_ErrorCode errcodeFromIoErr(LONG err)
{
    switch (err)
    {
        case 0: return PHYSFS_ERR_OK;
        case ERROR_NO_FREE_STORE: return PHYSFS_ERR_OUT_OF_MEMORY;
        case ERROR_OBJECT_IN_USE: return PHYSFS_ERR_BUSY;
        case ERROR_OBJECT_EXISTS: return PHYSFS_ERR_DUPLICATE;
        case ERROR_DIR_NOT_FOUND:
        case ERROR_OBJECT_NOT_FOUND:
        case ERROR_DEVICE_NOT_MOUNTED: return PHYSFS_ERR_NOT_FOUND;
        case ERROR_OBJECT_WRONG_TYPE: return PHYSFS_ERR_NOT_A_FILE;
        case ERROR_INVALID_COMPONENT_NAME: return PHYSFS_ERR_BAD_FILENAME;
        case ERROR_DISK_WRITE_PROTECTED:
        case ERROR_WRITE_PROTECTED: return PHYSFS_ERR_READ_ONLY;
        case ERROR_DIRECTORY_NOT_EMPTY: return PHYSFS_ERR_DIR_NOT_EMPTY;
        case ERROR_DISK_FULL: return PHYSFS_ERR_NO_SPACE;
        case ERROR_DELETE_PROTECTED:
        case ERROR_READ_PROTECTED: return PHYSFS_ERR_PERMISSION;
        case ERROR_SEEK_ERROR: return PHYSFS_ERR_PAST_EOF;
        default: return PHYSFS_ERR_OS_ERROR;
    } /* switch */
} /* errcodeFromIoErr */

static inline PHYSFS_ErrorCode errcodeFromDos(void)
{
    return errcodeFromIoErr(IoErr());
} /* errcodeFromDos */


/*
 * Copy (path) into (buf) as AmigaDOS wants it: no "/" at the end, and none
 *  straight after the ":". Names too long for the buffer are refused.
 */
static int amigaPath(const char *path, char *buf, size_t buflen)
{
    size_t len = strlen(path);
    char *colon;

    if (len >= buflen)
        return 0;
    memcpy(buf, path, len + 1);
    while (len > 0 && buf[len - 1] == '/')
        buf[--len] = '\0';
    colon = strchr(buf, ':');
    if (colon != NULL)
    {
        char *p = colon + 1;
        while (*p == '/')
            p++;
        memmove(colon + 1, p, strlen(p) + 1);
    } /* if */
    return 1;
} /* amigaPath */

#define AMIGA_PATH(var, path, failret) \
    char var[1024]; \
    BAIL_IF(!amigaPath((path), var, sizeof(var)), PHYSFS_ERR_BAD_FILENAME, failret)


int __PHYSFS_platformInit(void)
{
    return 1;  /* always succeed. */
} /* __PHYSFS_platformInit */


void __PHYSFS_platformDeinit(void)
{
    /* no-op */
} /* __PHYSFS_platformDeinit */


void __PHYSFS_platformDetectAvailableCDs(PHYSFS_StringCallback cb, void *data)
{
    /* no CD-ROM detection on AmigaOS. */
    (void) cb;
    (void) data;
} /* __PHYSFS_platformDetectAvailableCDs */


/* The full name of PROGDIR:, with a "/" at the end. */
static char *progdirName(void)
{
    char buf[1024];
    char *retval;
    size_t len;
    BPTR lock = Lock((CONST_STRPTR) "PROGDIR:", ACCESS_READ);

    if (lock)
    {
        const LONG ok = NameFromLock(lock, (STRPTR) buf, sizeof(buf) - 2);
        UnLock(lock);
        if (!ok)
            strcpy(buf, "PROGDIR:");
    } /* if */
    else
    {
        strcpy(buf, "PROGDIR:");
    } /* else */

    /* PhysicsFS asks for a separator at the end; amigaPath() drops it
     *  again before AmigaDOS sees the name. */
    len = strlen(buf);
    retval = (char *) allocator.Malloc(len + 2);
    BAIL_IF(!retval, PHYSFS_ERR_OUT_OF_MEMORY, NULL);
    memcpy(retval, buf, len);
    retval[len] = '/';
    retval[len + 1] = '\0';
    return retval;
} /* progdirName */


char *__PHYSFS_platformCalcBaseDir(const char *argv0)
{
    (void) argv0;
    return progdirName();
} /* __PHYSFS_platformCalcBaseDir */


char *__PHYSFS_platformCalcUserDir(void)
{
    return progdirName();
} /* __PHYSFS_platformCalcUserDir */


char *__PHYSFS_platformCalcPrefDir(const char *org, const char *app)
{
    (void) org;
    (void) app;
    return progdirName();
} /* __PHYSFS_platformCalcPrefDir */


void *__PHYSFS_platformGetThreadID(void)
{
    return (void *) FindTask(NULL);
} /* __PHYSFS_platformGetThreadID */


PHYSFS_EnumerateCallbackResult __PHYSFS_platformEnumerate(const char *dirname,
                               PHYSFS_EnumerateCallback callback,
                               const char *origdir, void *callbackdata)
{
    PHYSFS_EnumerateCallbackResult retval = PHYSFS_ENUM_OK;
    struct FileInfoBlock *fib;
    BPTR lock;
    AMIGA_PATH(name, dirname, PHYSFS_ENUM_ERROR);

    lock = Lock((CONST_STRPTR) name, ACCESS_READ);
    BAIL_IF(!lock, errcodeFromDos(), PHYSFS_ENUM_ERROR);
    fib = (struct FileInfoBlock *) AllocDosObject(DOS_FIB, NULL);
    if (!fib)
    {
        UnLock(lock);
        BAIL(PHYSFS_ERR_OUT_OF_MEMORY, PHYSFS_ENUM_ERROR);
    } /* if */

    if (!Examine(lock, fib) || fib->fib_DirEntryType <= 0)
    {
        FreeDosObject(DOS_FIB, fib);
        UnLock(lock);
        BAIL(PHYSFS_ERR_NOT_FOUND, PHYSFS_ENUM_ERROR);
    } /* if */

    while (retval == PHYSFS_ENUM_OK && ExNext(lock, fib))
    {
        retval = callback(callbackdata, origdir, (const char *) fib->fib_FileName);
        if (retval == PHYSFS_ENUM_ERROR)
            PHYSFS_setErrorCode(PHYSFS_ERR_APP_CALLBACK);
    } /* while */

    FreeDosObject(DOS_FIB, fib);
    UnLock(lock);
    return retval;
} /* __PHYSFS_platformEnumerate */


int __PHYSFS_platformMkDir(const char *path)
{
    BPTR lock;
    AMIGA_PATH(name, path, 0);

    lock = CreateDir((CONST_STRPTR) name);
    BAIL_IF(!lock, errcodeFromDos(), 0);
    UnLock(lock);
    return 1;
} /* __PHYSFS_platformMkDir */


/*
 * A file PhysicsFS opened. A file opened for reading reads through a buffer
 * of its own: dos.library's Read() is a packet to the filesystem for every
 * call, and PhysicsFS's archivers read their directories a field at a time
 * (a zip's central directory is 17 reads of 2 or 4 bytes an entry, its local
 * headers 10 more). Measured on PhysFSBench (task 42): mounting a zip of
 * 3,000 files was 51,050 Reads, and stat'ing them 30,020 more.
 * The buffer holds the bytes of the file from bufpos to bufpos + buflen, and
 * the next byte to give is buf[bufat]; the handle's own position is then
 * bufpos + buflen. A read as large as the buffer, with the buffer used up,
 * goes straight to Read() into the caller's memory. Files opened for writing
 * or appending have no buffer and behave as before.
 */
#define AMIGA_READBUF 4096

typedef struct
{
    BPTR fh;
    UBYTE *buf;        /* NULL: not buffered (written files) */
    LONG bufpos;       /* the file offset of buf[0] */
    LONG buflen;       /* bytes in buf */
    LONG bufat;        /* the next byte to give, 0 to buflen */
} AmigaFile;

static void *doOpen(const char *filename, LONG mode, int appending)
{
    BPTR fh;
    AmigaFile *retval;
    AMIGA_PATH(name, filename, NULL);

    fh = Open((CONST_STRPTR) name, mode);
    BAIL_IF(!fh, errcodeFromDos(), NULL);

    if (appending && Seek(fh, 0, OFFSET_END) == -1)
    {
        const PHYSFS_ErrorCode err = errcodeFromDos();
        Close(fh);
        BAIL(err, NULL);
    } /* if */

    retval = (AmigaFile *) allocator.Malloc(sizeof (AmigaFile) + (mode == MODE_OLDFILE ? AMIGA_READBUF : 0));
    if (!retval)
    {
        Close(fh);
        BAIL(PHYSFS_ERR_OUT_OF_MEMORY, NULL);
    } /* if */

    retval->fh = fh;
    retval->buf = (mode == MODE_OLDFILE) ? (UBYTE *) (retval + 1) : NULL;
    retval->bufpos = 0;
    retval->buflen = 0;
    retval->bufat = 0;
    return retval;
} /* doOpen */


void *__PHYSFS_platformOpenRead(const char *filename)
{
    return doOpen(filename, MODE_OLDFILE, 0);
} /* __PHYSFS_platformOpenRead */


void *__PHYSFS_platformOpenWrite(const char *filename)
{
    return doOpen(filename, MODE_NEWFILE, 0);
} /* __PHYSFS_platformOpenWrite */


void *__PHYSFS_platformOpenAppend(const char *filename)
{
    /* MODE_READWRITE opens an old file or makes a new one, unlocked. */
    return doOpen(filename, MODE_READWRITE, 1);
} /* __PHYSFS_platformOpenAppend */


PHYSFS_sint64 __PHYSFS_platformRead(void *opaque, void *buffer,
                                    PHYSFS_uint64 len)
{
    AmigaFile *f = (AmigaFile *) opaque;
    UBYTE *out = (UBYTE *) buffer;
    LONG want, got = 0, rc = 0;

    BAIL_IF(len > 0x7FFFFFFF, PHYSFS_ERR_INVALID_ARGUMENT, -1);
    want = (LONG) len;
    if (!f->buf)
    {
        rc = Read(f->fh, buffer, want);
        BAIL_IF(rc == -1, errcodeFromDos(), -1);
        return (PHYSFS_sint64) rc;
    } /* if */

    while (want > 0)
    {
        LONG have = f->buflen - f->bufat;
        if (have > 0)
        {
            if (have > want)
                have = want;
            memcpy(out, f->buf + f->bufat, (size_t) have);
            f->bufat += have;
            out += have;
            got += have;
            want -= have;
            continue;
        } /* if */

        /* the buffer is used up: the handle is at bufpos + buflen */
        f->bufpos += f->buflen;
        f->buflen = f->bufat = 0;
        if (want >= AMIGA_READBUF)
        {
            rc = Read(f->fh, out, want);
            if (rc == -1)
                break;
            f->bufpos += rc;
            got += rc;
            break;   /* all of it, or the end of the file */
        } /* if */

        rc = Read(f->fh, f->buf, AMIGA_READBUF);
        if (rc <= 0)
            break;
        f->buflen = rc;
    } /* while */

    BAIL_IF(got == 0 && rc == -1, errcodeFromDos(), -1);
    return (PHYSFS_sint64) got;
} /* __PHYSFS_platformRead */


PHYSFS_sint64 __PHYSFS_platformWrite(void *opaque, const void *buffer,
                                     PHYSFS_uint64 len)
{
    const BPTR fh = ((AmigaFile *) opaque)->fh;
    LONG rc;

    BAIL_IF(len > 0x7FFFFFFF, PHYSFS_ERR_INVALID_ARGUMENT, -1);
    rc = Write(fh, (APTR) buffer, (LONG) len);
    BAIL_IF(rc == -1, errcodeFromDos(), -1);
    return (PHYSFS_sint64) rc;
} /* __PHYSFS_platformWrite */


int __PHYSFS_platformSeek(void *opaque, PHYSFS_uint64 pos)
{
    AmigaFile *f = (AmigaFile *) opaque;
    BAIL_IF(pos > 0x7FFFFFFF, PHYSFS_ERR_INVALID_ARGUMENT, 0);
    if (f->buf)
    {
        /* inside what the buffer holds: no packet */
        if ((LONG) pos >= f->bufpos && (LONG) pos <= f->bufpos + f->buflen)
        {
            f->bufat = (LONG) pos - f->bufpos;
            return 1;
        } /* if */
        BAIL_IF(Seek(f->fh, (LONG) pos, OFFSET_BEGINNING) == -1, errcodeFromDos(), 0);
        f->bufpos = (LONG) pos;
        f->buflen = f->bufat = 0;
        return 1;
    } /* if */
    BAIL_IF(Seek(f->fh, (LONG) pos, OFFSET_BEGINNING) == -1, errcodeFromDos(), 0);
    return 1;
} /* __PHYSFS_platformSeek */


PHYSFS_sint64 __PHYSFS_platformTell(void *opaque)
{
    AmigaFile *f = (AmigaFile *) opaque;
    LONG pos;
    if (f->buf)
        return (PHYSFS_sint64) (f->bufpos + f->bufat);
    pos = Seek(f->fh, 0, OFFSET_CURRENT);
    BAIL_IF(pos == -1, errcodeFromDos(), -1);
    return (PHYSFS_sint64) pos;
} /* __PHYSFS_platformTell */


PHYSFS_sint64 __PHYSFS_platformFileLength(void *opaque)
{
    const BPTR fh = ((AmigaFile *) opaque)->fh;
    PHYSFS_sint64 retval = -1;
    struct FileInfoBlock *fib;

    fib = (struct FileInfoBlock *) AllocDosObject(DOS_FIB, NULL);
    BAIL_IF(!fib, PHYSFS_ERR_OUT_OF_MEMORY, -1);
    if (ExamineFH(fh, fib))
        retval = (PHYSFS_sint64) fib->fib_Size;
    else
        PHYSFS_setErrorCode(errcodeFromDos());
    FreeDosObject(DOS_FIB, fib);
    return retval;
} /* __PHYSFS_platformFileLength */


int __PHYSFS_platformFlush(void *opaque)
{
    const BPTR fh = ((AmigaFile *) opaque)->fh;
    BAIL_IF(!Flush(fh), errcodeFromDos(), 0);
    return 1;
} /* __PHYSFS_platformFlush */


void __PHYSFS_platformClose(void *opaque)
{
    Close(((AmigaFile *) opaque)->fh);
    allocator.Free(opaque);
} /* __PHYSFS_platformClose */


int __PHYSFS_platformDelete(const char *path)
{
    AMIGA_PATH(name, path, 0);
    BAIL_IF(!DeleteFile((CONST_STRPTR) name), errcodeFromDos(), 0);
    return 1;
} /* __PHYSFS_platformDelete */


int __PHYSFS_platformStat(const char *filename, PHYSFS_Stat *st, const int follow)
{
    struct FileInfoBlock *fib;
    BPTR lock;
    AMIGA_PATH(name, filename, 0);

    (void) follow;  /* Lock() follows links already. */
    lock = Lock((CONST_STRPTR) name, ACCESS_READ);
    BAIL_IF(!lock, errcodeFromDos(), 0);
    fib = (struct FileInfoBlock *) AllocDosObject(DOS_FIB, NULL);
    if (!fib)
    {
        UnLock(lock);
        BAIL(PHYSFS_ERR_OUT_OF_MEMORY, 0);
    } /* if */
    if (!Examine(lock, fib))
    {
        const PHYSFS_ErrorCode err = errcodeFromDos();
        FreeDosObject(DOS_FIB, fib);
        UnLock(lock);
        BAIL(err, 0);
    } /* if */

    if (fib->fib_DirEntryType > 0)
    {
        st->filetype = PHYSFS_FILETYPE_DIRECTORY;
        st->filesize = 0;
    } /* if */
    else
    {
        st->filetype = PHYSFS_FILETYPE_REGULAR;
        st->filesize = (PHYSFS_sint64) fib->fib_Size;
    } /* else */

    st->modtime = AMIGA_EPOCH_OFFSET
        + (PHYSFS_sint64) fib->fib_Date.ds_Days * 86400
        + (PHYSFS_sint64) fib->fib_Date.ds_Minute * 60
        + fib->fib_Date.ds_Tick / TICKS_PER_SECOND;
    st->createtime = st->modtime;
    st->accesstime = st->modtime;
    /* A set protection bit means the action is denied. */
    st->readonly = (fib->fib_Protection & FIBF_WRITE) ? 1 : 0;

    FreeDosObject(DOS_FIB, fib);
    UnLock(lock);
    return 1;
} /* __PHYSFS_platformStat */


/*
 * Mutexes are exec SignalSemaphores. PhysicsFS needs them recursive, and a
 *  task may obtain a semaphore it already holds, so they are. They matter in
 *  physfs.library: a program's main task and its sound task (SDL_mixer
 *  streaming from a PhysicsFS file) can call in at the same time.
 */
void *__PHYSFS_platformCreateMutex(void)
{
    struct SignalSemaphore *sem;
    sem = (struct SignalSemaphore *) allocator.Malloc(sizeof (*sem));
    BAIL_IF(!sem, PHYSFS_ERR_OUT_OF_MEMORY, NULL);
    memset(sem, 0, sizeof (*sem));
    InitSemaphore(sem);
    return sem;
} /* __PHYSFS_platformCreateMutex */


void __PHYSFS_platformDestroyMutex(void *mutex)
{
    allocator.Free(mutex);
} /* __PHYSFS_platformDestroyMutex */


int __PHYSFS_platformGrabMutex(void *mutex)
{
    ObtainSemaphore((struct SignalSemaphore *) mutex);
    return 1;
} /* __PHYSFS_platformGrabMutex */


void __PHYSFS_platformReleaseMutex(void *mutex)
{
    ReleaseSemaphore((struct SignalSemaphore *) mutex);
} /* __PHYSFS_platformReleaseMutex */

#endif  /* PHYSFS_PLATFORM_AMIGA */

/* end of physfs_platform_amiga.c ... */
