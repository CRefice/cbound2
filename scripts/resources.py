import glob
import os
import sys

import tiles

if len(sys.argv) < 2:
    sys.exit("Usage: resources.py [DIR]")

for file in glob.glob(os.path.join(sys.argv[1], "**", "*.tsx"), recursive=True):
    base, ext = os.path.splitext(file)
    outfile = base + "-set.lua"
    with open(outfile, "w+") as f:
        f.write("return {}".format(tiles.format_tsx(file)))
    os.remove(file)

for file in glob.glob(os.path.join(sys.argv[1], "**", "*.tmx"), recursive=True):
    base, ext = os.path.splitext(file)
    outfile = base + "-map.lua"
    with open(outfile, "w+") as f:
        f.write("return {}".format(tiles.format_tmx(file)))
    os.remove(file)
