#include <cstdint>
#include <charconv>

#include "common/color.hpp"

static bool result_valid(std::from_chars_result result) {
	return result.ec != std::errc::invalid_argument
		&& result.ec != std::errc::result_out_of_range;
}

namespace color {
std::optional<ssm::vec3> parse_color(std::string_view str) {
	if (str[0] != '#')
		return {};
	str.remove_prefix(1);

	std::uint8_t r, g, b;
	if (result_valid(std::from_chars(str.data() + 0, str.data() + 2, r, 16))
	 && result_valid(std::from_chars(str.data() + 2, str.data() + 4, g, 16))
	 && result_valid(std::from_chars(str.data() + 4, str.data() + 6, b, 16)))
		return ssm::vec3(r / 255.f, g / 255.f, b / 255.f);
	return {};
}
}
