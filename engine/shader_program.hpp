#ifndef SHADER_PROGRAM_HPP_
#define SHADER_PROGRAM_HPP_

#include <memory>
#include <vector>

#include <SDL2/SDL_opengles2.h>

#include "engine/shader.hpp"

class shader_program {
protected:
    GLuint program_id;
public:
    shader_program(std::unique_ptr<std::vector<std::unique_ptr<shader>>> shaders);
    shader_program(shader_program const &) = delete;
    ~shader_program();
    void operator=(shader_program const &) = delete;
    void use();
    void clear();
    GLuint get_attrib_location(const char *attrib);
};


#endif // SHADER_PROGRAM_HPP_
