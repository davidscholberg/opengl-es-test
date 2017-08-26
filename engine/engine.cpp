#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "engine/engine.hpp"
#include "engine/shader.hpp"
#include "engine/shader_program.hpp"
#include "engine/vertex_buffer.hpp"
#include "engine/window.hpp"

engine::engine() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
    }
}

engine::~engine() {
     SDL_Quit();
}

void engine::loop() {
    window main_window;

    auto shaders = std::make_unique<std::vector<std::unique_ptr<shader>>>();
    shaders->push_back(std::make_unique<shader>(GL_VERTEX_SHADER, "shaders/vertex_shader.glsl"));
    shaders->push_back(std::make_unique<shader>(GL_FRAGMENT_SHADER, "shaders/fragment_shader.glsl"));
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

        glDisableVertexAttribArray(position_attrib);
        main_program.clear();

        main_window.swap();

        SDL_Delay(10);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
    }
}
