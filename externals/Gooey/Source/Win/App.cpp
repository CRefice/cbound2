#include <functional>

#include "Platform.hpp"

#include "App.hpp"

namespace Goo
{
namespace App
{
void run() {
	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0) != 0) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void run(Window& wnd) {
	wnd.show();
	wnd.onDestroy.setHandler(quit);

	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0) != 0) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void quit() {
	::PostQuitMessage(0);
}

void pollEvents() {
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
}
}

#pragma comment(linker, "\"/manifestdependency:type='win32' \
						name='Microsoft.Windows.Common-Controls' \
						version='6.0.0.0' \
						processorArchitecture='*' \
						publicKeyToken='6595b64144ccf1df' language='*'\"")
