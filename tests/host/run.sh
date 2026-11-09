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
python3 -I "$HERE/tools/gen.py" "$B/gen" >/dev/null
for f in physfs_stubs.c physfs_vectors.h include/inline/physfs.h include/proto/physfs.h include/physfs.h fd/physfs_lib.fd; do
    test -s "$B/gen/$f" || { echo "gen.py: $f missing"; exit 1; }
done
n=$(grep -c '^PHYSFS_' "$B/gen/fd/physfs_lib.fd")
t=$(grep -c '^PHYSFS_' "$HERE/library/functions.txt")
[ "$n" = "$t" ] || { echo "gen.py: $n calls in the .fd, $t in functions.txt"; exit 1; }
v=$(grep -c '^PHYSFS_VECTOR(' "$B/gen/physfs_vectors.h")
[ "$v" = "$t" ] || { echo "gen.py: $v vectors, $t in functions.txt"; exit 1; }
echo "host: PASS (PhysFSTest, and gen.py made $n calls)"
