#include <memory>
#include <vector>

#include <math.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "engine/engine.hpp"
#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"
#include "modules/translated_triangle.hpp"

namespace translated_triangle {
    const std::string module_name("translated_triangle");

    struct vec2 {
        float x;
        float y;
    };

    const char *vertex_shader_source = R"glsl(
#version 100

attribute vec4 position;
attribute vec4 color;

varying vec4 fragment_color;

uniform vec2 offset;

void main() {
    fragment_color = color;
    gl_Position = position + vec4(offset.xy, 0.0, 0.0);
}
)glsl";

    const char *fragment_shader_source = R"glsl(
#version 100

precision mediump float;

varying vec4 fragment_color;

void main() {
   gl_FragColor = fragment_color;
}
)glsl";

    const int vertex_depth = 4;
    const float cirle_period = 10.0f;
    const float cirle_radius = 0.7f;
    const float angular_ratio = M_PI * 2.0f / cirle_period;

    void get_circular_offsets(vec2 *offsets) {
        float elapsed_time = SDL_GetTicks() / 1000.0f;
        float elapsed_period = fmodf(elapsed_time, cirle_period);
        float angle = angular_ratio * elapsed_period;
        offsets->x = cirle_radius * cosf(angle);
        offsets->y = cirle_radius * sinf(angle);
    }

    int run(int argc, char **argv) {
        engine e;
        window main_window;

        auto shaders = std::make_unique<std::vector<std::unique_ptr<shader>>>();
        shaders->push_back(std::make_unique<shader>(GL_VERTEX_SHADER, vertex_shader_source));
        shaders->push_back(std::make_unique<shader>(GL_FRAGMENT_SHADER, fragment_shader_source));
        shader_program main_program(std::move(shaders));

        auto triangle_vertex_vector = std::make_unique<std::vector<float>, std::initializer_list<float>>({
                0.0f, 0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f, 1.0f});
        const int vertex_count = triangle_vertex_vector->size() / vertex_depth / 2;
        vertex_buffer triangle_vertices(std::move(triangle_vertex_vector));

        main_program.use();
        triangle_vertices.bind();
        uint32_t position_attrib = main_program.get_attrib_location("position");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, vertex_depth, GL_FLOAT, GL_FALSE, 0, 0);
        uint32_t color_attrib = main_program.get_attrib_location("color");
        glEnableVertexAttribArray(color_attrib);
        glVertexAttribPointer(
                color_attrib,
                vertex_depth,
                GL_FLOAT,
                GL_FALSE,
                0,
                (GLvoid*) (sizeof(float) * vertex_depth * vertex_count));

        uint32_t offset_uniform = main_program.get_uniform_location("offset");
        vec2 offsets;

        SDL_Event event;
        bool done = false;
        while (!done) {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            get_circular_offsets(&offsets);
            glUniform2f(offset_uniform, offsets.x, offsets.y);

            glDrawArrays(GL_TRIANGLES, 0, vertex_count);

            main_window.swap();

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = true;
                }
            }
        }

        return 0;
    }
}
