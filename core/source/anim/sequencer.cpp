#include <algorithm>
#include <cstdint>

#include "anim/sequencer.hpp"

namespace anim {
void Sequencer::progress(double dt) {
	time -= dt;
	if (time <= 0.0) {
		if (is_last_frame()) {
			switch (seq->mode) {
				case PlayMode::Once:
					direction = 0;
					break;
				case PlayMode::Loop:
					restart();
					return;
				case PlayMode::PingPong:
					direction = -direction;
					break;
			}
		}
		cur_frame += direction;
		time = cur_frame->duration;
	}
}

bool Sequencer::is_last_frame() const {
	return cur_frame == (seq->frames.end() - 1)
		 || (cur_frame == seq->frames.begin() && direction < 0);
}
} // namespace anim
