#pragma once

#include <functional>

namespace input {
struct KeyEvent {
  enum class Action { Press, Release };
  int key;
  Action action;

  friend bool operator==(const KeyEvent &a, const KeyEvent &b) {
    return a.key == b.key && a.action == b.action;
  }
};

struct KeyRange {
  int key_pos, key_neg;

  friend bool operator==(const KeyRange &a, const KeyRange &b) {
    return a.key_pos == b.key_pos && a.key_neg == b.key_neg;
  }
};
} // namespace input

namespace std {
template <> struct hash<input::KeyEvent> {
  std::size_t operator()(const input::KeyEvent &key) const {
    return hash<int>()(key.key) ^
           hash<bool>()(key.action == input::KeyEvent::Action::Press);
  }
};

template <> struct hash<input::KeyRange> {
  std::size_t operator()(const input::KeyRange &key) const {
    auto a = hash<int>()(key.key_pos);
    auto b = hash<int>()(key.key_neg);
    return a >= b ? a * a + a + b : a + b * b;
  }
};
} // namespace std
