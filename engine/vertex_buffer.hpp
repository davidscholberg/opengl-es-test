#ifndef VERTEX_BUFFER_HPP_
#define VERTEX_BUFFER_HPP_

#include <memory>
#include <vector>

#include <stdint.h>

class vertex_buffer {
protected:
    uint32_t buffer_id;
public:
    vertex_buffer(std::unique_ptr<std::vector<float>> buffer);
    vertex_buffer(vertex_buffer const &) = delete;
    ~vertex_buffer();
    void operator=(vertex_buffer const &) = delete;
    void bind();
    void unbind();
};

#endif // VERTEX_BUFFER_HPP_
