dyn_camera = {
	offset = Vec2:new(0, 0),
	bounds = Rect:new(0, 0, 400, 224),
	update = function(self, dt)
		if self.target then
			local viewport = Rect:new(Vec2:new(0, 0), render.camera.resolution)
			viewport = viewport:translate(self.target.position - render.camera.resolution / 2)
			if viewport:left() < self.bounds:left() then
				viewport = viewport:translate(Vec2:new(self.bounds:left() - viewport:left(), 0))
			end
			if viewport:bottom() < self.bounds:bottom() then
				viewport = viewport:translate(Vec2:new(0, self.bounds:bottom() - viewport:bottom()))
			end
			if viewport:right() > self.bounds:right() then
				viewport = viewport:translate(Vec2:new(self.bounds:right() - viewport:right(), 0))
			end
			if viewport:top() > self.bounds:top() then
				viewport = viewport:translate(Vec2:new(0, self.bounds:top() - viewport:top()))
			end
			self.position = viewport:bottom_left()
			render.camera.move_to(self.position)
		end
	end
}
