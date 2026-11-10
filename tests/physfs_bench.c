/*
 * PhysFSBench: where a physfs-heavy load spends its time, phase by phase,
 * through physfs.library on the Amiga or against PhysicsFS on the host (the
 * same source). For the host-run work (task 42): run it with AC_PROFILE=1 to
 * see which 68k code is hot, and before and after a change for the numbers.
 *
 *   PhysFSBench ARCHIVE [ARCHIVE...] [PASSES=n]
 *
 * Each archive is mounted, then, for every regular file in it:
 *   mount     PHYSFS_mount (the archive's directory read and parsed)
 *   enum      PHYSFS_enumerate of every drawer, recursively
 *   exists    PHYSFS_exists on every file's path
 *   stat      PHYSFS_stat on every file
 *   read      open, read all of it in 4 KB pieces, close (inflate for
 *             deflated zip members); a checksum of every byte read
 *   seek      open, seek to 3 places, read 64 bytes at each, close
 *   unmount   PHYSFS_unmount
 * One line per phase: the time in ms, the count, and for read the bytes and
 * the checksum, so two builds can be compared byte for byte.
 *
 * zlib licence, like PhysicsFS. Copyright (c) 2026 AmiCradle Team.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "physfs.h"

#ifdef __amigaos__
#include <exec/types.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <proto/timer.h>
struct Device *TimerBase;
static struct timerequest treq;
static int timer_open(void)
{
    if (OpenDevice((CONST_STRPTR) TIMERNAME, UNIT_MICROHZ, (struct IORequest *) &treq, 0))
        return 0;
    TimerBase = treq.tr_node.io_Device;
    return 1;
}
static void timer_close(void) { if (TimerBase) CloseDevice((struct IORequest *) &treq); }
static unsigned long long now_us(void)
{
    struct timeval tv;
    GetSysTime(&tv);
    return (unsigned long long) tv.tv_secs * 1000000ULL + tv.tv_micro;
}
#else
#include <time.h>
static int timer_open(void) { return 1; }
static void timer_close(void) { }
static unsigned long long now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long long) ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000;
}
#endif

#define MAXFILES 20000
#define MAXPATH 256
static char *paths[MAXFILES];
static int nfiles, ndirs;
static char piece[4096];
static PHYSFS_uint32 crc;            /* not a CRC: a cheap order-dependent sum */

static void sum(const unsigned char *p, PHYSFS_sint64 n)
{
    PHYSFS_uint32 s = crc;
    while (n-- > 0)
        s = (s << 5) + (s >> 27) + *p++;
    crc = s;
}

static PHYSFS_EnumerateCallbackResult walk(void *data, const char *dir, const char *name)
{
    char full[MAXPATH];
    PHYSFS_Stat st;
    (void) data;
    if (dir[0])
        snprintf(full, sizeof full, "%s/%s", dir, name);
    else
        snprintf(full, sizeof full, "%s", name);
    if (!PHYSFS_stat(full, &st))
        return PHYSFS_ENUM_ERROR;
    if (st.filetype == PHYSFS_FILETYPE_DIRECTORY)
    {
        ndirs++;
        return PHYSFS_enumerate(full, walk, NULL) ? PHYSFS_ENUM_OK : PHYSFS_ENUM_ERROR;
    }
    if (nfiles < MAXFILES && (paths[nfiles] = malloc(strlen(full) + 1)) != NULL)
        strcpy(paths[nfiles++], full);
    return PHYSFS_ENUM_OK;
}

static unsigned long long t0;
static void phase(const char *archive, const char *what, unsigned long count, const char *more)
{
    unsigned long long t = now_us();
    printf("%-10s %-24s %8lu.%03lu ms %7lu%s\n", what, archive, (unsigned long) ((t - t0) / 1000),
           (unsigned long) ((t - t0) % 1000), count, more ? more : "");
    t0 = now_us();
}

static int bench(const char *archive)
{
    const char *base = strrchr(archive, '/');
    char more[96];
    unsigned long long bytes = 0;
    PHYSFS_Stat st;
    int i, ok = 1;
    base = base ? base + 1 : archive;
    if (strchr(base, ':'))
        base = strchr(base, ':') + 1;

    nfiles = ndirs = 0;
    t0 = now_us();
    if (!PHYSFS_mount(archive, "b", 0))
    {
        printf("FAIL mount %s: %s\n", archive, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return 0;
    }
    phase(base, "mount", 1, NULL);

    if (!PHYSFS_enumerate("b", walk, NULL))
        ok = 0;
    snprintf(more, sizeof more, " files, %d drawers", ndirs);
    phase(base, "enum", (unsigned long) nfiles, more);

    for (i = 0; i < nfiles; i++)
        if (!PHYSFS_exists(paths[i]))
            ok = 0;
    phase(base, "exists", (unsigned long) nfiles, NULL);

    for (i = 0; i < nfiles; i++)
        if (!PHYSFS_stat(paths[i], &st) || st.filetype != PHYSFS_FILETYPE_REGULAR)
            ok = 0;
    phase(base, "stat", (unsigned long) nfiles, NULL);

    crc = 0;
    for (i = 0; i < nfiles; i++)
    {
        PHYSFS_File *f = PHYSFS_openRead(paths[i]);
        PHYSFS_sint64 got;
        if (!f) { ok = 0; continue; }
        while ((got = PHYSFS_readBytes(f, piece, sizeof piece)) > 0)
        {
            sum((const unsigned char *) piece, got);
            bytes += (unsigned long long) got;
        }
        if (got < 0)
            ok = 0;
        PHYSFS_close(f);
    }
    snprintf(more, sizeof more, " files, %lu KB, sum %08lx", (unsigned long) (bytes / 1024), (unsigned long) crc);
    phase(base, "read", (unsigned long) nfiles, more);

    crc = 0;
    for (i = 0; i < nfiles; i++)
    {
        PHYSFS_File *f = PHYSFS_openRead(paths[i]);
        PHYSFS_sint64 len, at[3];
        int k;
        if (!f) { ok = 0; continue; }
        len = PHYSFS_fileLength(f);
        at[0] = len / 2; at[1] = len / 4; at[2] = (len * 3) / 4;   /* forward, back, forward */
        for (k = 0; k < 3; k++)
        {
            PHYSFS_sint64 got;
            if (!PHYSFS_seek(f, (PHYSFS_uint64) at[k])) { ok = 0; break; }
            got = PHYSFS_readBytes(f, piece, 64);
            if (got < 0) { ok = 0; break; }
            sum((const unsigned char *) piece, got);
        }
        PHYSFS_close(f);
    }
    snprintf(more, sizeof more, " files, sum %08lx", (unsigned long) crc);
    phase(base, "seek", (unsigned long) nfiles, more);

    if (!PHYSFS_unmount(archive))
        ok = 0;
    phase(base, "unmount", 1, NULL);
    for (i = 0; i < nfiles; i++)
        free(paths[i]);
    if (!ok)
        printf("FAIL %s: %s\n", archive, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return ok;
}

int main(int argc, char **argv)
{
    int i, p, passes = 1, ok = 1;
    unsigned long long start;
    if (argc < 2)
    {
        printf("usage: PhysFSBench ARCHIVE [ARCHIVE...] [PASSES=n]\n");
        return 10;
    }
    if (!timer_open())
    {
        printf("no timer.device\n");
        return 20;
    }
    for (i = 1; i < argc; i++)
        if (strncmp(argv[i], "PASSES=", 7) == 0)
            passes = atoi(argv[i] + 7);
    if (!PHYSFS_init(argv[0]))
    {
        printf("FAIL init\n");
        timer_close();
        return 20;
    }
    start = now_us();
    for (p = 0; p < passes; p++)
        for (i = 1; i < argc; i++)
            if (strncmp(argv[i], "PASSES=", 7) != 0 && !bench(argv[i]))
                ok = 0;
    printf("total %lu ms, %d pass(es), %s\n", (unsigned long) ((now_us() - start) / 1000), passes,
           ok ? "ok" : "FAILED");
    PHYSFS_deinit();
    timer_close();
    return ok ? 0 : 5;
}
