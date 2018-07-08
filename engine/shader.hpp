#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <stdint.h>

#include <SDL2/SDL_opengles2.h>

class shader {
protected:
    uint32_t shader_id;
public:
    shader(GLenum shader_type, const char *shader_source);
    shader(shader const &) = delete;
    ~shader();
    void operator=(shader const &) = delete;
    uint32_t get_shader_id();
};

#endif // SHADER_HPP_
