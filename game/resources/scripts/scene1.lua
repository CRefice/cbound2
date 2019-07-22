require("scripts/Birb")
require("scripts/character")
require("scripts/coroutine")
require("scripts/ui")

birb = birb()
birb.position = Vec2:new(100, 10)

return {
	tile_map = "tiles/overworld.tmx";
	tile_set = "tiles/overworld.tsx";
	entities = {
		character,
		birb,
	}
}
