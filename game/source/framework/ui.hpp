#pragma once

#include <optional>

#include <sol/sol.hpp>

#include "ui/widget.hpp"

namespace fw::ui {
// Add all the framework definitions to the given state.
void load_libraries(sol::state& state);
} // namespace fw::ui
