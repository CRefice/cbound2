#include <gl\core.h>

#include <stdexcept>

#include "common\inttypes.hpp"
#include "common\vec2.hpp"

#include "render-common.hpp"

namespace rendercommon
{

constexpr vec2<uint> buffer_size{ 320, 180 };

static GLuint render_fbo;
static GLuint render_texture;
static GLuint quad_vao;
static Resource<Shader> pp_shader;

static vec2<uint> viewport_pos, viewport_size;

static Goo::GLWindow* draw_wnd;

static void setup_vao() {
	GLuint vbo, ebo;
	GLfloat vertices[] = {
		//Position		//Texcoord
		 1.0f,  1.0f,	1.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

static void setup_fbo(GLuint& fbo, GLuint& tex, vec2<uint> size) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void init(Goo::GLWindow& window) {
	if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		throw std::runtime_error("Failed to load OpenGL core functions!");
	}

	draw_wnd = &window;

	setup_vao();
	setup_fbo(render_fbo, render_texture, { 320, 180 });
}

void shutdown() {}

void set_postprocess_shader(Resource<Shader> shader) {
	pp_shader = shader;
}

void set_viewport(vec2<uint> pos, vec2<uint> size) {
	viewport_pos = pos;
	viewport_size = size;
}

void clear_screen() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void begin_draw() {
	glViewport(0, 0, 320, 180);
	glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);
	glClearColor(0.3f, 0.6f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void end_draw() {
	glViewport(viewport_pos.x, viewport_pos.y, viewport_size.x, viewport_size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pp_shader->handle);
	pp_shader->uniform("u_texelStep", { 320, 180 });
	pp_shader->uniform("u_showEdges", 0);
	pp_shader->uniform("u_fxaaOn", 1);
	pp_shader->uniform("u_lumaThreshold", 0.1f);
	pp_shader->uniform("u_mulReduce", 1.0f / 8.0f);
	pp_shader->uniform("u_minReduce", 1.0f / 128.0f);
	pp_shader->uniform("u_maxSpan", 8.0f);

	glBindVertexArray(quad_vao);

	glBindTexture(GL_TEXTURE_2D, render_texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
}
