function text_window(text, position, size)
	local text_ui = ui.Text.new(text, 0.02, size - Vec2:new(10, 10))
	local text = {
		ui = text_ui,
		position = position + Vec2:new(9, size.y - 17),
	}
	local text_id = world.instantiate(text)
	local frame = {
		ui = ui.Window.new(size),
		position = position,
		input = {
			["Q+"] = function(self)
				if (text_ui:done()) then
					world.remove(self)
					world.remove(text_id)
				else
					text_ui:skip()
				end
			end
		}
	}
	world.instantiate(frame)
end
