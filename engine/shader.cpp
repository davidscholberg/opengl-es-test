#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "engine/shader.hpp"
#include "utils/utils.hpp"

shader::shader(GLenum shader_type, const char *shader_path) {
    this->shader_id = glCreateShader(shader_type);

    auto shader_source = get_file_contents(shader_path);
    const char *shader_source_str = shader_source->c_str();
    glShaderSource(this->shader_id, 1, &shader_source_str, NULL);

    glCompileShader(this->shader_id);
    GLint shader_status;
    glGetShaderiv(this->shader_id, GL_COMPILE_STATUS, &shader_status);
    if (shader_status == GL_FALSE) {
        GLint info_log_length;
        glGetShaderiv(this->shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<GLchar> info_log(info_log_length + 1);
        glGetShaderInfoLog(this->shader_id, info_log_length, NULL, &info_log[0]);
        throw std::runtime_error(
                "glCompileShader error in "
                + std::string(shader_path)
                + ": " + std::string(&info_log[0])
        );
    }
}

shader::~shader() {
    glDeleteShader(this->shader_id);
}

GLuint shader::get_shader_id() {
    return this->shader_id;
}
