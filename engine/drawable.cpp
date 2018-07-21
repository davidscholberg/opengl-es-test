#include <SDL2/SDL_opengles2.h>

#include "engine/drawable.hpp"

drawable::drawable(const std::vector<float> &vertex_vector, const int vertex_depth, std::shared_ptr<shader_program> program)
    : vertices(vertex_vector), vertex_depth(vertex_depth), program(program) {
    this->vertex_count = vertex_vector.size() / this->vertex_depth / 2;

	this->position_attrib = this->program->get_attrib_location("position");
	this->color_attrib = this->program->get_attrib_location("color");
	this->offset_uniform = this->program->get_uniform_location("offset");
}

void drawable::update_offsets(float dx, float dy, float dz) {
    this->offset_x += dx;
    this->offset_y += dy;
    this->offset_z += dz;
}

void drawable::draw() {
    this->program->use();
    this->vertices.bind();

	glEnableVertexAttribArray(this->position_attrib);
	glVertexAttribPointer(
            this->position_attrib,
            this->vertex_depth,
            GL_FLOAT,
            GL_FALSE,
            0,
            0);

	glEnableVertexAttribArray(this->color_attrib);
	glVertexAttribPointer(
			this->color_attrib,
			this->vertex_depth,
			GL_FLOAT,
			GL_FALSE,
			0,
			(GLvoid*) (sizeof(float) * this->vertex_depth * this->vertex_count));

    glUniform3f(this->offset_uniform, this->offset_x, this->offset_y, this->offset_z);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertex_count);

    this->vertices.unbind();
    this->program->clear();
}
