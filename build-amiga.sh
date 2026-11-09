#!/bin/sh
# physfs.library for AmigaOS 3.x (68040), with a GCC 16 stove, for Cradle's
# stove_build (target .) or by hand:
#   STOVE  the stove (default ~/AmiCradle/stoves/os32-gcc16); its compiler is
#          STOVE/prefix/bin/m68k-amigaos-gcc or STOVE/bin/m68k-amigaos-gcc
#   OUT    the output directory (default build/os3)
#   CPU    -m68040 (default) or -m68060
#   NDK    the OS headers' Include_H, when the stove has none of its own
#          (found under ~/CradleLab/gfopensdk/OpenNDK when there)
# Outputs:
#   OUT/Libs/physfs.library     PhysicsFS 3.2.0, all ten archivers, one copy
#                               of its state per program (physfs_libinit.c)
#   OUT/sdk/include/            physfs.h, proto/physfs.h, inline/physfs.h:
#                               PhysicsFS's header with the calls through
#                               PhysFSBase
#   OUT/sdk/lib/libphysfs_auto.a  opens physfs.library before main()
#   OUT/sdk/lib/libphysfs.a     PhysicsFS linked in instead (static), with
#   OUT/sdk/include-static/physfs.h  its own header
#   OUT/sdk/fd/physfs_lib.fd
#   OUT/tests/PhysFSTest        the checks through physfs.library
#   OUT/tests/PhysFSTestStatic  the same checks with libphysfs.a
# zlib licence, Copyright (c) 2026 AmiCradle Team.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
OUT=${OUT:-$HERE/build/os3}
STOVE=${STOVE:-$HOME/AmiCradle/stoves/os32-gcc16}
CPU=${CPU:--m68040}
NDKFLAG=""
if ! printf '#include <exec/types.h>\n' | "${STOVE}/prefix/bin/m68k-amigaos-gcc" -E -x c - >/dev/null 2>&1 \
   && ! printf '#include <exec/types.h>\n' | "${STOVE}/bin/m68k-amigaos-gcc" -E -x c - >/dev/null 2>&1; then
    NDK=${NDK:-$HOME/CradleLab/gfopensdk/OpenNDK/Include_H}
    [ -f "$NDK/exec/types.h" ] || { echo "the stove has no OS headers: set NDK to an Include_H"; exit 2; }
    NDKFLAG="-I$NDK"
fi
if [ -x "$STOVE/prefix/bin/m68k-amigaos-gcc" ]; then BIN=$STOVE/prefix/bin; else BIN=$STOVE/bin; fi
CC=$BIN/m68k-amigaos-gcc
AR=$BIN/m68k-amigaos-ar
W=$HERE/build/work
"$CC" --version | head -1

rm -rf "$W" "$OUT"
mkdir -p "$W/src" "$W/lib" "$W/static" "$W/stub" "$OUT/Libs" "$OUT/sdk/lib" "$OUT/sdk/include-static" "$OUT/tests"

# PhysicsFS as released, with the Team's AmigaOS platform and its header patch.
for f in "$HERE"/physfs/*.c "$HERE"/physfs/*.h; do
    case "$f" in *physfs_platform_posix.c|*physfs_platform_unix.c) ;; *) cp "$f" "$W/src/" ;; esac
done
cp "$HERE/amiga/physfs_platform_amiga.c" "$W/src/"
for p in "$HERE"/patches/*.patch; do patch -s -d "$W/src" -p1 < "$p"; done
cp "$W/src/physfs.h" "$OUT/sdk/include-static/physfs.h"
SRC=$(ls "$W"/src/*.c)

python3 -I "$HERE/tools/gen.py" "$W/gen"
cp -R "$W/gen/include" "$OUT/sdk/include"
cp -R "$W/gen/fd" "$OUT/sdk/fd"

COMMON="-noixemul $CPU $NDKFLAG -O2 -fomit-frame-pointer -fno-tree-loop-distribute-patterns -fno-common -std=gnu99 -DNDEBUG -D__amigaos__ -Wall -Wno-unused"

# physfs.library: PhysicsFS and the call entries a4-relative, so that
# physfs_libinit.c (the RomTag and the library's own Open/Close/Expunge,
# built absolute and linked first) can give each opener its own copy of the
# data. -resident has ld add the table of the data's pointers into itself
# (__datadata_relocs), which each copy is relocated by.
for f in $SRC "$HERE/library/physfs_lib.c" "$W/gen/physfs_stubs.c"; do
    "$CC" $COMMON -fbaserel -I"$W/src" -I"$HERE/library" -c -o "$W/lib/$(basename "$f" .c).o" "$f"
done
"$CC" $COMMON -fno-toplevel-reorder -I"$W/gen" -I"$HERE/library" -c -o "$W/physfs_libinit.o" "$HERE/library/physfs_libinit.c"
# strip refuses the a4-relative data section, so the link strips (-s).
"$CC" -noixemul $CPU -resident -nostartfiles -s -o "$OUT/Libs/physfs.library" "$W/physfs_libinit.o" "$W"/lib/*.o -lgcc
python3 -I "$HERE/tools/checklib.py" "$OUT/Libs/physfs.library"

# libphysfs.a: the same PhysicsFS, linked into a program instead.
for f in $SRC; do
    "$CC" $COMMON -I"$W/src" -c -o "$W/static/$(basename "$f" .c).o" "$f"
done
"$AR" rcs "$OUT/sdk/lib/libphysfs.a" "$W"/static/*.o

# libphysfs_auto.a: opens the library for a program.
"$CC" -noixemul $CPU $NDKFLAG -O2 -Wall -c -o "$W/stub/physfs_auto.o" "$HERE/stub/physfs_auto.c"
"$AR" rcs "$OUT/sdk/lib/libphysfs_auto.a" "$W/stub/physfs_auto.o"

# The checks: through the library, and linked in.
"$CC" -noixemul $CPU $NDKFLAG -O2 -Wall -I"$OUT/sdk/include" -I"$HERE/tests" -o "$OUT/tests/PhysFSTest" \
    "$HERE/tests/physfs_test.c" -L"$OUT/sdk/lib" -lphysfs_auto
"$CC" -noixemul $CPU $NDKFLAG -O2 -Wall -I"$OUT/sdk/include-static" -I"$HERE/tests" -o "$OUT/tests/PhysFSTestStatic" \
    "$HERE/tests/physfs_test.c" "$OUT/sdk/lib/libphysfs.a"

for f in "$OUT/Libs/physfs.library" "$OUT/tests/PhysFSTest" "$OUT/tests/PhysFSTestStatic" \
         "$OUT/sdk/lib/libphysfs.a" "$OUT/sdk/lib/libphysfs_auto.a"; do
    echo "$(basename "$f") $(wc -c < "$f") bytes $(sha256sum "$f" | cut -c1-16)"
done
