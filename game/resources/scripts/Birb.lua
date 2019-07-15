speed = 60
require("scripts/ui");

function interact_box(pos)
	return {
		sprite = {
			image = "textures/Cazz.png",
			size = Vec2:new(10, 10),
			frame = IRect:new(0, 0, 10, 10)
		},
		collision = {
			bounds = Rect:new(0, 0, 10, 10),
			solid = false,
			on_collision = function(self, other)
				world.remove(self)
			end,
			tag = "interact",
		},
		position = pos,
		behavior = function(self)
			coro.wait(0.1)
			world.remove(self)
		end,
}
end

function birb()
	last_dir = Vec2:new(0, -1)
	return {
		sprite = {
			image = "textures/Birb.png",
			size = Vec2:new(32, 32),
			frame = IRect:new(0, 0, 32, 32)
		},
		collision = {
			bounds = Rect:new(0, 0, 32, 32)
		},
		anim = {
			mode = anim.play_mode.loop,
			frames = {
				{
					duration = 0.1,
					coords = IRect:new(0, 0, 32, 32)
				},
				{
					duration = 0.1,
					coords = IRect:new(32, 0, 64, 32)
				},
			}
		},
		input = {
			["Q+"] = function(self)
				local box_pos = self.pos + Vec2:new(11, 11) + last_dir * 21
				world.instantiate(interact_box(box_pos))
			end,
			["D+"] = function(self)
				last_dir = Vec2:new(1, 0)
				self.vel = self.vel + Vec2:new(speed, 0)
			end,
			["D-"] = function(self)
				self.vel = self.vel + Vec2:new(-speed, 0)
			end,
			["A+"] = function(self)
				last_dir = Vec2:new(-1, 0)
				self.vel = self.vel + Vec2:new(-speed, 0)
			end,
			["A-"] = function(self)
				self.vel = self.vel + Vec2:new(speed, 0)
			end,
			["W+"] = function(self)
				last_dir = Vec2:new(0, 1)
				self.vel = self.vel + Vec2:new(0, speed)
			end,
			["W-"] = function(self)
				self.vel = self.vel + Vec2:new(0, -speed)
			end,
			["S+"] = function(self)
				last_dir = Vec2:new(0, -1)
				self.vel = self.vel + Vec2:new(0, -speed)
			end,
			["S-"] = function(self)
				self.vel = self.vel + Vec2:new(0, speed)
			end,
		},
	}
end
