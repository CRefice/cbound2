require("scripts/functional")

function text_window(txt, pos, sz)
	local text = {
		ui = ui.Text.new(txt, 0.02, sz - Vec2:new(32, 32)),
		position = pos + Vec2:new(9, sz.y - 19),
	}
	local frame = {
		ui = ui.Window.new(sz),
		position = pos,
		input = {
			["Action"] = rising(function(self)
				if (text.ui:done()) then
					world.remove(self)
					world.remove(text)
				else
					text.ui:skip()
				end
			end)
		}
	}
	world.instantiate(text)
	world.instantiate(frame)
end
