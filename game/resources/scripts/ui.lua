function text_window(text, position, size)
	local text = {
		ui = Text.new(text, 0.02, size),
		position = position + Vec2:new(9, size.y - 17),
	}
	local text_id = world.instantiate(text)
	local frame = {
		ui = Window.new(size),
		position = position,
		input = {
			["Q+"] = function(self)
				world.remove(self)
				world.remove(text_id)
			end
		}
	}
	world.instantiate(frame)
end
