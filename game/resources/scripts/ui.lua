function text_window(txt, pos, sz)
	local text = {
		ui = {
			kind = "text",
			text = txt,
			speed = 0.02,
			size = sz - Vec2:new(10, 10)
		},
		position = pos + Vec2:new(9, sz.y - 19)
	}
	local text_id = world.instantiate(text)
	local frame = {
		ui = {
			kind = "window",
			size = sz
		},
		position = pos,
		input = {
			["Action+"] = function(self)
				text_ui = ui.text_widget(text_id)
				if (text_ui:done()) then
					world.remove(self.id)
					world.remove(text_id)
				else
					text_ui:skip()
				end
			end
		}
	}
	world.instantiate(frame)
end
