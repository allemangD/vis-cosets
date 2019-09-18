#pragma once

#include <vector>

#include "coxeter.hpp"
#include "mirrors.hpp"
#include "numeric.hpp"

glm::vec4 identity(const std::vector<glm::vec4> &normals, const std::vector<float> &coords);

glm::vec4 identity(const Mults &mults, const std::vector<float> &coords);

glm::vec4 center(const Mults &mults);

std::vector<glm::vec4> vertices(const Table *t_vert, const glm::vec4 ident);

std::vector<int> edges(const Table *t_vert);

std::vector<int> faces(const Table *t_vert);


struct Buffer {
   const GLuint name;
   const unsigned size;
   const float gen_time;
};

struct Mesh {
   const Mults mults;
   const Table *t_vert;

   Buffer vert;
   Buffer edge;
   Buffer face;

   Mesh(Mults mults, const Table *t_vert, Buffer vert, Buffer edge, Buffer face);

   ~Mesh();
};

Mesh *mesh(const Mults &mults, glm::vec4 ident);

std::ostream &operator<<(std::ostream &out, Buffer b);
