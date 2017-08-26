#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <GLES2/gl2.h>

class shader {
protected:
    GLuint shader_id;
public:
    shader(GLenum shader_type, const char *shader_path);
    shader(shader const &) = delete;
    ~shader();
    void operator=(shader const &) = delete;
    GLuint get_shader_id();
};

#endif // SHADER_HPP_
