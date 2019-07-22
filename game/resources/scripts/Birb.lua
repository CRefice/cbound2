require("scripts/ui");

function birb()
	return {
		sprite = {
			image = "textures/Birb.png",
			size = Vec2:new(32, 32),
			frame = IRect:new(0, 0, 32, 32)
		},
		collision = {
			on_collision = function(self, tag)
				if tag == "interact" then
					text_window("The hell u want?", Vec2:new(20, 10), Vec2:new(100, 100))
				end
			end;
			bounds = Rect:new(0, 0, 32, 32)
		},
		animation = {
			mode = anim.play_mode.loop,
			frames = {
				{
					duration = 0.1,
					value = IRect:new(0, 0, 32, 32)
				},
				{
					duration = 0.1,
					value = IRect:new(32, 0, 64, 32)
				},
			}
		}
	}
end
