horiz = function(val)
	bird.x = val
end

vert = function(val)
	bird.y = val
end

bird = {
	Sprite = {
		image = "textures/Birb.png",
		size = { 32, 32 },
		frame = { 0, 0, 32, 32 }
	},

	Animation = {
		fly = {
			mode = play_mode.loop,
			frames = {
				{
					duration = 0.1,
					coords = { 0, 0, 32, 32 }
				},
				{
					duration = 0.1,
					coords = { 32, 0, 64, 32 }
				},
			}
		}
	},

	x = 0.0,
	y = 0.0,

	Input = {
		["D,A"] = horiz,
		["W,S"] = vert,
	}
}
