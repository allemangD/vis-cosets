#pragma once

#include <glad/glad.h>
#include <string>

GLint build_shader(GLenum type, const std::string &name, const std::string &src);

GLint build_shader_file(GLenum type, const std::string &name, const std::string &file);

GLint build_program(const std::string &name, GLint vs, GLint fs);
