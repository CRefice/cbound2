#include "Button.hpp"

namespace Goo
{
void Button::createControl() {
	createHandle("BUTTON", text(), BS_PUSHBUTTON);
}
}
