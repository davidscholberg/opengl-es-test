#ifndef DRAWABLE_HPP_
#define DRAWABLE_HPP_

#include <memory>
#include <vector>

#include <stdint.h>

#include "engine/vertex_buffer.hpp"
#include "engine/shader_program.hpp"

class drawable {
protected:
    vertex_buffer vertices;
    int vertex_depth;
    int vertex_count;
    std::shared_ptr<shader_program> program;
    uint32_t position_attrib;
    uint32_t color_attrib;
    uint32_t offset_uniform;
    float offset_x;
    float offset_y;
    float offset_z;
public:
    drawable(const std::vector<float> &vertex_vector, const int vertex_depth, std::shared_ptr<shader_program> program);
    drawable(drawable const &) = delete;
    void operator=(drawable const &) = delete;
    void update_offsets(float dx, float dy, float dz);
    void draw();
};

#endif // DRAWABLE_HPP_
