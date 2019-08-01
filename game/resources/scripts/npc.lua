require("scripts/ui");

function npc(desc)
	return {
		sprite = desc.sprite,
		collision = {
			on_collision = function(self, tag)
				if tag == "interact" then
					text_window(desc.dialog, Vec2:new(20, 10), Vec2:new(140, 100))
				end
			end;
			bounds = Rect:new(0, 0, 32, 32)
		},
		animation = desc.animation
	}
end
