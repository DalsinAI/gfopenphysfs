#!/usr/bin/env python3
"""The archives PhysFSBench reads (task 42): the same bytes every time.

    make_bench_data.py OUTDIR [--files N]

OUTDIR/bench.zip  N files (default 3000) in 60 drawers two deep, about 2 in 3
                  deflated and 1 in 3 stored, 1 KB to 24 KB each, plus two of
                  1 MB (one of each kind): a game's data archive in shape
OUTDIR/bench.pak  a Quake PAK of N/2 files (names of 55 bytes at most), the
                  same kinds of contents, stored as PAKs are
Contents are text-like (compress about 3:1), from a fixed seed.

zlib licence, Copyright (c) 2026 AmiCradle Team.
"""
import os
import random
import struct
import sys
import zipfile

WORDS = ("amiga cradle physfs library zip member inflate archive drawer path "
         "lookup hash entry seek read stored deflated level map sound sprite "
         "texture palette font music script data save config").split()


def content(rng, n):
    out = []
    size = 0
    while size < n:
        w = rng.choice(WORDS)
        if rng.random() < 0.15:
            w = "%s%d" % (w, rng.randrange(100000))
        out.append(w)
        size += len(w) + 1
    return (" ".join(out).encode() + b"\n")[:n]


def files(rng, n, maxname):
    entries = []
    for i in range(n):
        d1 = "set%02d" % (i % 12)
        d2 = "part%d" % ((i // 12) % 5)
        name = "%s/%s/file%05d_%s.dat" % (d1, d2, i, rng.choice(WORDS))
        entries.append((name[:maxname], content(rng, rng.randrange(1024, 24 * 1024))))
    entries.append(("big/stored.bin", content(rng, 1 << 20)))
    entries.append(("big/deflated.bin", content(rng, 1 << 20)))
    return entries


def main():
    args = sys.argv[1:]
    if not args:
        print(__doc__)
        return 2
    out = args[0]
    n = int(args[args.index("--files") + 1]) if "--files" in args else 3000
    os.makedirs(out, exist_ok=True)

    rng = random.Random(42)
    zpath = os.path.join(out, "bench.zip")
    with zipfile.ZipFile(zpath, "w") as z:
        for i, (name, data) in enumerate(files(rng, n, 200)):
            kind = zipfile.ZIP_STORED if (i % 3 == 0 or name == "big/stored.bin") else zipfile.ZIP_DEFLATED
            if name == "big/deflated.bin":
                kind = zipfile.ZIP_DEFLATED
            info = zipfile.ZipInfo(name, date_time=(2026, 11, 10, 12, 0, 0))
            info.compress_type = kind
            z.writestr(info, data, compresslevel=6 if kind == zipfile.ZIP_DEFLATED else None)

    rng = random.Random(43)
    entries = files(rng, n // 2, 55)
    ppath = os.path.join(out, "bench.pak")
    with open(ppath, "wb") as f:
        f.write(b"\0" * 12)
        dirs = []
        for name, data in entries:
            dirs.append((name, f.tell(), len(data)))
            f.write(data)
        diroff = f.tell()
        for name, off, size in dirs:
            f.write(struct.pack("<56sII", name.encode(), off, size))
        f.seek(0)
        f.write(b"PACK" + struct.pack("<II", diroff, len(dirs) * 64))
    for p in (zpath, ppath):
        print(p, os.path.getsize(p))
    return 0


if __name__ == "__main__":
    sys.exit(main())
