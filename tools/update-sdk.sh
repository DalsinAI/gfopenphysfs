#!/bin/sh
# Writes sdk/: tools/gen.py's headers and .fd, the files a port builds
# against (sdk/include/physfs.h, proto/physfs.h, inline/physfs.h and
# sdk/fd/physfs_lib.fd). They are kept in the repository so the Open SDK's
# tools/gather.py can take them at a pinned commit, as it does every other
# library's. Run it after changing library/functions.txt or tools/gen.py;
# tests/host/run.sh and build-amiga.sh stop when sdk/ is out of date.
# zlib licence, Copyright (c) 2026 AmiCradle Team.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
python3 -I "$HERE/tools/gen.py" "$T"
mkdir -p "$HERE/sdk/include/inline" "$HERE/sdk/include/proto" "$HERE/sdk/fd"
for f in include/inline/physfs.h include/proto/physfs.h include/physfs.h fd/physfs_lib.fd; do
    cp "$T/$f" "$HERE/sdk/$f"
done
echo "sdk/ written"
