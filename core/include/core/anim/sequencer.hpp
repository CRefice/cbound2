#pragma once

#include "sequence.hpp"

namespace anim {
// Controls over an animation sequence.
class Sequencer
{
public:
	Sequencer(const Sequence& seq) : seq(&seq) {
		restart();
	}

	// Switch to a new sequence.
	// Also resets the play counter.
	void switch_to(const Sequence& sequence) {
		seq = &sequence;
		restart();
	}

	// Advance the play counter
	// by dt milliseconds.
	void progress(double dt);

	// Reset the play counter.
	void restart() {
		cur_frame = seq->frames.begin();
		time = cur_frame->duration;
		direction = 1;
	}

	// The current animation frame
	// of the sequence.
	Frame current_frame() const {
		return *cur_frame;
	}

private:
	bool is_last_frame() const;

	const Sequence* seq;
	std::vector<Frame>::const_iterator cur_frame;
	double time = 0.0;
	short direction = 1;
};
}
