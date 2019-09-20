#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <cmath>

#include "coxeter.hpp"
#include "numeric.hpp"

std::vector<glm::vec4> mirror(const Mults &mults) {
    std::vector<glm::vec4> mirrors{};
    for (int p = 0; p < mults.dim; ++p) {
        glm::vec4 vp{};
        for (int m = 0; m < p; ++m) {
            glm::vec4 vq = mirrors[m];
            vp[m] = (cos(M_PI / mults.get(p, m)) - dot(m, vp, vq)) / vq[m];
        }
        vp[p] = std::sqrt(1 - glm::dot(vp, vp));

        for (const auto &v : mirrors) {
            if (glm::dot(vp, v) > 0) {
                vp *= -1;
                break;
            }
        }

        mirrors.push_back(round(vp, 15));
    }

    return mirrors;
}
