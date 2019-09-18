#include <glad/glad.h>

#include <utility>
#include <chrono>
#include "mesh.hpp"

glm::vec4 identity(const std::vector<glm::vec4> &normals, const std::vector<float> &coords) {
   const std::vector<glm::vec4> corners = plane_intersections(normals);
   const glm::vec4 identity = barycentric(corners, coords);
   return glm::normalize(identity);
}

glm::vec4 identity(const Mults &mults, const std::vector<float> &coords) {
   const std::vector<glm::vec4> normals = mirror(mults);
   return identity(normals, coords);
}

glm::vec4 center(const Mults &mults) {
   const std::vector<glm::vec4> normals = mirror(mults);
   const std::vector<glm::vec4> corners = plane_intersections(normals);
   std::vector<float> coords{};
   for (int i = 0; i < corners.size(); ++i) {
      const auto u = (i + 2) % corners.size();
      const auto v = (i + 1) % corners.size();
      const auto &d = corners[u] - corners[v];
      coords.push_back(glm::length(d));
   }
   const glm::vec4 identity = barycentric(corners, coords);
   return glm::normalize(identity);
}

Mesh::Mesh(Mults mults, const Table *t_vert, Buffer vert, Buffer edge, Buffer face)
   : mults(std::move(mults)), t_vert(t_vert), vert(vert), edge(edge), face(face) {
}

Mesh::~Mesh() {
   delete t_vert;

   glDeleteBuffers(1, &vert.name);
   glDeleteBuffers(1, &edge.name);
   glDeleteBuffers(1, &face.name);
}

Mesh *mesh(const Mults &mults, glm::vec4 ident) {
   GLuint buffers[3];
   glGenBuffers(3, buffers);

   Table *t_vert = mults.solve({});

   auto vert_data = vertices(t_vert, ident);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vert_data.size(), &vert_data[0], GL_STATIC_DRAW);
   Buffer vert{buffers[0], vert_data.size()};

   auto edge_data = edges(t_vert);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(int) * edge_data.size(), &edge_data[0], GL_STATIC_DRAW);
   Buffer edge{buffers[1], edge_data.size()};

   auto face_data = faces(t_vert);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(int) * face_data.size(), &face_data[0], GL_STATIC_DRAW);
   Buffer face{buffers[2], face_data.size()};

   return new Mesh(mults, t_vert, vert, edge, face);
}

std::vector<glm::vec4>
vertices(const Table *t_vert, const glm::vec4 ident) {
   const std::vector<glm::vec4> normals = mirror(t_vert->mults);

   std::vector<glm::vec4> verts{};
   for (const auto &word : t_vert->words()) {
      glm::vec4 vert = ident;
      for (const auto &gen : word) {
         vert = reflect(vert, normals[gen]);
      }
      verts.push_back(vert);
   }

   return verts;
}

std::vector<int> edges(const Table *t_vert) {
   std::vector<int> res{};

   Mults mults = t_vert->mults;
   int N = mults.num_gens;
   const std::vector<int> &gens = t_vert->gens;

   for (const auto &subgens : combinations(N, 1)) {
      Table *t_edge = mults.solve(subgens, {});;

      std::vector<int> edge = t_vert->apply_each(t_edge->words());

      Table *c_edge = mults.solve(gens, subgens);

      for (const auto &coset : c_edge->words()) {
         for (const auto &e : edge) {
            res.push_back(t_vert->apply(e, coset));
         }
      }

      delete t_edge;
      delete c_edge;
   }

   return res;
}

std::vector<int> faces(const Table *t_vert) {
   std::vector<int> res{};
   const Mults &mults = t_vert->mults;
   int N = mults.num_gens;
   const std::vector<int> &gens = t_vert->gens;

   // for each *kind* of face
   for (const auto &sg_face : combinations(N, 2)) {
      Table *cs_face = mults.solve(gens, sg_face);

      // for each *kind* of edge
      for (const auto &sg_edge : combinations(sg_face, 1)) {
         Table *cs_edge = mults.solve(sg_face, sg_edge);

         // find the vertices of that edge
         Table *t_edge = mults.solve(sg_edge, {});
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

               if (c_edge.size() & 1u)
                  std::swap(res[res.size() - 1], res[res.size() - 2]);

               unsigned ro_si1 = (sg_face[0] + sg_face[1]);
               unsigned flag = sg_edge[0] == sg_face[0];
               unsigned n_mirrors = c_face.size();

               if ((n_mirrors + flag + ro_si1) & 1u) {
                  std::swap(res[res.size() - 1], res[res.size() - 2]);
               }
            }
         }

         delete t_edge;
         delete cs_edge;
      }

      delete cs_face;
   }

   return res;
}
