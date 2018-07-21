#include <list>
#include <memory>
#include <vector>

#include <math.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "engine/drawable.hpp"
#include "engine/engine.hpp"
#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"
#include "modules/movable_squares.hpp"

namespace movable_squares {
    const std::string module_name("movable_squares");

    const char *vertex_shader_source = R"glsl(
#version 100

attribute vec4 position;
attribute vec4 color;

varying vec4 fragment_color;

uniform vec3 offset;

void main() {
    fragment_color = color;
    gl_Position = position + vec4(offset.xyz, 0.0);
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
    const float square_unit_offset = 0.025f;

    bool up_pressed = 0;
    bool left_pressed = 0;
    bool down_pressed = 0;
    bool right_pressed = 0;
    bool w_pressed = 0;
    bool a_pressed = 0;
    bool s_pressed = 0;
    bool d_pressed = 0;

    int run(int argc, char **argv) {
        engine e;
        window main_window;

        std::list<shader> shaders;
        shaders.emplace_back(GL_VERTEX_SHADER, vertex_shader_source);
        shaders.emplace_back(GL_FRAGMENT_SHADER, fragment_shader_source);
        auto main_program = std::make_shared<shader_program>(shaders);

        std::vector<float> square_1_vertex_vector {
            0.0f, 0.0f, 0.0f, 1.0f,
            -0.2f, 0.0f, 0.0f, 1.0f,
            -0.2f, -0.2f, 0.0f, 1.0f,
            0.0f, -0.2f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f
        };

        std::vector<float> square_2_vertex_vector {
            0.1f, 0.1f, 0.0f, 1.0f,
            -0.1f, 0.1f, 0.0f, 1.0f,
            -0.1f, -0.1f, 0.0f, 1.0f,
            0.1f, -0.1f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f
        };

        auto square_1 = std::make_shared<drawable>(square_1_vertex_vector, vertex_depth, main_program);
        auto square_2 = std::make_shared<drawable>(square_2_vertex_vector, vertex_depth, main_program);
        std::list<std::shared_ptr<drawable>> drawables;
        drawables.push_back(square_1);
        drawables.push_back(square_2);

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
                            case SDLK_UP:    up_pressed = 1; break;
                            case SDLK_LEFT:  left_pressed = 1; break;
                            case SDLK_DOWN:  down_pressed = 1; break;
                            case SDLK_RIGHT: right_pressed = 1; break;
                            case SDLK_w: w_pressed = 1; break;
                            case SDLK_a: a_pressed = 1; break;
                            case SDLK_s: s_pressed = 1; break;
                            case SDLK_d: d_pressed = 1; break;
                        }
                        break;
                    case SDL_KEYUP:
                        switch (event.key.keysym.sym) {
                            case SDLK_UP:    up_pressed = 0; break;
                            case SDLK_LEFT:  left_pressed = 0; break;
                            case SDLK_DOWN:  down_pressed = 0; break;
                            case SDLK_RIGHT: right_pressed = 0; break;
                            case SDLK_w: w_pressed = 0; break;
                            case SDLK_a: a_pressed = 0; break;
                            case SDLK_s: s_pressed = 0; break;
                            case SDLK_d: d_pressed = 0; break;
                        }
                        break;
                }
            }

            if (up_pressed) {
                square_1->update_offsets(0, square_unit_offset, 0);
            }
            if (left_pressed) {
                square_1->update_offsets(-square_unit_offset, 0, 0);
            }
            if (down_pressed) {
                square_1->update_offsets(0, -square_unit_offset, 0);
            }
            if (right_pressed) {
                square_1->update_offsets(square_unit_offset, 0, 0);
            }
            if (w_pressed) {
                square_2->update_offsets(0, square_unit_offset, 0);
            }
            if (a_pressed) {
                square_2->update_offsets(-square_unit_offset, 0, 0);
            }
            if (s_pressed) {
                square_2->update_offsets(0, -square_unit_offset, 0);
            }
            if (d_pressed) {
                square_2->update_offsets(square_unit_offset, 0, 0);
            }

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (const auto &d : drawables) {
                d->draw();
            }

            main_window.swap();
        }

        return 0;
    }
}
