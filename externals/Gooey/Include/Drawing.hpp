#pragma once

namespace Goo
{
struct Point
{
  inline Point(int x, int y) : x(x), y(y) {}
  int x = 0, y = 0;
};

struct Size
{
  inline Size(int x, int y) : x(x), y(y) {}
  int x = 0, y = 0;
};

Point defaultPosition();
Size defaultSize();

inline Point& operator+=(Point& lhs, const Point& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline Point& operator-=(Point& lhs, const Point& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

//Points cannot be multiplied by points
inline Point& operator*=(Point& lhs, const Size& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline Point operator+(Point lhs, const Point& rhs) {
	return lhs += rhs;
}

inline Point operator-(Point lhs, const Point& rhs) {
	return lhs -= rhs;
}

inline Point operator*(Point lhs, const Size& rhs) {
	return lhs *= rhs;
}

inline Size& operator+=(Size& lhs, const Size& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline Size& operator-=(Size& lhs, const Size& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline Size& operator*=(Size& lhs, const Size& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline Size operator+(Size lhs, const Size& rhs) {
	return lhs += rhs;
}

inline Size operator-(Size lhs, const Size& rhs) {
	return lhs -= rhs;
}

inline Size operator*(Size lhs, const Size& rhs) {
	return lhs *= rhs;
}
}
