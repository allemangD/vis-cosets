#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>

glm::vec4 round(glm::vec4 f, int prec) {
   auto dec = (float) pow(10, prec);
   auto res = glm::trunc(f * dec + 0.5f) / dec;
   return res;
}

template<int N>
using vecn = glm::vec<N, float, glm::defaultp>;

float dot(int n, glm::vec4 a, glm::vec4 b) {
   float sum = 0;
   for (int i = 0; i < n; ++i) {
      sum += a[i] * b[i];
   }
   return sum;
}

template<int N>
std::vector<glm::vec4> mirror(const float (&arr)[N][N]) {
   static_assert(1 <= N and N <= 4, "Vector size is unsupported");

   std::vector<glm::vec4> mirrors{};
   for (int p = 0; p < N; ++p) {
      glm::vec4 vp{};
      for (int m = 0; m < p; ++m) {
         glm::vec4 vq = mirrors[m];
         vp[m] = (cos(M_PI / arr[p][m]) - dot(m, vp, vq)) / vq[m];
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

int main(int argc, char *argv[]) {
   auto normals = mirror<3>({
      {},
      {4},
      {2, 3}
   });

   for (const auto &normal : normals) {
      std::cout << glm::to_string(normal) << std::endl;
   }

   return 0;
}