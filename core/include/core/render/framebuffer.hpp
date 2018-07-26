#pragma once

#include <ssm/vector.hpp>

#include "glinterface/framebuffer.hpp"

class FrameBuffer
{
public:
	// We're just going to assume all textures have the same size.
	// Which they damn well should, and if not, the log will catch it.
	template <typename Tex, typename... Args>
	FrameBuffer(Tex& tex, Args&... args)
		: FrameBuffer(tex.size(), tex, args...) {}

	template <typename... Texts>
	FrameBuffer(ssm::ivec2 size, Texts&... texts) : sz(std::move(size)) {
		glBindFramebuffer(GL_FRAMEBUFFER, handl);
		attach(0, texts...);
		allocate_depthbuf(GL_DEPTH24_STENCIL8);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	auto size() { return sz; }

	auto& handle() { return handl; }
	const auto& handle() const { return handl; }

private:
	template <typename Tex, typename... Args>
	void attach(int i, Tex& tex, Args&&... args) {
		glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex.handle(), 0);
		attach(i + 1, args...);
	}
	void attach(int i) { }

	void allocate_depthbuf(GLenum format);

	ssm::ivec2 sz;
	gl::FrameBufferObject handl;
	gl::RenderBufferObject depth_buf;
};
