#include <exception>
#include <iostream>
#include <memory>
#include <vector>

#include "engine/engine.hpp"
#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"

const char *vertex_shader_source = R"glsl(
#version 100

attribute vec4 position;
void main() {
    gl_Position = vec4(position);
}
)glsl";

const char *fragment_shader_source = R"glsl(
#version 100

precision mediump float;
void main() {
   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)glsl";

void loop() {
    window main_window;

    auto shaders = std::make_unique<std::vector<std::unique_ptr<shader>>>();
    shaders->push_back(std::make_unique<shader>(GL_VERTEX_SHADER, vertex_shader_source));
    shaders->push_back(std::make_unique<shader>(GL_FRAGMENT_SHADER, fragment_shader_source));
    shader_program main_program(std::move(shaders));

    auto triangle_vertex_vector = std::make_unique<std::vector<GLfloat>, std::initializer_list<GLfloat>>({
            0.0f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f});
    vertex_buffer triangle_vertices(std::move(triangle_vertex_vector));

    SDL_Event event;
    bool done = false;
    while (!done) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        main_program.use();

        triangle_vertices.bind();
        GLuint position_attrib = main_program.get_attrib_location("position");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        main_window.swap();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
    }
}

int main(int argv, char** argc) {
    try {
        engine e;
        loop();
    } catch (const std::exception& exc) {
        std::cerr << "error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
