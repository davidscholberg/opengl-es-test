#ifndef UTILS_HPP_
#define UTILS_HPP_

std::unique_ptr<std::string> get_file_contents(const char *filename);
std::unique_ptr<std::string> get_gl_error_str(const char *function_name);

#endif // UTILS_HPP_
