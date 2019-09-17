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
