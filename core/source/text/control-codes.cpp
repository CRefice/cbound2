#include "text/control-code.hpp"

namespace text {
std::optional<ControlCode> parse_ctrl_code(std::string_view text, std::string_view::size_type* index) {
	if (text[0] != '{')
		return {};
	auto end = text.find('}');
	if (end == std::string_view::npos)
		return {};

	if (index != nullptr) {
		*index = end + 1;
	}

	std::string_view ctrl_str = text.substr(1, end - 1);
	auto colon = ctrl_str.find(':');
	ControlCode ret;
	ret.name = ctrl_str.substr(0, colon);
	ret.argument = colon == std::string_view::npos
		? std::nullopt : std::optional(ctrl_str.substr(colon + 1));
	return ret;
}
}
