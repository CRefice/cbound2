#include <algorithm>
#include <cstdint>

#include "anim/sequencer.hpp"

namespace anim {
void Sequencer::progress(double dt) {
	time += multiplier * dt;
	if (time <= 0.0 || time >= duration(*seq)) {
		switch (seq->mode) {
			case PlayMode::Once:
				multiplier = 0;
				break;
			case PlayMode::Loop:
				time = 0.0;
				break;
			case PlayMode::PingPong:
				multiplier = -multiplier;
				break;
		}
	}
}

Frame Sequencer::current_frame() const {
	if (seq->frame_time == 0.0)
		return seq->frames.front();
	if (time >= duration(*seq))
		return seq->frames.back();

	std::size_t index = time / seq->frame_time;
	return seq->frames[index];
}
} // namespace anim
