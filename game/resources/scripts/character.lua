box_size = 10

anims = {
	Back = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				value = IRect:new(0, 0, 17, 29)
			}, {
				duration = 0.2,
				value = IRect:new(17, 0, 34, 29)
			}, {
				duration = 0.2,
				value = IRect:new(34, 0, 51, 29)
			}, {
				duration = 0.2,
				value = IRect:new(51, 0, 68, 29)
			}
		}
	},
	Front = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				value = IRect:new(68, 0, 85, 29)
			}, {
				duration = 0.2,
				value = IRect:new(85, 0, 102, 29)
			}, {
				duration = 0.2,
				value = IRect:new(102, 0, 119, 29)
			}, {
				duration = 0.2,
				value = IRect:new(0, 29, 17, 58)
			}
		}
	},
	Left = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				value = IRect:new(17, 29, 34, 58)
			}, {
				duration = 0.2,
				value = IRect:new(34, 29, 51, 58)
			}, {
				duration = 0.2,
				value = IRect:new(51, 29, 68, 58)
			}, {
				duration = 0.2,
				value = IRect:new(68, 29, 85, 58)
			}
		}
	},
	Right = {
		mode = anim.play_mode.loop,
		frames = {
			{
				duration = 0.2,
				value = IRect:new(85, 29, 102, 58)
			}, {
				duration = 0.2,
				value = IRect:new(102, 29, 119, 58)
			}, {
				duration = 0.2,
				value = IRect:new(0, 58, 17, 87)
			}, {
				duration = 0.2,
				value = IRect:new(17, 58, 34, 87)
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
				world.remove(self.id)
			end,
			tag = "interact",
		},
		position = pos,
		behavior = function(self)
			coro.wait(0.1)
			world.remove(self.id)
		end,
	}
end

function directional_anim(vel)
	if vel.x > 0 then
		return anims.Right
	elseif vel.x < 0 then
		return anims.Left
	elseif vel.y > 0 then
		return anims.Back
	else
		return anims.Front
	end
end

local mov_speed = 50
local coll_bounds = Rect:new(0, 0, 15, 15)

character = {
	last_dir = Vec2:new(0, -1),
	anim_time = 0.0,

	sprite = {
		image = "textures/human.png",
		size = Vec2:new(17, 29),
		frame = IRect:new(68, 0, 85, 29)
	},
	collision = {
		bounds = coll_bounds
	},
	input = {
		["Action+"] = function(self)
			local pos = self.pos + coll_bounds:bottom_left()
			local dist = coll_bounds:size()
			local box_pos = pos + self.last_dir * dist
			world.instantiate(interact_box(box_pos))
		end,
		["XAxis"] = function(self, val)
			self:update_vel(Vec2:new(val * mov_speed, self.vel.y))
		end,
		["YAxis"] = function(self, val)
			self:update_vel(Vec2:new(self.vel.x, val * mov_speed))
		end,
	},
}

function character:update_vel(vel)
	self.vel = vel
	if self.animation then
		self.anim_time = self.animation:current_time()
		if vel == Vec2:new(0, 0) then
			self.animation:restart()
			self.animation:pause()
			return
		end
	end
	self.last_dir = vel:normalize()
	self.animation = directional_anim(self.last_dir)
	self.animation:skip_to(self.anim_time)
end
