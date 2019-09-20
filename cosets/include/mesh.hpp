#pragma once

#include <vector>
#include <chrono>
#include <iostream>

#include "coxeter.hpp"
#include "mirrors.hpp"
#include "numeric.hpp"

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


struct Buffer {
    GLuint name = 0;
    unsigned size = 0;
};

struct Mesh {
    const Mults mults;
    const Table *t_vert;

    const std::vector<glm::vec4> normals;
    glm::vec4 *vert_data;

    Buffer vert;
    Buffer edge;
    Buffer face;

    explicit Mesh(const Mults &mults) : mults(mults), normals(mirror(mults)) {
        auto a = std::chrono::high_resolution_clock::now();
        t_vert = mults.isolve({});
        auto b = std::chrono::high_resolution_clock::now();
        auto delta = (std::chrono::duration<double>) (b - a);
        std::cout << "time to solve verts: " << delta.count() << std::endl;

        vert.size = t_vert->size();
        vert_data = new glm::vec4[vert.size];

        glGenBuffers(1, &vert.name);
        glGenBuffers(1, &edge.name);
        glGenBuffers(1, &face.name);

        gen_indices();
    }

    void gen_vertices(const glm::vec4 root) {
        auto a = std::chrono::high_resolution_clock::now();
        vert_data[0] = root;
        for (unsigned from = 0; from < vert.size; ++from) {
            for (unsigned gen = 0; gen < mults.dim; ++gen) {
                int to = t_vert->iget((int) from, (int) gen);
                vert_data[to] = reflect(vert_data[from], normals[gen]);
            }
        }
        auto b = std::chrono::high_resolution_clock::now();
        auto delta = (std::chrono::duration<double>) (b - a);
        std::cout << "time to build verts: " << delta.count() << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, vert.name);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vert.size, vert_data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void gen_indices() {
        std::vector<int> edge_data;

        auto e_table = mults.sub({0, 1}).isolve({});

//        std::cout << "building gens" << std::endl;
//        const std::vector<int> gens{0, 1};
//        std::cout << "built gens" << std::endl;
//        const std::vector<int> &igens = gens;
//        Mults<2> res{};
//        for (int a = 0; a < 2; ++a) {
//            for (int b = a + 1; b < 2; ++b) {
//                res.set(a, b, t_vert->mults.get(igens[a], igens[b]));
//            }
//        }
//        const auto &mults = res;
//        std::cout << "built mults" << std::endl;


//        auto *e_table = mults.isolve({});
//        std::cout << "built table" << std::endl;
//        std::cout << "edge verts:" << e_table->size() << std::endl;
//
////        for (int i = 0; i < e_table->size(); ++i) {
////            edge_data.push_back(i);
////        }
//
//        delete e_table;

        edge.size = edge_data.size();
        glBindBuffer(GL_ARRAY_BUFFER, edge.name);
        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * edge.size, &edge_data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~Mesh() {
        delete t_vert;
        delete[] vert_data;

//        glDeleteBuffers(1, &vert.name);
//        glDeleteBuffers(1, &edge.name);
//        glDeleteBuffers(1, &face.name);
    }
};

std::ostream &operator<<(std::ostream &out, Buffer b);
