#include <fstream>
#include <sstream>

#include <gl\core.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "sprite-renderer.hpp"

static GLuint sprite_vao;
static Resource<Shader> common_shader;

static void setup_sprite_vao() {
	GLuint vbo, ebo;
	GLfloat vertices[] = {
		//Position		//Texcoord
		1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f,		0.0f, 0.0f,
		0.0f, 1.0f,		0.0f, 1.0f
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &sprite_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(sprite_vao);
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

namespace sprite_renderer
{
void init(Resource<Shader> shader) {
	setup_sprite_vao();
	
	common_shader = shader;
	shader->uniform("current_frame", 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void draw(Sprite& sprite, const vec2<uint>& pos) {
	glUseProgram(common_shader->handle);
	const glm::mat4 projection = glm::ortho(0.0f, 320.0f, 180.0f, 0.0f, -10000.0f, 10000.0f);
	common_shader->uniform("projection", projection);

	float width = float(sprite.texture()->size.x);
	float height = float(sprite.texture()->size.y);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(float(pos.x), float(pos.y), float(pos.y)));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));
		
	common_shader->uniform("model", model);

	glBindVertexArray(sprite_vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite.texture()->handle);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
}
