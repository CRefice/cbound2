#pragma once

#ifdef _WIN32
//Do not include other headers such as winsock.h
#define WIN32_LEAN_AND_MEAN
//Do not define min and max macros, which are unneeded in C++
#define NOMINMAX

#include <Windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>

//The WinApi defines a macro, MessageBox,
//which conflicts with our MessageBox namespace.
#undef MessageBox

namespace Goo
{
//Type definitions
using NativeControl = HWND;
using NativeFont = HFONT;
using NativeMenu = HMENU;
using NativeImage = HBITMAP;
using NativeLayout = void*; //WinApi doesn't use layouts
using GLContext = HGLRC;

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}

#elif defined(__linux__) //Should probably check for GTK in CMake instead.
#include <gtk/gtk.h>
	
namespace Goo
{
using NativeControl = GtkWidget*;
using NativeFont = PangoFont*;
using NativeMenu = GtkMenuShell*;
using NativeImage = GtkPixbuf*;
using NativeLayout = GtkContainer*;
using GLContext =; 


#else
#error "You're on an unsupported platform. Sorry!"

#endif
