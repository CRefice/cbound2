#include "Platform.hpp"

#include "Keys.hpp"

namespace Goo
{
bool keyDown(Key key) {
	//TODO: give more information...?
  return ::GetAsyncKeyState(static_cast<unsigned char>(key)) != 0;
}
}
