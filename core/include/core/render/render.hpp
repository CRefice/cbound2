#pragma once

#include <memory>

// Forward declarations
struct GLFWwindow;

namespace render {
using GLContext = GLFWwindow*;
using Instance = std::unique_ptr<GLFWwindow, void (*)(GLContext)>;
Instance create_instance();
} // namespace render
