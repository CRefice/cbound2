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

function change_sel(window, new_sel)
	local selected = window.options[new_sel]
	local arrow = window.arrow
	if not selected then return end
	window.selection = new_sel
	arrow.position = Vec2:new(arrow.position.x, selected.position.y)
end

function option_window(options, pos, sz)
	arrow = {
		sprite = {
			image = "textures/gui-elements.png",
			size = Vec2:new(5, 7),
			frame = IRect:new(27, 9, 32, 16)
		},
		position = pos + Vec2:new(0, sz.y - 19)
	}
	world.instantiate(arrow)
	local opt_pos = Vec2:new(pos.x, pos.y)
	local opts = {}
	for _, opt in ipairs(options) do
		local text = {
			ui = ui.Text.new(opt, sz - Vec2:new(32, 32)),
			position = opt_pos + Vec2:new(20, sz.y - 19),
		}
		world.instantiate(text)
		table.insert(opts, text)
		opt_pos.y = opt_pos.y - 20
	end
	return {
		ui = ui.Window.new(sz),
		arrow = arrow,
		selection = 1,
		options = opts,
		position = pos,
		input = {
			["Action"] = rising(function (self)
				for _, opt in ipairs(self.options) do
					world.remove(opt)
				end
				world.remove(self.arrow)
				world.remove(self)
			end),
			["YAxis"] = function (self, val)
				if val >= 0.5 then
					change_sel(self, self.selection - 1)
				elseif val <= -0.5 then
					change_sel(self, self.selection + 1)
				end
			end
	}
}
end
