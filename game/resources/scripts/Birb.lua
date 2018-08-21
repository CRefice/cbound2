bird = {
	Sprite = {
		image = "textures/Birb.png",
		size = { 32, 32 },
		frame = { 0, 0, 32, 32 }
	},
	Animation = {
		fly = {
			frame_time = 0.1,
			mode = play_mode.loop,
			frames = {
				{ 0, 0, 32, 32 },
				{ 32, 0, 64, 32 }
			}
		}
	}
}
