#include <memory>
#include <vector>

#include <math.h>

#include <SDL2/SDL.h>

#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"
#include "modules/translated_triangle.hpp"

#define VERTEX_DEPTH 4
#define VERTEX_COUNT 3

#define CIRCLE_PERIOD 0.25f
#define CIRCLE_RADIUS 0.7f
#define ANGULAR_RATIO M_PI * 2.0f / CIRCLE_PERIOD

struct vec2 {
    GLfloat x;
    GLfloat y;
};

const char *translated_triangle_vertex_shader_source = R"glsl(
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

const char *translated_triangle_fragment_shader_source = R"glsl(
#version 100

precision mediump float;

varying vec4 fragment_color;

void main() {
   gl_FragColor = fragment_color;
}
)glsl";

void get_circular_offsets(vec2 *offsets) {
    GLfloat elapsed_time = SDL_GetTicks() / 1000.0f;
    GLfloat elapsed_period = fmodf(elapsed_time, CIRCLE_PERIOD);
    GLfloat angle = ANGULAR_RATIO * elapsed_period;
    offsets->x = CIRCLE_RADIUS * cosf(angle);
    offsets->y = CIRCLE_RADIUS * sinf(angle);
}

void translated_triangle() {
    window main_window;

    auto shaders = std::make_unique<std::vector<std::unique_ptr<shader>>>();
    shaders->push_back(std::make_unique<shader>(GL_VERTEX_SHADER, translated_triangle_vertex_shader_source));
    shaders->push_back(std::make_unique<shader>(GL_FRAGMENT_SHADER, translated_triangle_fragment_shader_source));
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
    GLint position_attrib = main_program.get_attrib_location("position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, VERTEX_DEPTH, GL_FLOAT, GL_FALSE, 0, 0);
    GLint color_attrib = main_program.get_attrib_location("color");
    glEnableVertexAttribArray(color_attrib);
    glVertexAttribPointer(
            color_attrib,
            VERTEX_DEPTH,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*) (sizeof(GLfloat) * VERTEX_DEPTH * VERTEX_COUNT));

    GLint offset_uniform = main_program.get_uniform_location("offset");
    vec2 offsets;

    SDL_Event event;
    bool done = false;
    while (!done) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        get_circular_offsets(&offsets);
        glUniform2f(offset_uniform, offsets.x, offsets.y);

        glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

        main_window.swap();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
    }
}
