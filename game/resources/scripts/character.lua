box_size = 10
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
	local mov_speed = 60
	local coll_bounds = Rect:new(0, 0, 15, 15)

	return {
		sprite = {
			image = "textures/human.png",
			size = Vec2:new(15, 31),
			frame = IRect:new(2, 0, 17, 31)
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
				self.vel = self.vel + Vec2:new(mov_speed, 0)
			end,
			["D-"] = function(self)
				self.vel = self.vel + Vec2:new(-mov_speed, 0)
			end,
			["A+"] = function(self)
				last_dir = Vec2:new(-1, 0)
				self.vel = self.vel + Vec2:new(-mov_speed, 0)
			end,
			["A-"] = function(self)
				self.vel = self.vel + Vec2:new(mov_speed, 0)
			end,
			["W+"] = function(self)
				last_dir = Vec2:new(0, 1)
				self.vel = self.vel + Vec2:new(0, mov_speed)
			end,
			["W-"] = function(self)
				self.vel = self.vel + Vec2:new(0, -mov_speed)
			end,
			["S+"] = function(self)
				last_dir = Vec2:new(0, -1)
				self.vel = self.vel + Vec2:new(0, -mov_speed)
			end,
			["S-"] = function(self)
				self.vel = self.vel + Vec2:new(0, mov_speed)
			end,
		},
	}
end
