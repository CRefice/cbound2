function lerp(a, b, x)
	return a + (b - a) * x
end

local convergence_rate = 0.02
dyn_camera = {
	offset = Vec2:new(0, 0),
	update = function(self, dt)
		if self.target then
			local target_pos = self.target.pos - render.camera.resolution / 2
			self.pos = lerp(self.pos, target_pos, convergence_rate)
			self.pos.x = math.floor(self.pos.x)
			self.pos.y = math.floor(self.pos.y)
			render.camera.move_to(self.pos)
		end
	end
}
