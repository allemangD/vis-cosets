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


template<int NGENS>
glm::vec4 identity(const Mults<NGENS> &mults, const std::vector<float> &coords) {
    const std::vector<glm::vec4> normals = mirror(mults);
    return identity(normals, coords);
}


template<int NGENS>
glm::vec4 center(const Mults<NGENS> &mults) {
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

template<int NGENS>
struct Mesh {
    const Mults<NGENS> mults;
    const Table<NGENS> *t_vert;

    const std::vector<glm::vec4> normals;
    glm::vec4 *vert_data;

    Buffer vert;
    Buffer edge;
    Buffer face;

    explicit Mesh(Mults<NGENS> mults) : mults(mults), normals(mirror(mults)) {
        auto a = std::chrono::high_resolution_clock::now();
        t_vert = mults.isolve({});
        auto b = std::chrono::high_resolution_clock::now();
        std::cout << "time to solve: " << ((std::chrono::duration<double>) (b - a)).count() << std::endl;

        vert.size = t_vert->size();
        vert_data = new glm::vec4[vert.size];

        glGenBuffers(1, &vert.name);
        glGenBuffers(1, &edge.name);
        glGenBuffers(1, &face.name);

        gen_indices();
    }

    explicit Mesh(Mults<NGENS> mults, const glm::vec4 root) {
        this(mults);
        gen_vertices(root);
    }

    void gen_vertices(const glm::vec4 root) {
        vert_data[0] = root;
        for (unsigned from = 0; from < vert.size; ++from) {
            for (unsigned gen = 0; gen < NGENS; ++gen) {
                int to = t_vert->iget((int) from, (int) gen);
                vert_data[to] = reflect(vert_data[from], normals[gen]);
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, vert.name);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vert.size, vert_data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void gen_indices() {
        std::vector<int> edges;
        std::vector<int> faces;

        glBindBuffer(GL_ARRAY_BUFFER, edge.name);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * edge.size, edge_data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~Mesh() {
        delete t_vert;
        delete[] vert_data;

        glDeleteBuffers(1, &vert.name);
        glDeleteBuffers(1, &edge.name);
        glDeleteBuffers(1, &face.name);
    }
};

std::ostream &operator<<(std::ostream &out, Buffer b);
