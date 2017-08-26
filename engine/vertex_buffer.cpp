#include <stdexcept>
#include <string>

#include "engine/vertex_buffer.hpp"

vertex_buffer::vertex_buffer(std::unique_ptr<std::vector<GLfloat>> buffer) {
    glGenBuffers(1, &this->buffer_id);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, buffer->size() * sizeof(GLfloat), buffer->data(), GL_STATIC_DRAW);
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
