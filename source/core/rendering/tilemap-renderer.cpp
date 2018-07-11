#include <gl/core.h>

#include <vector>

#include "tilemap-renderer.hpp"

static struct Vertex
{
	vec2<float> position;
	vec2<float> texcoords;
};

static GLuint create_vao(const std::vector<Vertex>& verts, const std::vector<uint>& indices) {
	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);
	glGenBuffers(1, ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Vertex data needs to be dynamic for tile animations
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_DYNAMIC_DRAW);  

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//Index data can be static
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	//Vertex positions
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//Texture coords
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));
	
	glBindVertexArray(0);
	return vao;
}

void StrictTileMapRenderer::reset(Resource<TileMap> tmap, Resource<TileSet> tset) {
	tilemap = tmap;
	tileset = tset;
	
	std::vector<Vertex> verts;
	std::vector<uint> indices;
	
	verts.reserve(tilemap->tiles.size() * 4);
	indices.reserve(tilemap->tiles.size() * 6);

	const auto tilsize = tilemap->tile_size;
	const auto texsize = tileset->tile_size;

	for(uint n = 0; n < tilemap->tiles.size(); ++n) {
		const uint mapx = n % tilemap->size.x;
		const uint mapy = n / tilemap->size.x;
		const uint texx = tilemap->tiles[n] % tileset->size.x;
		const uint texy = tilemap->tiles[n] / tileset->size.x;

		                  //Vertex position                             //Texture coordinate
		verts.push_back({{ (mapx + 0) * tilsize.x, (mapy + 1) * tilsize.y }, { (texx + 0) * texsize.x, (texy + 1) * texsize.y }});	//Bottom left
		verts.push_back({{ (mapx + 1) * tilsize.x, (mapy + 1) * tilsize.y }, { (texx + 1) * texsize.x, (texy + 1) * texsize.y }});	//Bottom right
		verts.push_back({{ (mapx + 1) * tilsize.x, (mapy + 0) * tilsize.y }, { (texx + 1) * texsize.x, (texy + 0) * texsize.y }});	//Top right
		verts.push_back({{ (mapx + 0) * tilsize.x, (mapy + 0) * tilsize.y }, { (texx + 0) * texsize.x, (texy + 0) * texsize.y }});	//Top left

		//First triangle
		indices.push_back((n * 6) + 0); //Bottom left
		indices.push_back((n * 6) + 1); //Bottom right
		indices.push_back((n * 6) + 2); //Top right

		//Second triangle
		indices.push_back((n * 6) + 0); //Bottom left
		indices.push_back((n * 6) + 2); //Top right
		indices.push_back((n * 6) + 3); //Top left
	}

	vao = create_vao(verts, indices);
}

void draw() {
	glUseProgram(shader->handle);
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
