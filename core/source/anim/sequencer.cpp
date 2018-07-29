#include <cstdint>

#include "anim/sequencer.hpp"

namespace anim {
void Sequencer::progress(double dt) {
	time += multiplier * dt;
	switch (seq->mode) {
		case PlayMode::PlayOnce:
			multiplier = 0;
			break;
		case PlayMode::Repeat:
			time = 0.0;
			break;
		case PlayMode::PingPong:
			multiplier = -multiplier;
			break;
	}
}

Frame Sequencer::current_frame() const {
	if (seq->frame_time_ms == 0.0) {
		return seq->frames.front();
	}
	std::size_t index = time / seq->frame_time_ms;
	return seq->frames[index];
}
} // namespace anim
