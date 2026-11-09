#!/bin/sh
# Host check: PhysFSTest against PhysicsFS itself (physfs/, unpatched, on
# its Unix platform), and the generator's output. Needs cc and python3.
# zlib licence, Copyright (c) 2026 AmiCradle Team.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
B=${B:-$HERE/build/host}
rm -rf "$B"; mkdir -p "$B/w"
CC=${CC:-cc}
SRC=$(ls "$HERE"/physfs/*.c)
"$CC" -std=gnu99 -O1 -g -Wall -Wno-unused -I"$HERE/physfs" -I"$HERE/tests" \
    -o "$B/PhysFSTest" $SRC "$HERE/tests/physfs_test.c" -lpthread
"$B/PhysFSTest" "$B/w" host
# The same checks with the zip reader as physfs.library builds it (patch
# 0002): members inflate through zlib's calls, here the system's zlib, where
# the library has z.library; PhysicsFS's miniz is left out.
mkdir -p "$B/z/src" "$B/z/w"
cp "$HERE"/physfs/*.c "$HERE"/physfs/*.h "$HERE/amiga/physfs_zlibrary.h" "$B/z/src/"
patch -s -d "$B/z/src" -p1 < "$HERE/patches/0002-zip-inflate-from-zlibrary.patch"
"$CC" -std=gnu99 -O1 -g -Wall -Wno-unused -DPHYSFS_ZLIBRARY_HOSTED -I"$B/z/src" -I"$HERE/tests" \
    -o "$B/PhysFSTestZlib" $(ls "$B"/z/src/*.c) "$HERE/tests/physfs_test.c" -lz -lpthread
"$B/PhysFSTestZlib" "$B/z/w" host
python3 -I "$HERE/tools/gen.py" "$B/gen" >/dev/null
for f in physfs_stubs.c physfs_vectors.h include/inline/physfs.h include/proto/physfs.h include/physfs.h fd/physfs_lib.fd; do
    test -s "$B/gen/$f" || { echo "gen.py: $f missing"; exit 1; }
done
n=$(grep -c '^PHYSFS_' "$B/gen/fd/physfs_lib.fd")
t=$(grep -c '^PHYSFS_' "$HERE/library/functions.txt")
[ "$n" = "$t" ] || { echo "gen.py: $n calls in the .fd, $t in functions.txt"; exit 1; }
v=$(grep -c '^PHYSFS_VECTOR(' "$B/gen/physfs_vectors.h")
[ "$v" = "$t" ] || { echo "gen.py: $v vectors, $t in functions.txt"; exit 1; }
# sdk/: the SDK's copies of gen.py's headers and .fd, kept in the repository
# so that the Open SDK's tools/gather.py can take them at a pinned commit.
for f in include/inline/physfs.h include/proto/physfs.h include/physfs.h fd/physfs_lib.fd; do
    cmp -s "$B/gen/$f" "$HERE/sdk/$f" || { echo "sdk/$f is not what gen.py makes: run tools/update-sdk.sh"; exit 1; }
done
echo "host: PASS (PhysFSTest with miniz and with zlib, gen.py made $n calls, sdk/ matches it)"
