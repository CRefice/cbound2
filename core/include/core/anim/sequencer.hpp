#pragma once

#include "sequence.hpp"

namespace anim {
// Controls over an animation sequence.
template <typename T>
class Sequencer {
public:
  Sequencer(Sequence<T> seq) : seq(std::move(seq)) { restart(); }

  // Advance the play counter
  // by dt milliseconds.
  void progress(double dt) {
    time -= speed * dt;
    if (time <= 0.0) {
      if (is_last_frame()) {
        switch (seq.mode) {
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

  // Reset the play counter.
  void restart() {
    cur_frame = seq.frames.begin();
    time = cur_frame->duration;
    direction = 1;
  }

  void fast_forward(double speed) { this->speed = speed; }
  void pause() { fast_forward(0.0); }
  void resume() { fast_forward(1.0); }

  // The current animation frame
  // of the sequence.
  T current_value() const { return cur_frame->value; }

private:
  bool is_last_frame() const {
    return cur_frame == (seq.frames.end() - 1) ||
           (cur_frame == seq.frames.begin() && direction < 0);
  }

  Sequence<T> seq;
  typename std::vector<Frame<T>>::const_iterator cur_frame;
  double time = 0.0;
  double speed = 1.0;
  short direction = 1;
};
} // namespace anim
