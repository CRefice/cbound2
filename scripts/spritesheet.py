import json
from sys import argv

import lua


def irect_call(frame):
    x1 = frame["x"]
    y1 = frame["y"]
    x2 = x1 + frame["w"]
    y2 = y1 + frame["h"]
    return lua.call("IRect:new", x1, y1, x2, y2)


file = open(argv[1], "r")
contents = json.load(file)
bounds = [
    (tag["name"], tag["from"], tag["to"]) for tag in contents["meta"]["frameTags"]
]

frames = contents["frames"]
anims = {
    name: {
        "mode": lua.literal("play_mode.loop"),
        "frames": [
            {"duration": frame["duration"] / 1000, "coords": irect_call(frame["frame"])}
            for frame in frames[begin:end + 1]
        ],
    }
    for (name, begin, end) in bounds
}
file = open(argv[2], "w")
file.write("return {}".format(lua.format(anims)))
