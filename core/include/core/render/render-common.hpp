#pragma once

// Forward declarations
struct GLFWwindow;

namespace render
{
using Context = GLFWwindow*;

// Create and initialize an opengl context.
// Since a context is tied to a window,
// it will also create an unspecified, hidden window.
// Make sure to show it later.
Context create_context();

// Initialize all required openGL apis.
// Requires a created context.
void init(Context context);
void shutdown();
}
