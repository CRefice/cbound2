require("scripts/Birb")
require("scripts/character")
require("scripts/dyn-camera")
require("scripts/ui")

birb.position = Vec2:new(100, 10)
dyn_camera.target = character

return {
	tile_map = "tiles/overworld.tmx";
	tile_set = "tiles/overworld.tsx";
	entities = {
		dyn_camera,
		character,
		birb,
	}
}
