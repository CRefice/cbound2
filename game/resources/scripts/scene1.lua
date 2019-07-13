require("scripts/Birb")
require("scripts/coroutine")
require("scripts/ui")

birb1 = birb()
birb2 = birb()

print(birb1.sprite.size)

birb2.position = Vec2:new(100, 10)
birb2.input = {}
birb2.collision.on_collision = function (self, tag)
	if tag == "interact" then
		text_window("The hell u want?", Vec2:new(20, 10), Vec2:new(100, 100))
	end
end

return {
	tile_map = "tiles/overworld.tmx";
	tile_set = "tiles/overworld.tsx";
	entities = {
		birb1,
		birb2,
	}
}
