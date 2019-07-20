#pragma once

#include <algorithm>

#include "sequence.hpp"

namespace anim {
// Controls over an animation sequence.
template <typename T>
class Sequencer {
public:
  Sequencer(Sequence<T> seq, double speed = 1.0)
      : seq(std::move(seq)), speed(speed) {
    restart();
  }

  // Advance the play counter
  // by dt (real life) milliseconds.
  void progress(double dt) { advance(dt * speed); }

  // Reset the play counter.
  void restart() {
    cur_frame = seq.frames.begin();
    time = accum = 0.0;
    direction = 1;
  }

  void fast_forward(double speed) { this->speed = speed; }
  void skip_to(double t) {
    restart();
    advance(t);
  }

  void pause() { fast_forward(0.0); }
  void resume() { fast_forward(1.0); }

  // The current animation frame
  // of the sequence.
  T current_value() const { return cur_frame->value; }
  double current_time() const { return time; }

private:
  void advance(double t) {
    accum += t;
    time += t;
    while (accum >= cur_frame->duration) {
      accum -= cur_frame->duration;
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
    }
  }

  bool is_last_frame() const {
    return cur_frame == (seq.frames.end() - 1) ||
           (cur_frame == seq.frames.begin() && direction < 0);
  }

  Sequence<T> seq;
  double speed;
  typename std::vector<Frame<T>>::const_iterator cur_frame;
  double time = 0.0, accum = 0.0;
  short direction = 1;
};
} // namespace anim
