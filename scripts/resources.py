import os
import glob
import sys
from tiles import convert_tmx, convert_tsx

if (len(sys.argv) < 2):
    sys.exit("Usage: resources.py [DIR]")

for file in glob.glob(os.path.join(sys.argv[1], "**", "*.tsx"), recursive=True):
    base, ext = os.path.splitext(file)
    convert_tsx(file, base + '-set.lua')
    os.remove(file)

for file in glob.glob(os.path.join(sys.argv[1], "**", "*.tmx"), recursive=True):
    base, ext = os.path.splitext(file)
    convert_tmx(file, base + '-map.lua')
    os.remove(file)
