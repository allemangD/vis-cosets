#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

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
std::vector<glm::vec4> mirror(const float (&arr)[N - 1][N - 1]) {
   static_assert(1 <= N and N <= 4, "Vector size is unsupported");

   std::vector<glm::vec4> mirrors{};
   for (int p = 0; p < N; ++p) {
      glm::vec4 vp{};
      for (int m = 0; m < p; ++m) {
         glm::vec4 vq = mirrors[m];
         float a = cos(M_PI / arr[p-1][m]);
         float b = dot(m, vp, vq);
         float c = vq[m];
         float d = a - b / c;
         vp[m] = d;
      }
      vp[p] = std::sqrt(1 - glm::dot(vp, vp));

      for (const auto &v : mirrors) {
         if (glm::dot(vp, v) > 0) {
            vp *= -1;
            break;
         }
      }

      mirrors[p] = vp;
   }

   return mirrors;
}

int main(int argc, char *argv[]) {
   int planes = 3;
   auto normals = mirror<3>((float[2][2]) {
      {4},
      {2, 3}
   });

   for (const auto &normal : normals) {
      std::cout << glm::to_string(normal) << std::endl;
   }

   return 0;
}