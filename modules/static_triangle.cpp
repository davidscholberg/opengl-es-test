#include <memory>
#include <vector>

#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"
#include "modules/static_triangle.hpp"

#define VERTEX_DEPTH 4
#define VERTEX_COUNT 3

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

void static_triangle() {
    window main_window;

    auto shaders = std::make_unique<std::vector<std::unique_ptr<shader>>>();
    shaders->push_back(std::make_unique<shader>(GL_VERTEX_SHADER, vertex_shader_source));
    shaders->push_back(std::make_unique<shader>(GL_FRAGMENT_SHADER, fragment_shader_source));
    shader_program main_program(std::move(shaders));

    auto triangle_vertex_vector = std::make_unique<std::vector<GLfloat>, std::initializer_list<GLfloat>>({
            0.0f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f});
    vertex_buffer triangle_vertices(std::move(triangle_vertex_vector));

    main_program.use();
    triangle_vertices.bind();
    GLuint position_attrib = main_program.get_attrib_location("position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, VERTEX_DEPTH, GL_FLOAT, GL_FALSE, 0, 0);
    GLuint color_attrib = main_program.get_attrib_location("color");
    glEnableVertexAttribArray(color_attrib);
    glVertexAttribPointer(
            color_attrib,
            VERTEX_DEPTH,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*) (sizeof(GLfloat) * VERTEX_DEPTH * VERTEX_COUNT));

    SDL_Event event;
    bool done = false;
    while (!done) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

        main_window.swap();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
    }
}
