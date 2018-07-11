#pragma once

#include "ButtonBase.hpp"

namespace Goo
{
class Button : public ButtonBase
{
public:
	Button() = default;
	Button(std::string text) : ButtonBase(std::move(text)) {}
	Button(std::string text, const Point& pos, const Size& size) : ButtonBase(std::move(text), pos, size) {}

private:
	void createControl() override;
};
}
