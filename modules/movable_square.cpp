#include <list>
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
#include "modules/movable_square.hpp"

namespace movable_square {
    const std::string module_name("movable_square");

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
    const float square_units_per_msec = 0.001f;
    const float square_unit_offset = 0.025f;

    bool left_pressed = 0;
    bool right_pressed = 0;
    bool up_pressed = 0;
    bool down_pressed = 0;

    int run(int argc, char **argv) {
        engine e;
        window main_window;

        std::list<shader> shaders;
        shaders.emplace_back(GL_VERTEX_SHADER, vertex_shader_source);
        shaders.emplace_back(GL_FRAGMENT_SHADER, fragment_shader_source);
        shader_program main_program(shaders);

        std::vector<float> square_vertex_vector {
            0.1f, 0.1f, 0.0f, 1.0f,
            -0.1f, 0.1f, 0.0f, 1.0f,
            -0.1f, -0.1f, 0.0f, 1.0f,
            0.1f, -0.1f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f
        };
        const int vertex_count = square_vertex_vector.size() / vertex_depth / 2;
        vertex_buffer square_vertices(square_vertex_vector);

        main_program.use();
        square_vertices.bind();
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
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        done = true;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_LEFT:  left_pressed = 1; break;
                            case SDLK_RIGHT: right_pressed = 1; break;
                            case SDLK_UP:    up_pressed = 1; break;
                            case SDLK_DOWN:  down_pressed = 1; break;
                        }
                        break;
                    case SDL_KEYUP:
                        switch (event.key.keysym.sym) {
                            case SDLK_LEFT:  left_pressed = 0; break;
                            case SDLK_RIGHT: right_pressed = 0; break;
                            case SDLK_UP:    up_pressed = 0; break;
                            case SDLK_DOWN:  down_pressed = 0; break;
                        }
                        break;
                }
            }

            if (left_pressed) {
                offsets.x -= square_unit_offset;
            }
            if (right_pressed) {
                offsets.x += square_unit_offset;
            }
            if (up_pressed) {
                offsets.y += square_unit_offset;
            }
            if (down_pressed) {
                offsets.y -= square_unit_offset;
            }

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUniform2f(offset_uniform, offsets.x, offsets.y);

            glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count);

            main_window.swap();
        }

        return 0;
    }
}
