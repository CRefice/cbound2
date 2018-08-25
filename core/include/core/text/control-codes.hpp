#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace text {
struct ControlCode
{
	std::string name;
	std::optional<std::string> argument;
};

// If the string view starts with a control code, it returns it,
// and index receives the location of the start of the first
// non-control code character.
// Otherwise, std::nullopt is returned, and index is left unchanged.
std::optional<ControlCode> parse_ctrl_code(std::string_view text, std::string_view::size_type* index = nullptr);
}
