#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

glm::vec4 round(glm::vec4 f, int prec) {
   auto dec = (float) pow(10, prec);
   auto res = glm::trunc(f * dec + 0.5f) / dec;
   return res;
}

float dot(int n, glm::vec4 a, glm::vec4 b) {
   float sum = 0;
   for (int i = 0; i < n; ++i) {
      sum += a[i] * b[i];
   }
   return sum;
}

glm::vec4 project(glm::vec4 vec, glm::vec4 target) {
   return glm::dot(vec, target) / glm::dot(target, target) * target;
}

glm::vec4 reflect(glm::vec4 vec, glm::vec4 axis) {
   return vec - 2.0f * project(vec, axis);
}

glm::vec4 gram_schmidt_last(std::vector<glm::vec4> vecs) {
   int N = vecs.size();
   for (int i = 0; i < N; ++i) {
      for (int j = 0; j < i; ++j) {
         vecs[i] -= project(vecs[i], vecs[j]);
      }
   }

   return glm::normalize(vecs[N - 1]);
}

glm::vec4 barycentric(std::vector<glm::vec4> basis, std::vector<float> coords) {
   glm::vec4 res{};
   int N = std::min(basis.size(), coords.size());
   for (int i = 0; i < N; ++i) {
      res += basis[i] * coords[i];
   }
   return res;
}

std::vector<glm::vec4> plane_intersections(std::vector<glm::vec4> normals) {
   int N = normals.size();
   std::vector<glm::vec4> results(N);

   for (int i = 0; i < N; ++i) {
      results[i] = gram_schmidt_last(normals);
      std::rotate(normals.begin(), normals.begin() + 1, normals.end());
   }

   return results;
}
