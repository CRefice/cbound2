#pragma once

#include <cstdlib> // std::abs()
#include <utility>

#include <ssm/vector.hpp>

// An immutable rectangle whose dimensions are stored as vectors of type T.
template <typename T>
class Rectangle
{
public:
	// TODO: swap axes, check for actual top left bottom right invariants
	Rectangle(ssm::vector<T, 2> top_left, ssm::vector<T, 2> bottom_right)
		: tl(std::move(top_left)), br(std::move(bottom_right)) {}

	Rectangle(T left, T top, T right, T bottom)
		: tl(left, top), br(right, bottom) {}

	auto pos() const {
		return tl;
	}
	auto size() const {
		return br - tl;
	}
	auto top_left() const {
		return tl;
	}
	auto top_right() const {
		return ssm::vector<T, 2>(br.x, tl.y);
	}
	auto bottom_left() const {
		return ssm::vector<T, 2>(tl.x, br.y);
	}
	auto bottom_right() const {
		return br;
	}

	T width() const {
		return std::abs(br.x - tl.x);
	}
	T height() const {
		return std::abs(br.y - tl.y);
	}
	T left() const {
		return tl.x;
	}
	T right() const {
		return br.x;
	}
	T top() const {
		return tl.y;
	}
	T bottom() const {
		return br.y;
	}

private:
	ssm::vector<T, 2> tl, br;
};
