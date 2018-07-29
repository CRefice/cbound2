#pragma once

#include "sequence.hpp"

namespace anim {
// Controls over an animation sequence.
class Sequencer
{
public:
	Sequencer(const Sequence& seq) : seq(&seq) {}

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
		time = 0.0;
		multiplier = 1;
	}

	// The current animation frame
	// of the sequence.
	Frame current_frame() const;

private:
	const Sequence* seq;
	double time = 0.0;
	short multiplier = 1;
};
}
