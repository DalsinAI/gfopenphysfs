/*
 * physfs.library on AmigaOS: the part of zlib physfs_archiver_zip.c uses,
 * answered by z.library at run time (the Team's GFOpenZlib, 53.13 or later,
 * or the Aminet z.library 53) instead of PhysicsFS's own copy of miniz.
 * One inflate for every program on the machine, and the one AmiCradle runs
 * as host code (the host-run libraries plan, 9 November 2026).
 *
 * patches/0002-zip-inflate-from-zlibrary.patch includes this in place of
 * physfs_miniz.h when PHYSFS_ZLIBRARY is defined (build-amiga.sh). With
 * PHYSFS_ZLIBRARY_HOSTED (tests/host/run.sh) it is the system's zlib, so
 * the patched zip reader is checked on a PC against real zlib.
 *
 * zlib licence, Copyright (c) 2026 AmiCradle Team.
 */
#ifndef PHYSFS_ZLIBRARY_H
#define PHYSFS_ZLIBRARY_H

#ifdef PHYSFS_ZLIBRARY_HOSTED

#include <zlib.h>
#define physfs_zlibrary_close() ((void) 0)

#else

typedef unsigned int uInt;          /* 32 bits on the 68k, as z.library's */
typedef unsigned long uLong;
typedef unsigned char Bytef;
typedef void *voidpf;

#define Z_NO_FLUSH      0
#define Z_SYNC_FLUSH    2
#define Z_FINISH        4

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)

#define MAX_WBITS 15

/* zlib.h's z_stream as z.library lays it out on the 68k (56 bytes). The
 * zip reader sets zalloc, zfree and opaque to PhysicsFS's allocator; the
 * calls below clear them, so z.library allocates for itself. */
typedef struct z_stream_s {
    const Bytef *next_in;
    uInt avail_in;
    uLong total_in;
    Bytef *next_out;
    uInt avail_out;
    uLong total_out;
    const char *msg;
    void *state;
    voidpf (*zalloc)(voidpf opaque, uInt items, uInt size);
    void (*zfree)(voidpf opaque, voidpf address);
    voidpf opaque;
    int data_type;
    uLong adler;
    uLong reserved;
} z_stream;

/* inflateInit2 opens z.library the first time it is needed (53 or later,
 * not GFOpenZlib 53.12, whose jump table was wrong from -102 on). With none,
 * it answers Z_VERSION_ERROR, msg says what is missing, and the zip reader
 * reports the member as corrupt. */
int inflateInit2(z_stream *strm, int windowBits);
int inflate(z_stream *strm, int flush);
int inflateEnd(z_stream *strm);
int inflateReset(z_stream *strm);

/* Closes z.library: physfs.library's __UserLibCleanup, or atexit in a
 * program linked with libphysfs.a. */
void physfs_zlibrary_close(void);

#endif
#endif
