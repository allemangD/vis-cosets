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

   Table *t_vert = solve<N>({}, mults);

   for (const auto &subgens : combinations(N, 1)) {
      Table *t_edge = solve(subgens, {}, mults);

      std::vector<int> edge = t_vert->apply_each(t_edge->words());

      Table *c_edge = solve<N>(subgens, mults);

      for (const auto &coset : c_edge->words()) {
         for (const auto &e : edge) {
            res.push_back(t_vert->apply(e, coset));
         }
      }
   }

   return res;
}

template<int N>
std::vector<int> faces(const Mults &mults) {
   std::vector<int> res{};

   Table *t_vert = solve<N>({}, mults);

   // for each *kind* of face
   for (const auto &sg_face : combinations(N, 2)) {
      Table *cs_face = solve<N>(sg_face, mults);

      // for each *kind* of edge
      for (const auto &sg_edge : combinations(sg_face, 1)) {
         Table *cs_edge = solve(sg_face, sg_edge, mults);

         // find the vertices of that edge
         Table *t_edge = solve(sg_edge, {}, mults);
         std::vector<int> edge = t_vert->apply_each(t_edge->words());

         // for each face
         for (const auto &c_face : cs_face->words()) {
            // for each edge
            for (const auto &c_edge : cs_edge->words()) {
               if (c_edge.empty()) { continue; }

               for (auto e : edge) {
                  e = t_vert->apply(e, c_edge);
                  e = t_vert->apply(e, c_face);
                  res.push_back(e);
               }
               res.push_back(t_vert->apply(0, c_face));

               int s = 0;
               s += c_edge.size();
               s += c_face.size();
               s += sg_edge[0];
               s += sg_face[0] + sg_face[1];

               if (s % 2 == 0) {
                  std::swap(res[res.size() - 1], res[res.size() - 2]);
               }
            }
//            break;
         }
      }
   }

   return res;
}