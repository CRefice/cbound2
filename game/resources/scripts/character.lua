box_size = 10

anims = {
	Back = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				coords = IRect:new(0, 0, 17, 29)
			}, {
				duration = 0.2,
				coords = IRect:new(17, 0, 34, 29)
			}, {
				duration = 0.2,
				coords = IRect:new(34, 0, 51, 29)
			}, {
				duration = 0.2,
				coords = IRect:new(51, 0, 68, 29)
			}
		}
	},
	Front = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				coords = IRect:new(68, 0, 85, 29)
			}, {
				duration = 0.2,
				coords = IRect:new(85, 0, 102, 29)
			}, {
				duration = 0.2,
				coords = IRect:new(102, 0, 119, 29)
			}, {
				duration = 0.2,
				coords = IRect:new(0, 29, 17, 58)
			}
		}
	},
	Left = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				coords = IRect:new(17, 29, 34, 58)
			}, {
				duration = 0.2,
				coords = IRect:new(34, 29, 51, 58)
			}, {
				duration = 0.2,
				coords = IRect:new(51, 29, 68, 58)
			}, {
				duration = 0.2,
				coords = IRect:new(68, 29, 85, 58)
			}
		}
	},
	Right = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				coords = IRect:new(85, 29, 102, 58)
			}, {
				duration = 0.2,
				coords = IRect:new(102, 29, 119, 58)
			}, {
				duration = 0.2,
				coords = IRect:new(0, 58, 17, 87)
			}, {
				duration = 0.2,
				coords = IRect:new(17, 58, 34, 87)
			}
		}
	}
}

function interact_box(pos)
	return {
		sprite = {
			image = "textures/Cazz.png",
			size = Vec2:new(box_size, box_size),
			frame = IRect:new(0, 0, box_size, box_size)
		},
		collision = {
			bounds = Rect:new(0, 0, box_size, box_size),
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

function character()
	local last_dir = Vec2:new(0, -1)
	local mov_speed = 50
	local coll_bounds = Rect:new(0, 0, 15, 15)

	return {
		sprite = {
			image = "textures/human.png",
			size = Vec2:new(17, 29),
			frame = IRect:new(68, 0, 85, 29)
		},
		collision = {
			bounds = coll_bounds
		},
		input = {
			["Q+"] = function(self)
				local pos = self.pos + coll_bounds:bottom_left()
				local dist = coll_bounds:size()
				local box_pos = pos + last_dir * dist
				world.instantiate(interact_box(box_pos))
			end,
			["D+"] = function(self)
				last_dir = Vec2:new(1, 0)
				anim.switch_to(self, anims.Right)
				self.vel = self.vel + Vec2:new(mov_speed, 0)
			end,
			["D-"] = function(self)
				self.vel = self.vel + Vec2:new(-mov_speed, 0)
			end,
			["A+"] = function(self)
				last_dir = Vec2:new(-1, 0)
				anim.switch_to(self, anims.Left)
				self.vel = self.vel + Vec2:new(-mov_speed, 0)
			end,
			["A-"] = function(self)
				self.vel = self.vel + Vec2:new(mov_speed, 0)
			end,
			["W+"] = function(self)
				last_dir = Vec2:new(0, 1)
				anim.switch_to(self, anims.Back)
				self.vel = self.vel + Vec2:new(0, mov_speed)
			end,
			["W-"] = function(self)
				self.vel = self.vel + Vec2:new(0, -mov_speed)
			end,
			["S+"] = function(self)
				last_dir = Vec2:new(0, -1)
				anim.switch_to(self, anims.Front)
				self.vel = self.vel + Vec2:new(0, -mov_speed)
			end,
			["S-"] = function(self)
				self.vel = self.vel + Vec2:new(0, mov_speed)
			end,
		},
	}
end
