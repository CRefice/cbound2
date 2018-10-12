#include <cstdlib>

#include "anim/text-drawl.hpp"

static std::optional<double> parse_double(std::string_view str) {
	char* end;
	double result = std::strtod(str.data(), &end);
	return end == str.data() ? std::nullopt : std::optional(result);
}

namespace anim {
void TextDrawl::progress(double dt) {
	countdown -= dt;
	if (countdown <= 0.0) {
		countdown = sec_per_char;
		next_char();
	}
}

void TextDrawl::next_char() {
	std::string_view slice(txt);
	slice.remove_prefix(len);
	auto index = txt.length();
	if (auto maybe_code = text::parse_ctrl_code(slice, &index)) {
		len += index;
		handle_ctrl_code(*maybe_code);
	}
	if (len < txt.length()) {
		len++;
	}
}

void TextDrawl::handle_ctrl_code(const text::ControlCode& code) {
	if (code.name == "wait") {
		const auto& arg = code.argument.value_or("0.5");
		auto time = parse_double(arg);
		if (time) {
			countdown += *time;
		}
	}
}
} // namespace anim
