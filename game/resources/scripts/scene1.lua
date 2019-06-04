require("scripts/Birb")
require("scripts/coroutine")

birb1 = birb()
birb2 = birb()
birb2.input = {
	["E+"] = function(self)
			self.vel = Vec2:new(10, 0)
			print(self)
			coro.wait(1.0)
			self.vel = Vec2:new(-10, 0)
			print(self)
	end,
}

return {
	tile_map = require("tiles/overworld-map");
	tile_set = require("tiles/overworld-set");
	entities = {
		birb1,
		birb2
	}
}
