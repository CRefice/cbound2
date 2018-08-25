#pragma once

#include <optional>

#include "core/render/text-batch.hpp"

namespace anim {
class TextDrawl
{
public:
	// Speed is in secs/char
	TextDrawl(std::string text, double sec_per_char = 0.02)
		: txt(std::move(text)), len(0),
			sec_per_char(sec_per_char), countdown(sec_per_char) {}

	auto current_slice() const {
		return std::string_view(txt.c_str(), len);
	}
	bool done() const {
		return len == txt.length();
	}

	void progress(double dt);
	void skip() {
		len = txt.length();
	}

private:
	void next_char();
	std::optional<double> parse_wait();

	std::string txt;
	std::string::size_type len;
	double sec_per_char;
	double countdown;
};
} 
