return {
	tile_map = require("tilemaps/overworld-map");
	tile_set = require("tilesets/overworld-set");
	entities = {
		require("scripts/Birb"),
		require("scripts/Birb"),
		{
			ui = Text.new("This is {color:#000000}colored {color}text!",
				0.02, Vec2.new(1000, 20)),
			position = Vec2.new(50, 100)
		}
	}
}
