#pragma once

#include "Window.hpp"

namespace Goo
{
namespace App
{
	void run();
	void run(Window& wnd);

	void quit();
	void pollEvents();
}
}
