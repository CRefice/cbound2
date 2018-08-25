#include <cstdlib>

#include "anim/text-drawl.hpp"
#include "text/control-codes.hpp"

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
	if (auto wait = parse_wait(); wait) {
		countdown += *wait;
	}
	if (len < txt.length()) {
		len++;
	}
}

std::optional<double> TextDrawl::parse_wait() {
	std::string_view slice(txt);
	slice.remove_prefix(len);
	auto index = txt.length();
	auto maybe_ctrl = text::parse_ctrl_code(slice, &index);
	if (!maybe_ctrl || maybe_ctrl->name != "wait")
		return {};

	txt.erase(len, index);
	const auto& arg = maybe_ctrl->argument.value_or("0.5");
	return parse_double(arg);
}
} // namespace anim
