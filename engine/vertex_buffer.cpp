#include <stdexcept>
#include <string>

#include <SDL2/SDL_opengles2.h>

#include "engine/vertex_buffer.hpp"

vertex_buffer::vertex_buffer(std::unique_ptr<std::vector<float>> buffer) {
    glGenBuffers(1, &this->buffer_id);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, buffer->size() * sizeof(float), buffer->data(), GL_STATIC_DRAW);
    this->unbind();
}

vertex_buffer::~vertex_buffer() {
    glDeleteBuffers(1, &this->buffer_id);
}

void vertex_buffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_id);
}

void vertex_buffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
