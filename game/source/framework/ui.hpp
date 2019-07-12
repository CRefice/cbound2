#pragma once

#include <memory>

#include <sol/forward.hpp>

#include "core/render/font.hpp"
#include "core/resource/resource-cache.hpp"

#include "ui/widget.hpp"

namespace fw::ui {
// Parse a UI component from the given table
std::unique_ptr<::ui::Widget> parse_widget(const sol::table& table);
} // namespace fw::ui
