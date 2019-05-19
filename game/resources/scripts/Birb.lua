speed = 100

return {
	sprite = {
		image = "textures/Birb.png",
		size = { 32, 32 },
		frame = { 0, 0, 32, 32 }
	},
	anim = {
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
	input = {
		["D+"] = function(self)
			self.vel = self.vel + Vec2:new(speed, 0)
		end,
		["D-"] = function(self)
			self.vel = self.vel + Vec2:new(-speed, 0)
		end,
		["A+"] = function(self)
			self.vel = self.vel + Vec2:new(-speed, 0)
		end,
		["A-"] = function(self)
			self.vel = self.vel + Vec2:new(speed, 0)
		end,
		["W+"] = function(self)
			self.vel = self.vel + Vec2:new(0, speed)
		end,
		["W-"] = function(self)
			self.vel = self.vel + Vec2:new(0, -speed)
		end,
		["S+"] = function(self)
			self.vel = self.vel + Vec2:new(0, -speed)
		end,
		["S-"] = function(self)
			self.vel = self.vel + Vec2:new(0, speed)
		end,
	},
}
