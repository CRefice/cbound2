require("scripts/npc");

birb = npc{
	dialog = "I'm a bird! Chirp chirp! THis is a long as text",
	sprite = {
		image = "textures/Birb.png",
		size = Vec2:new(32, 32),
		frame = IRect:new(0, 0, 32, 32)
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
