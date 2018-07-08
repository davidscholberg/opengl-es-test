#include <list>
#include <memory>
#include <vector>

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "engine/engine.hpp"
#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"
#include "modules/static_triangle.hpp"

namespace static_triangle {
    const std::string module_name("static_triangle");

    const char *vertex_shader_source = R"glsl(
#version 100

attribute vec4 position;
attribute vec4 color;

varying vec4 fragment_color;

void main() {
    fragment_color = color;
    gl_Position = position;
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

    int run(int argc, char **argv) {
        engine e;
        window main_window;

        std::list<shader> shaders;
        shaders.emplace_back(GL_VERTEX_SHADER, vertex_shader_source);
        shaders.emplace_back(GL_FRAGMENT_SHADER, fragment_shader_source);
        shader_program main_program(shaders);

        std::vector<float> triangle_vertex_vector {
            0.0f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f
        };
        const int vertex_count = triangle_vertex_vector.size() / vertex_depth / 2;
        vertex_buffer triangle_vertices(triangle_vertex_vector);

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

        SDL_Event event;
        bool done = false;
        while (!done) {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

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
