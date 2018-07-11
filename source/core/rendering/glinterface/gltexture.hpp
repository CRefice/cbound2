#pragma once

struct GlTexture
{
	using value_type = unsigned int;
	static value_type create();
	static void destroy(value_type);
};
