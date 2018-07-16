#pragma once

#include <cstdint>
#include <ssm/vector.hpp>

#include "glinterface/texture.hpp"

class Texture
{
public:
	Texture(ssm::ivec2 size, const uint8_t* data);

	auto size() { return sz; }
	auto handle() { return handl; }

private:
	GlObject<GlTexture> handl;
	ssm::ivec2 sz;
}
