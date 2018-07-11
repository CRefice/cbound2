#include "RadioButton.hpp"

namespace Goo
{
void RadioButton::createControl() {
	createHandle("BUTTON", _text, BS_AUTORADIOBUTTON);
}
}
