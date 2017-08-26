#ifndef VERTEX_BUFFER_HPP_
#define VERTEX_BUFFER_HPP_

#include <memory>
#include <vector>

#include <GLES2/gl2.h>

class vertex_buffer {
protected:
    GLuint buffer_id;
public:
    vertex_buffer(std::unique_ptr<std::vector<GLfloat>> buffer);
    vertex_buffer(vertex_buffer const &) = delete;
    ~vertex_buffer();
    void operator=(vertex_buffer const &) = delete;
    void bind();
    void unbind();
};

#endif // VERTEX_BUFFER_HPP_
