#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

glm::vec4 round(glm::vec4 f, int prec);

float dot(int n, glm::vec4 a, glm::vec4 b);

glm::vec4 project(glm::vec4 vec, glm::vec4 target);

glm::vec4 reflect(glm::vec4 vec, glm::vec4 axis);

glm::vec4 gram_schmidt_last(std::vector<glm::vec4> vecs);

glm::vec4 barycentric(std::vector<glm::vec4> basis, std::vector<float> coords);

std::vector<glm::vec4> plane_intersections(std::vector<glm::vec4> normals);


std::vector<std::vector<int>> combinations(int N, int K);

std::vector<std::vector<int>> combinations(std::vector<int> N, int K);
