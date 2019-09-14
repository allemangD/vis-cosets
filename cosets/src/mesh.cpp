#include "util/mesh.hpp"

int main(int argc, char *argv[]) {
   auto vs = vertices<3>({}, Multiplicites<3>({
      {0, 1, 4},
      {1, 2, 3}
   }), {
      1.0f, 1.0f, 1.0f
   });

   for (const auto &v : vs) {
      std::cout << glm::to_string(v) << std::endl;
   }

   return 0;
}