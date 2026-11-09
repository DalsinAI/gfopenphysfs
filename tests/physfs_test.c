/*
 * PhysFSTest: PhysicsFS's calls, checked, through physfs.library on the
 * Amiga or against PhysicsFS itself on the host (the same source).
 *
 *   PhysFSTest WRITEDIR [NAME]
 *
 * WRITEDIR is a drawer the test may write in (T: on the Amiga); NAME (default
 * "a") names its own files, so two copies can run at once. Prints one line
 * per check and "PASS n/n", and returns 0, or 5 when a check fails.
 *
 * zlib licence, like PhysicsFS. Copyright (c) 2026 AmiCradle Team.
 */
#include <stdio.h>
#include <string.h>
#include "physfs.h"
#include "test_zip.h"

static int checks, failures;

static void check(int ok, const char *what)
{
    checks++;
    if (!ok)
    {
        failures++;
        printf("FAIL %s (%s)\n", what, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
    else
        printf("ok   %s\n", what);
}

static int found;
static PHYSFS_EnumerateCallbackResult seen(void *data, const char *dir, const char *name)
{
    (void) dir;
    if (strcmp(name, (const char *) data) == 0)
        found = 1;
    return PHYSFS_ENUM_OK;
}

static int read_all(const char *name, char *buf, PHYSFS_uint64 size, PHYSFS_sint64 *got)
{
    PHYSFS_File *f = PHYSFS_openRead(name);
    if (!f)
        return 0;
    *got = PHYSFS_readBytes(f, buf, size);
    PHYSFS_close(f);
    return 1;
}

int main(int argc, char **argv)
{
    static char buf[2048];
    char dir[64], file[96], hello[96];
    PHYSFS_Version v;
    PHYSFS_File *f;
    PHYSFS_Stat st;
    PHYSFS_sint64 got = 0;
    const char *name = argc > 2 ? argv[2] : "a";
    int i;

    if (argc < 2)
    {
        printf("usage: PhysFSTest WRITEDIR [NAME]\n");
        return 10;
    }
    snprintf(dir, sizeof dir, "t5physfs-%s", name);
    snprintf(file, sizeof file, "%s/hello.txt", dir);
    snprintf(hello, sizeof hello, "Hello from %s.\n", name);

    PHYSFS_getLinkedVersion(&v);
    check(v.major == 3 && v.minor == 2 && v.patch == 0, "linked version 3.2.0");
    check(!PHYSFS_isInit(), "not yet initialised");
    check(PHYSFS_init(argv[0]) != 0, "PHYSFS_init");
    check(PHYSFS_isInit(), "initialised");
    check(PHYSFS_getBaseDir() && PHYSFS_getBaseDir()[0], "base dir");
    check(PHYSFS_setWriteDir(argv[1]) != 0, "setWriteDir");
    check(PHYSFS_mkdir(dir) != 0, "mkdir");

    f = PHYSFS_openWrite(file);
    check(f != NULL, "openWrite");
    if (f)
    {
        check(PHYSFS_writeBytes(f, hello, strlen(hello)) == (PHYSFS_sint64) strlen(hello), "writeBytes (64-bit length)");
        check(PHYSFS_writeULE32(f, 0x01020304) != 0, "writeULE32");
        check(PHYSFS_writeSBE64(f, -2) != 0, "writeSBE64 (64-bit argument)");
        check(PHYSFS_close(f) != 0, "close");
    }

    check(PHYSFS_mount(argv[1], NULL, 1) != 0, "mount the write dir");
    check(PHYSFS_exists(file), "exists");
    check(PHYSFS_stat(file, &st) && st.filetype == PHYSFS_FILETYPE_REGULAR
          && st.filesize == (PHYSFS_sint64) strlen(hello) + 12, "stat: a regular file, the right size");
    f = PHYSFS_openRead(file);
    check(f != NULL, "openRead");
    if (f)
    {
        PHYSFS_uint32 u32 = 0;
        PHYSFS_sint64 s64 = 0;
        check(PHYSFS_fileLength(f) == (PHYSFS_sint64) strlen(hello) + 12, "fileLength (64-bit result)");
        check(PHYSFS_readBytes(f, buf, strlen(hello)) == (PHYSFS_sint64) strlen(hello)
              && memcmp(buf, hello, strlen(hello)) == 0, "readBytes: what was written");
        check(PHYSFS_readULE32(f, &u32) && u32 == 0x01020304, "readULE32");
        check(PHYSFS_readSBE64(f, &s64) && s64 == -2, "readSBE64");
        check(PHYSFS_eof(f), "eof");
        check(PHYSFS_seek(f, 6) && PHYSFS_tell(f) == 6, "seek and tell (64-bit both ways)");
        check(PHYSFS_close(f) != 0, "close");
    }

    found = 0;
    check(PHYSFS_enumerate(dir, seen, "hello.txt") != 0 && found, "enumerate (a callback)");
    check(PHYSFS_openRead("no-such-file") == NULL
          && PHYSFS_getLastErrorCode() == PHYSFS_ERR_NOT_FOUND, "a missing file: PHYSFS_ERR_NOT_FOUND");

    check(PHYSFS_mountMemory(test_zip, sizeof test_zip, NULL, "test.zip", "zip", 1) != 0,
          "mountMemory (six arguments)");
    check(read_all("zip/data/stored.txt", buf, sizeof buf, &got) && got == 14
          && memcmp(buf, "stored member\n", 14) == 0, "zip: a stored member");
    memset(buf, 0, sizeof buf);
    check(read_all("zip/data/deflated.txt", buf, sizeof buf, &got) && got == 1025
          && memcmp(buf + 1008, "deflated member \n", 17) == 0, "zip: a deflated member (inflate)");
    {   /* back to an earlier place in a deflated member: inflate starts again */
        PHYSFS_File *f = PHYSFS_openRead("zip/data/deflated.txt");
        char a[17], b[17];
        int ok = f && PHYSFS_seek(f, 1008) && PHYSFS_readBytes(f, a, 17) == 17
                 && PHYSFS_seek(f, 0) && PHYSFS_seek(f, 1008) && PHYSFS_readBytes(f, b, 17) == 17
                 && memcmp(a, b, 17) == 0 && memcmp(a, "deflated member \n", 17) == 0;
        if (f) PHYSFS_close(f);
        check(ok, "zip: seek back in a deflated member");
    }
    check(strcmp(PHYSFS_getMountPoint("test.zip"), "zip/") == 0, "getMountPoint");
    check(PHYSFS_unmount("test.zip") != 0, "unmount");

    check(PHYSFS_swapULE64(1) == ((PHYSFS_uint64) 1 << 56) || PHYSFS_swapULE64(1) == 1, "swapULE64 (64-bit result)");
    check(PHYSFS_swapUBE16(0x1234) == 0x1234 || PHYSFS_swapUBE16(0x1234) == 0x3412, "swapUBE16");
    check(PHYSFS_utf8stricmp("PhysicsFS", "physicsfs") == 0, "utf8stricmp");
    for (i = 0; PHYSFS_supportedArchiveTypes()[i]; i++)
        ;
    check(i >= 1, "archive types listed");
    printf("     %d archive types\n", i);

    check(PHYSFS_delete(file) != 0, "delete the file");
    check(PHYSFS_delete(dir) != 0, "delete the drawer");
    check(PHYSFS_deinit() != 0, "PHYSFS_deinit");
    check(!PHYSFS_isInit(), "deinitialised");

    printf("%s %d/%d\n", failures ? "FAIL" : "PASS", checks - failures, checks);
    return failures ? 5 : 0;
}
