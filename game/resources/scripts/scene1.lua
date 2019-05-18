return {
	tile_map = require("tilemaps/overworld-map");
	tile_set = require("tilesets/overworld-set");
	entities = {
		require("scripts/Birb"),
		require("scripts/Birb"),
		{
			ui = Text.new("This is\nreaaaaallly loong text that will definitely overflow!",
				0.02, Vec2.new(100, 100)),
			position = Vec2.new(27, 100)
		},
		{
			ui = Window.new(Vec2.new(100, 100)),
			position = Vec2.new(20, 20)
		},
	}
}
