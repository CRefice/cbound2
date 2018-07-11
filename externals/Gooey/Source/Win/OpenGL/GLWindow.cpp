#include "Platform.hpp"
#include <gl\GL.h>

#include "OpenGL/GLWindow.hpp"

//Little helper function to get extensions
//I shouldn't need this really... should I?
static void* getAnyGLFuncAddress(const char* name) {
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}
	return p;
}

namespace Goo
{
void GLWindow::createControl() {
	std::string classText = text();

	//Need to register the window first,
	//in order to override the class style.
	WNDCLASSEX wclass;
	::ZeroMemory(&wclass, sizeof(wclass));
	wclass.cbSize = sizeof(wclass);
	wclass.style = CS_OWNDC;
	wclass.lpfnWndProc = wndProc;
	wclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wclass.hInstance = ::GetModuleHandle(NULL);
	wclass.hbrBackground = ::CreateSolidBrush(RGB(0, 0, 0));
	wclass.lpszClassName = classText.c_str();
	::RegisterClassEx(&wclass);

	//Window registration will now fail, but no worries!
	Window::createControl();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC hdc = ::GetDC(handle());

	const int format = ::ChoosePixelFormat(hdc, &pfd);
	::SetPixelFormat(hdc, format, &pfd);

	_context = ::wglCreateContext(hdc);
	::wglMakeCurrent(hdc, _context);

	setViewport({ 0, 0 }, clientArea());
}

void GLWindow::swapBuffers() {
	::SwapBuffers(::GetDC(handle()));
}

void GLWindow::setViewport(const Point& point, const Size& size) {
	::glViewport(point.x, point.y, size.x, size.y);
}

void GLWindow::setFullscreen(bool enable) {
	if(enable) {
		const int w = GetSystemMetrics(SM_CXSCREEN);
		const int h = GetSystemMetrics(SM_CYSCREEN);
		::SetWindowLongPtr(handle(), GWL_STYLE, WS_VISIBLE | WS_POPUP);
		::SetWindowPos(handle(), HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);
	} else {
		::SetWindowLongPtr(handle(), GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
		::InvalidateRect(handle(), NULL, TRUE);
		Point pos_ = position();
		Size size_ = size();
		::glViewport(pos_.x, pos_.y, size_.x, size_.y);
	}
}

void GLWindow::setVsync(bool enable) {
	typedef void(*VSyncFunc)(int);
	VSyncFunc func = (VSyncFunc)getAnyGLFuncAddress("wglSwapIntervalEXT");
	func(enable);
}
}
