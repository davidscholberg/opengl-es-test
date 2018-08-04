#include <list>
#include <memory>
#include <vector>

#include <math.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "engine/drawable.hpp"
#include "engine/engine.hpp"
#include "engine/keyboard_state.hpp"
#include "engine/scene.hpp"
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

        auto square_1 = std::make_shared<drawable>(square_1_vertex_vector, vertex_depth, *main_program);
        auto square_2 = std::make_shared<drawable>(square_2_vertex_vector, vertex_depth, *main_program);
        auto drawables = std::make_shared<std::list<std::shared_ptr<drawable>>>();
        drawables->push_back(square_1);
        drawables->push_back(square_2);
        scene squares(drawables, main_program);

        keyboard_state kb;

        SDL_Event event;
        bool done = false;
        while (!done) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        done = true;
                        break;
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        kb.update_state(event.type, event.key.keysym.sym);
                        break;
                }
            }

            if (kb.get_up_pressed()) {
                square_1->update_offsets(0, square_unit_offset, 0);
            }
            if (kb.get_left_pressed()) {
                square_1->update_offsets(-square_unit_offset, 0, 0);
            }
            if (kb.get_down_pressed()) {
                square_1->update_offsets(0, -square_unit_offset, 0);
            }
            if (kb.get_right_pressed()) {
                square_1->update_offsets(square_unit_offset, 0, 0);
            }
            if (kb.get_w_pressed()) {
                square_2->update_offsets(0, square_unit_offset, 0);
            }
            if (kb.get_a_pressed()) {
                square_2->update_offsets(-square_unit_offset, 0, 0);
            }
            if (kb.get_s_pressed()) {
                square_2->update_offsets(0, -square_unit_offset, 0);
            }
            if (kb.get_d_pressed()) {
                square_2->update_offsets(square_unit_offset, 0, 0);
            }

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            squares.draw();

            main_window.swap();
        }

        return 0;
    }
}
