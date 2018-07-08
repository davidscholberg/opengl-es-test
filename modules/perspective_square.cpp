#include <list>
#include <memory>
#include <vector>

#include <math.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"
#include "modules/perspective_square.hpp"

namespace perspective_square {
    const std::string module_name("perspective_square");

    const char *vertex_shader_source = R"glsl(
#version 100

attribute vec4 position;
attribute vec4 color;

varying vec4 fragment_color;

uniform float y_rotation_sin;
uniform float y_rotation_cos;
uniform float z_rotation_sin;
uniform float z_rotation_cos;

uniform vec3 offset;
uniform float frustum_scale;
uniform float z_mapping_factor;
uniform float z_mapping_offset;

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

    vec4 camera_position = vec4(
        (position.xyz * z_rotation * y_rotation) + offset,
        position.w
    );
    gl_Position = vec4(
        camera_position.xy * frustum_scale,
        (camera_position.z * z_mapping_factor) + z_mapping_offset,
        -camera_position.z
    );
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
    const int vertex_count = 4;

    const float y_rotation_period = 5.0f;
    const float z_rotation_period = 1.0f;
    const float y_angular_ratio = M_PI * 2.0f / y_rotation_period;
    const float z_angular_ratio = M_PI * 2.0f / z_rotation_period;

    const float frustum_scale = 1.0f;
    const float z_near = 1.0f;
    const float z_far = 3.0f;
    const float z_mapping_factor = (z_near + z_far) / (z_near - z_far);
    const float z_mapping_offset = (2 * z_near * z_far) / (z_near - z_far);

    float get_rotation_angle(float rotation_period, float angular_ratio) {
        float elapsed_time = SDL_GetTicks() / 1000.0f;
        float elapsed_period = fmodf(elapsed_time, rotation_period);
        return angular_ratio * elapsed_period;
    }

    int run(int argc, char **argv) {
        window main_window;

        std::list<shader> shaders;
        shaders.emplace_back(GL_VERTEX_SHADER, vertex_shader_source);
        shaders.emplace_back(GL_FRAGMENT_SHADER, fragment_shader_source);
        shader_program main_program(shaders);

        std::vector<float> square_vertex_vector {
            0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f
        };
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

        uint32_t y_rotation_sin_uniform = main_program.get_uniform_location("y_rotation_sin");
        uint32_t y_rotation_cos_uniform = main_program.get_uniform_location("y_rotation_cos");
        uint32_t z_rotation_sin_uniform = main_program.get_uniform_location("z_rotation_sin");
        uint32_t z_rotation_cos_uniform = main_program.get_uniform_location("z_rotation_cos");

        uint32_t offset_uniform = main_program.get_uniform_location("offset");
        uint32_t frustum_scale_uniform = main_program.get_uniform_location("frustum_scale");
        uint32_t z_mapping_factor_uniform = main_program.get_uniform_location("z_mapping_factor");
        uint32_t z_mapping_offset_uniform = main_program.get_uniform_location("z_mapping_offset");

        glUniform3f(offset_uniform, 0.0f, 0.0f, -2.0f);
        glUniform1f(frustum_scale_uniform, frustum_scale);
        glUniform1f(z_mapping_factor_uniform, z_mapping_factor);
        glUniform1f(z_mapping_offset_uniform, z_mapping_offset);

        SDL_Event event;
        bool done = false;
        while (!done) {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            float y_rotation_angle = get_rotation_angle(y_rotation_period, y_angular_ratio);
            float z_rotation_angle = get_rotation_angle(z_rotation_period, z_angular_ratio);
            glUniform1f(y_rotation_sin_uniform, sinf(y_rotation_angle));
            glUniform1f(y_rotation_cos_uniform, cosf(y_rotation_angle));
            glUniform1f(z_rotation_sin_uniform, sinf(z_rotation_angle));
            glUniform1f(z_rotation_cos_uniform, cosf(z_rotation_angle));

            glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count);

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
