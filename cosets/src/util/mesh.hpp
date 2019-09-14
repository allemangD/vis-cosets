#pragma once

#include "coxeter.hpp"
#include "mirrors.hpp"
#include "numeric.hpp"

#include <vector>

template<int N>
std::vector<glm::vec4>
vertices(const std::vector<int> &subgens, const Multiplicites<N> &mults, const float (&coords)[N]) {
   std::vector<std::vector<int>> rels{};
   rels.reserve(N + N * (N - 1) / 2);

   for (int i = 0; i < N; ++i) {
      rels.push_back({i, i});
   }
   for (int a = 0; a < N; ++a) {
      for (int b = 0; b < a; ++b) {
         int mult = mults.get(a, b);

         std::vector<int> rel{};
         for (int i = 0; i < mult; ++i) {
            rel.push_back(a);
            rel.push_back(b);
         }
         rels.push_back(rel);
      }
   }

   Table *table = solve(N, subgens, rels);

//   std::cout << table->size() << std::endl;
//   std::cout << *table << std::endl;

//   for (const auto &v : table->words()) {
//      std::cout << "[ ";
//      for (auto e : v) {
//         std::cout << e << " ";
//      }
//      std::cout << "]\n";
//   }

   const std::vector<glm::vec4> &normals = mirror<3>(mults);
   const std::vector<glm::vec4> &corners = plane_intersections(normals);
   const std::vector<float> coords_vec(coords, coords + N);
   const glm::vec4 &identity = glm::normalize(barycentric(corners, coords_vec));

   std::vector<glm::vec4> verts(table->size());

   for (const auto &word : table->words()) {
      glm::vec4 vert = identity;
      for (const auto &gen : word) {
         vert = reflect(vert, normals[gen]);
      }
      verts.push_back(vert);
   }

   return verts;
}