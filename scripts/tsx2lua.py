from collections import deque
import os
import os.path as path
import sys

import xml.etree.ElementTree as ET

if len(sys.argv) < 3:
    sys.exit('Usage: tsx2lua.py INPUT OUTPUT')

def split_until(name, path):
    path = os.path.normpath(path)
    queue = deque(path.split(os.sep))
    while queue:
        folder = queue.popleft()
        if (folder == name):
            break
    return os.sep.join(queue)


def formatAnim(tile):
    anim = tile.find('animation')
    return ",\n\t\t\t".join("{{\n\t\t\t\tduration = {},\n\t\t\t\tid = {}\n\t\t\t}}".format(
        float(f.attrib['duration'])/1000, f.attrib['tileid']) for f in anim.findall('frame'))

tsx = ET.parse(sys.argv[1])
root = tsx.getroot()
attribs = root.attrib
width=int(attribs['columns'])
height=int(attribs['tilecount']) / width
tilewidth=attribs['tilewidth']
tileheight=attribs['tileheight']
image=root.find('image').attrib['source']
anims = [tile for tile in root.findall('tile') if tile.find('animation')]
with open(sys.argv[2], 'w') as f:
    f.write("return {\n")
    f.write("\tsize = {{{},{}}},\n".format(int(width), int(height)))
    f.write("\ttile_size = {{{},{}}},\n".format(tilewidth, tileheight))
    f.write("\timage = \"{}\",\n".format(split_until('resources', image)))
    f.write("\ttiles = {{\n\t\t{}\n\t}}\n".format(',\n\t\t'.join("{{\n\t\t\tid = {}, frames = {{\n{}\n}}\n\t\t}}".format(tile.attrib['id'], formatAnim(tile)) for tile in anims)))
    f.write("}\n")

