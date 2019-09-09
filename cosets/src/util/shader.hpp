#pragma once

#include <glad/glad.h>
#include <string>
#include <cstdio>
#include <cstdlib>

GLint build_shader(GLenum type, const std::string &name, const std::string &src) {
   const char *c_str = src.c_str();

   auto s = glCreateShader(type);
   glShaderSource(s, 1, &c_str, nullptr);
   glCompileShader(s);

   GLint stat;
   glGetShaderiv(s, GL_COMPILE_STATUS, &stat);
   if (!stat) {
      GLint log_len;
      glGetShaderiv(s, GL_INFO_LOG_LENGTH, &log_len);
      char log[log_len];
      glGetShaderInfoLog(s, log_len, nullptr, log);
      fprintf(stderr, "SHADER LOG (%s):\n%s", name.c_str(), log);

      glDeleteShader(s);
      return 0;
   }

   return s;
}

GLint build_program(const std::string &name, GLint vs, GLint fs) {
   GLint p = glCreateProgram();
   glAttachShader(p, vs);
   glAttachShader(p, fs);
   glLinkProgram(p);

   GLint stat;
   glGetProgramiv(p, GL_LINK_STATUS, &stat);
   if (!stat) {
      GLint log_len;
      glGetShaderiv(p, GL_INFO_LOG_LENGTH, &log_len);
      char log[log_len];
      glGetProgramInfoLog(p, log_len, nullptr, log);
      fprintf(stderr, "SHADER LOG (%s):\n%s", name.c_str(), log);

      glDeleteProgram(p);
      return 0;
   }

   return p;
}
