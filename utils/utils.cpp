#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include <GLES2/gl2.h>

#include "utils/utils.hpp"

std::unique_ptr<std::string> get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::unique_ptr<std::string> contents (new std::string);
        std::string& contents_ref = *contents;
        in.seekg(0, std::ios::end);
        contents_ref.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents_ref[0], contents_ref.size());
        in.close();
        return contents;
    }
    throw std::runtime_error("couldn't open file \"" + std::string(filename) + "\"");
}

std::unique_ptr<std::string> get_gl_error_str(const char *function_name) {
    std::string error_str;
    bool done = false;
    while (!done) {
        GLenum error_code = glGetError();
        switch (error_code) {
            case GL_NO_ERROR:
                done = true;
                break;
            case GL_INVALID_ENUM:
                error_str += " invalid enum.";
                break;
            case GL_INVALID_VALUE:
                error_str += " invalid value.";
                break;
            case GL_INVALID_OPERATION:
                error_str += " invalid operation.";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error_str += " invalid framebuffer operation.";
                break;
            case GL_OUT_OF_MEMORY:
                error_str += " out of memory.";
                break;
            default:
                error_str += " unknown error code " + error_code + std::string(".");
        }
    }
    if (!error_str.empty()) {
        error_str = "error in " + std::string(function_name) + ":" + error_str;
    }
    return std::make_unique<std::string>(error_str);
}
