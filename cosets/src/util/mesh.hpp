#pragma once

#include "coxeter.hpp"
#include "mirrors.hpp"
#include "numeric.hpp"

#include <vector>

template<int N>
std::vector<std::vector<int>> coxeter_rels() {
   std::vector<std::vector<int>> rels{};

   return rels;
}

template<int N>
glm::vec4 identity(const std::vector<glm::vec4> &normals, const float(&coords)[N]) {
   const std::vector<glm::vec4> corners = plane_intersections(normals);
   const std::vector<float> coords_vec(coords, coords + N);
   const glm::vec4 identity = barycentric(corners, coords_vec);
   return glm::normalize(identity);
}

template<int N>
std::vector<glm::vec4>
vertices(const Mults<N> &mults, const float (&coords)[N]) {
   Table *table = solve({}, mults);

   const std::vector<glm::vec4> normals = mirror<N>(mults);
   glm::vec4 ident = identity(normals, coords);

   std::vector<glm::vec4> verts{};
   for (const auto &word : table->words()) {
      glm::vec4 vert = ident;
      for (const auto &gen : word) {
         vert = reflect(vert, normals[gen]);
      }
      verts.push_back(vert);
   }

   return verts;
}

template<int N>
std::vector<unsigned> lines(const Mults<N> &mults) {
//   Table *table = solve(N, {}, coxeter_rels(mults));

   return {};
}

