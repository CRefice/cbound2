require("scripts/Birb")
require("scripts/coroutine")

birb1 = birb()
birb2 = birb()

birb1.collision.on_collision = function(self, other)
end

birb2.position = { 100, 10 }
birb2.input = {
	["E+"] = function(self)
			self.vel = Vec2:new(10, 0)
			coro.wait(1.0)
			self.vel = Vec2:new(-10, 0)
	end,
}

return {
	tile_map = "tiles/overworld.tmx";
	tile_set = "tiles/overworld.tsx";
	entities = {
		birb1,
		birb2,
	}
}
