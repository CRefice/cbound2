return {
	Sprite = {
		image = "textures/Birb.png",
		size = { 32, 32 },
		frame = { 0, 0, 32, 32 }
	},
	Anim = {
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
	},
	Input = {
		["D+"] = function(self)
			self.pos.x = self.pos.x + 10
		end,
		["A+"] = function(self)
			self.pos.x = self.pos.x - 10
		end,
		["W+"] = function(self)
			self.pos.y = self.pos.y + 10
		end,
		["S+"] = function(self)
			self.pos.y = self.pos.y - 10
		end,
	}
}
