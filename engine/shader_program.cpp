#include <stdexcept>
#include <string>

#include "engine/shader_program.hpp"

shader_program::shader_program(std::unique_ptr<std::vector<std::unique_ptr<shader>>> shaders) {
    this->program_id = glCreateProgram();

    for(auto &shader : *shaders) {
        glAttachShader(this->program_id, shader->get_shader_id());
    }

    glLinkProgram(this->program_id);

    for(auto &shader : *shaders) {
        glDetachShader(this->program_id, shader->get_shader_id());
    }

    GLint program_status;
    glGetProgramiv(this->program_id, GL_LINK_STATUS, &program_status);
    if (program_status == GL_FALSE) {
        GLint info_log_length;
        glGetProgramiv(this->program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<GLchar> info_log(info_log_length + 1);
        glGetProgramInfoLog(this->program_id, info_log_length, NULL, &info_log[0]);
        throw std::runtime_error("glLinkProgram error: " + std::string(&info_log[0]));
    }
}

shader_program::~shader_program() {
    glDeleteProgram(this->program_id);
}

void shader_program::use() {
    glUseProgram(this->program_id);
}

void shader_program::clear() {
    glUseProgram(0);
}

GLuint shader_program::get_attrib_location(const char *attrib) {
    return glGetAttribLocation(this->program_id, attrib);
}

GLuint shader_program::get_uniform_location(const char *uniform) {
    return glGetUniformLocation(this->program_id, uniform);
}
