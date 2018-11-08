import sys

import xml.etree.ElementTree as ET

def layerStr(layer):
    ids = [str(id.number) for id in layer.data]
    return "{{\n{}\n}}\n".format(','.join(ids))

if len(sys.argv) < 3:
    sys.exit('Usage: tiled2lua.py INPUT OUTPUT')

tmx = ET.parse(sys.argv[1])
root = tmx.getroot()
attribs = root.attrib
width=attribs['width']
height=attribs['height']
tilewidth=attribs['tilewidth']
tileheight=attribs['tileheight']
layers = [layer.find('data').text.replace('\n','') for layer in root.findall('layer')]
with open(sys.argv[2], 'w') as f:
    f.write("return {\n")
    f.write("\tsize = {{{},{}}},\n".format(width, height))
    f.write("\ttile_size = {{{},{}}},\n".format(tilewidth, tileheight))
    f.write("\tlayers = {{\n\t\t{}\n}}".format(',\n\t\t'.join("{{{}}}".format(l) for l in layers)))
    f.write("}\n")

