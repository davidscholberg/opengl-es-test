#include <memory>
#include <vector>

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"

#define VERTEX_DEPTH 4
#define VERTEX_COUNT 4

#define Y_ROTATION_PERIOD 5.0f
#define Z_ROTATION_PERIOD 1.0f
#define Y_ANGULAR_RATIO M_PI * 2.0f / Y_ROTATION_PERIOD
#define Z_ANGULAR_RATIO M_PI * 2.0f / Z_ROTATION_PERIOD

const char *vertex_shader_source = R"glsl(
#version 100

attribute vec4 position;
attribute vec4 color;

varying vec4 fragment_color;

uniform float y_rotation_sin;
uniform float y_rotation_cos;
uniform float z_rotation_sin;
uniform float z_rotation_cos;

void main() {
    fragment_color = color;

    mat3 y_rotation = mat3(
        vec3(y_rotation_cos, 0.0, -y_rotation_sin),
        vec3(0.0, 1.0, 0.0),
        vec3(y_rotation_sin, 0.0, y_rotation_cos)
    );

    mat3 z_rotation = mat3(
        vec3(z_rotation_cos, z_rotation_sin, 0.0),
        vec3(-z_rotation_sin, z_rotation_cos, 0.0),
        vec3(0.0, 0.0, 1.0)
    );

    gl_Position = vec4(position.xyz * z_rotation * y_rotation, position.w);
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

GLfloat get_rotation_angle(GLfloat rotation_period, GLfloat angular_ratio) {
    GLfloat elapsed_time = SDL_GetTicks() / 1000.0f;
    GLfloat elapsed_period = fmodf(elapsed_time, rotation_period);
    return angular_ratio * elapsed_period;
}

int main(int argc, char **argv) {
    window main_window;

    auto shaders = std::make_unique<std::vector<std::unique_ptr<shader>>>();
    shaders->push_back(std::make_unique<shader>(GL_VERTEX_SHADER, vertex_shader_source));
    shaders->push_back(std::make_unique<shader>(GL_FRAGMENT_SHADER, fragment_shader_source));
    shader_program main_program(std::move(shaders));

    auto square_vertex_vector = std::make_unique<std::vector<GLfloat>, std::initializer_list<GLfloat>>({
            0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f});
    vertex_buffer square_vertices(std::move(square_vertex_vector));

    main_program.use();
    square_vertices.bind();
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

    GLint y_rotation_sin_uniform = main_program.get_uniform_location("y_rotation_sin");
    GLint y_rotation_cos_uniform = main_program.get_uniform_location("y_rotation_cos");
    GLint z_rotation_sin_uniform = main_program.get_uniform_location("z_rotation_sin");
    GLint z_rotation_cos_uniform = main_program.get_uniform_location("z_rotation_cos");

    SDL_Event event;
    bool done = false;
    while (!done) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat y_rotation_angle = get_rotation_angle(Y_ROTATION_PERIOD, Y_ANGULAR_RATIO);
        GLfloat z_rotation_angle = get_rotation_angle(Z_ROTATION_PERIOD, Z_ANGULAR_RATIO);
        glUniform1f(y_rotation_sin_uniform, sinf(y_rotation_angle));
        glUniform1f(y_rotation_cos_uniform, cosf(y_rotation_angle));
        glUniform1f(z_rotation_sin_uniform, sinf(z_rotation_angle));
        glUniform1f(z_rotation_cos_uniform, cosf(z_rotation_angle));

        glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT);

        main_window.swap();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
    }
}
