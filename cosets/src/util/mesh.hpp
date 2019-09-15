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
vertices(const Mults &mults, const float (&coords)[N]) {
   Table *table = solve<N>({}, mults);

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
std::vector<int> edges(const Mults &mults) {
   std::vector<int> res{};

   Table *verts = solve<N>({}, mults);

   int K = 1;
   for (const auto &subgens : combinations(N, K)) {
      Table *edge = solve(subgens, {}, mults);

      std::vector<int> primitive = verts->apply_each(edge->words());

      Table *cosets = solve<N>(subgens, mults);

      for (const auto &coset : cosets->words()) {
         for (const auto &e : primitive) {
            res.push_back(verts->apply(e, coset));
         }
      }
   }

   return res;
}
