#include "render/framebuffer.hpp"

void FrameBuffer::allocate_depthbuf(GLenum format) {
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buf);
	glRenderbufferStorage(GL_RENDERBUFFER, format, sz.x, sz.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_buf);
}
