#include <memory>
#include <vector>

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"

struct mat4 {
    GLfloat c1_r1;
    GLfloat c1_r2;
    GLfloat c1_r3;
    GLfloat c1_r4;
    GLfloat c2_r1;
    GLfloat c2_r2;
    GLfloat c2_r3;
    GLfloat c2_r4;
    GLfloat c3_r1;
    GLfloat c3_r2;
    GLfloat c3_r3;
    GLfloat c3_r4;
    GLfloat c4_r1;
    GLfloat c4_r2;
    GLfloat c4_r3;
    GLfloat c4_r4;
};

const char *vertex_shader_source = R"glsl(
#version 100

attribute vec4 position;
attribute vec4 color;

varying vec4 fragment_color;

uniform mat4 y_rotation_matrix;
uniform mat4 z_rotation_matrix;

uniform vec4 offset;

uniform mat4 perspective_matrix;

void main() {
    fragment_color = color;

    vec4 camera_position = (y_rotation_matrix * (z_rotation_matrix * position)) + offset;

    gl_Position = perspective_matrix * camera_position;
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

const GLfloat y_rotation_period = 60.0f;
const GLfloat z_rotation_period = 12.0f;
const GLfloat y_angular_ratio = M_PI * 2.0f / y_rotation_period;
const GLfloat z_angular_ratio = M_PI * 2.0f / z_rotation_period;

const GLfloat frustum_scale = 1.0f;
const GLfloat z_near = 1.0f;
const GLfloat z_far = 3.0f;
const GLfloat z_mapping_factor = (z_near + z_far) / (z_near - z_far);
const GLfloat z_mapping_offset = (2 * z_near * z_far) / (z_near - z_far);

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
            -0.5f, 0.5f, 0.5f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f,
            -0.5f, -0.5f, 0.5f, 1.0f,

            -0.5f, -0.5f, 0.5f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f,

            -0.5f, 0.5f, -0.5f, 1.0f,
            -0.5f, 0.5f, 0.5f, 1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f,

            -0.5f, -0.5f, -0.5f, 1.0f,
            -0.5f, 0.5f, 0.5f, 1.0f,
            -0.5f, -0.5f, 0.5f, 1.0f,

            -0.5f, -0.5f, 0.5f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f,

            -0.5f, -0.5f, -0.5f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f,

            0.5f, 0.5f, -0.5f, 1.0f,
            -0.5f, 0.5f, -0.5f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f,

            0.5f, -0.5f, -0.5f, 1.0f,
            -0.5f, 0.5f, -0.5f, 1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f,

            0.5f, 0.5f, 0.5f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f,

            0.5f, -0.5f, 0.5f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f,

            -0.5f, 0.5f, -0.5f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f,
            -0.5f, 0.5f, 0.5f, 1.0f,

            -0.5f, 0.5f, 0.5f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f,

            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0F, 0.0f, 1.0f,

            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,

            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0F, 1.0f, 1.0f,

            1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,

            1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,

            0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,

            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,

            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
    });
    const int vertex_count = square_vertex_vector->size() / vertex_depth / 2;
    vertex_buffer square_vertices(std::move(square_vertex_vector));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    main_program.use();
    square_vertices.bind();
    GLint position_attrib = main_program.get_attrib_location("position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, vertex_depth, GL_FLOAT, GL_FALSE, 0, 0);
    GLint color_attrib = main_program.get_attrib_location("color");
    glEnableVertexAttribArray(color_attrib);
    glVertexAttribPointer(
            color_attrib,
            vertex_depth,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*) (sizeof(GLfloat) * vertex_depth * vertex_count));

    GLint offset_uniform = main_program.get_uniform_location("offset");
    glUniform4f(offset_uniform, 0.0f, 0.0f, -2.0f, 0.0f);

    const mat4 perspective_matrix = {
        frustum_scale, 0, 0, 0,
        0, frustum_scale, 0, 0,
        0, 0, z_mapping_factor, -1,
        0, 0, z_mapping_offset, 0,
    };
    GLint perspective_matrix_uniform = main_program.get_uniform_location("perspective_matrix");
    glUniformMatrix4fv(perspective_matrix_uniform, 1, GL_FALSE, (const GLfloat*) &perspective_matrix);

    GLint y_rotation_matrix_uniform = main_program.get_uniform_location("y_rotation_matrix");
    GLint z_rotation_matrix_uniform = main_program.get_uniform_location("z_rotation_matrix");

    SDL_Event event;
    bool done = false;
    while (!done) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat y_rotation_angle = get_rotation_angle(y_rotation_period, y_angular_ratio);
        GLfloat z_rotation_angle = get_rotation_angle(z_rotation_period, z_angular_ratio);
        GLfloat y_rotation_sin = sinf(y_rotation_angle);
        GLfloat y_rotation_cos = cosf(y_rotation_angle);
        GLfloat z_rotation_sin = sinf(z_rotation_angle);
        GLfloat z_rotation_cos = cosf(z_rotation_angle);
        const mat4 y_rotation_matrix = {
            y_rotation_cos, 0, -y_rotation_sin, 0,
            0, 1, 0, 0,
            y_rotation_sin, 0, y_rotation_cos, 0,
            0, 0, 0, 1,
        };
        const mat4 z_rotation_matrix = {
            z_rotation_cos, z_rotation_sin, 0, 0,
            -z_rotation_sin, z_rotation_cos, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
        glUniformMatrix4fv(y_rotation_matrix_uniform, 1, GL_FALSE, (const GLfloat*) &y_rotation_matrix);
        glUniformMatrix4fv(z_rotation_matrix_uniform, 1, GL_FALSE, (const GLfloat*) &z_rotation_matrix);

        glDrawArrays(GL_TRIANGLES, 0, vertex_count);

        main_window.swap();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
    }
}
