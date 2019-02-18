#pragma once

#include <functional>
#include <stack>
#include <unordered_map>
#include <vector>

#include "key.hpp"

namespace input {
using ActionCallback = std::function<void()>;
using RangeCallback = std::function<void(double)>;

struct Context {
  std::unordered_map<KeyEvent, ActionCallback> actions;
  std::unordered_map<KeyRange, RangeCallback> ranges;
};

class ContextStack {
public:
  using iterator = typename std::vector<Context>::reverse_iterator;
  using const_iterator = typename std::vector<Context>::const_reverse_iterator;

	void push(Context ctext) {
		stack.push_back(std::move(ctext));
	}

	Context pop() {
		auto ret = stack.back();
		stack.pop_back();
		return ret;
	}

  iterator begin() { return stack.rbegin(); }

  iterator end() { return stack.rend(); }

  const_iterator begin() const { return stack.rbegin(); }

  const_iterator end() const { return stack.rend(); }

private:
  std::vector<Context> stack;
};
} // namespace input
