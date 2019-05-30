import os
import sys
import xml.etree.ElementTree as ET
from collections import deque

import lua


def split_until(name, path):
    path = os.path.normpath(path)
    queue = deque(path.split(os.sep))
    while queue:
        folder = queue.popleft()
        if folder == name:
            break
    return os.sep.join(queue)


def resource_path(file, resource):
    dir = os.path.dirname(file)
    resource = os.path.join(dir, resource)
    absolute = os.path.abspath(resource)
    return split_until("resources", absolute)


def format_tmx(file):
    tmx = ET.parse(file)
    root = tmx.getroot()
    attribs = root.attrib
    table = {}
    table["size"] = [int(attribs["width"]), int(attribs["height"])]
    table["tile_size"] = [int(attribs["tilewidth"]), int(attribs["tileheight"])]
    table["layers"] = [
        {"tiles": list(map(int, layer.find("data").text.replace("\n", "").split(",")))}
        for layer in root.findall("layer")
    ]
    return lua.format(table)


def parse_anim(tile):
    anim = tile.find("animation")
    return [
        {"duration": float(f.attrib["duration"]) / 1000, "id": int(f.attrib["tileid"])}
        for f in anim.findall("frame")
    ]


def format_tsx(file):
    tsx = ET.parse(file)
    root = tsx.getroot()
    attribs = root.attrib
    table = {}
    width = int(attribs["columns"])
    height = int(attribs["tilecount"]) // width
    table["size"] = [width, height]
    table["tile_size"] = [int(attribs["tilewidth"]), int(attribs["tileheight"])]
    image = root.find("image").attrib["source"]
    table["image"] = resource_path(file, image)
    table["tiles"] = [
        {"id": int(tile.attrib["id"]), "frames": parse_anim(tile)}
        for tile in root.findall("tile")
        if tile.find("animation")
    ]
    return lua.format(table)
