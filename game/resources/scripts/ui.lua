function text_window(txt, pos, sz)
	local text = {
		ui = {
			kind = "text",
			text = txt,
			speed = 0.02,
			size = { sz.x - 10, sz.y - 10 }
		},
		position = { pos.x + 9, pos.y - 17 + sz.y - 10 },
	}
	local text_id = world.instantiate(text)
	local frame = {
		ui = {
			kind = "window",
			size = {sz.x, sz.y},
		},
		position = { pos.x, pos.y },
		input = {
			["Q+"] = function(self)
				text_ui = ui.text_widget(text_id)
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
