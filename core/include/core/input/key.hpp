#pragma once

#include <functional>

namespace input {
struct KeyEvent {
  enum class Action { Press, Release };
  int key;
  Action action;

  friend bool operator==(const KeyEvent& a, const KeyEvent& b) {
    return a.key == b.key && a.action == b.action;
  }
};

class KeyHandler {
public:
  virtual void handle(KeyEvent event) = 0;
};
} // namespace input

namespace std {
template <>
struct hash<input::KeyEvent> {
  std::size_t operator()(const input::KeyEvent& key) const {
    return hash<int>()(key.key) ^
           hash<bool>()(key.action == input::KeyEvent::Action::Press);
  }
};
} // namespace std
