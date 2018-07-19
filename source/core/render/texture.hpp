#pragma once

#include <cstdint>

#include <ssm/vector.hpp>

#include "glinterface/common.hpp"

// A 2D Texture, whose data is stored in an
// OpenGL texture object.
// It's a pretty thin layer over it,
// basically only adding a size member.
class Texture
{
public:
	// This constructor is pretty much the only reason why
	// this isn't a struct. Textures can't change size,
	// and the data is provided to them through an array
	// of RGBA values (in bytes) that is then pushed to the GPU.
	Texture(ssm::ivec2 size, const uint8_t* data);

	auto size() const { return sz; }
	auto& handle() { return handl; }
	const auto& handle() const { return handl; }

private:
	gl::Texture handl;
	ssm::ivec2 sz;
};
