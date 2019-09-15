#include "util/mirrors.hpp"

int main(int argc, char *argv[]) {
   auto normals = mirror<3>(Multiplicities<3>({
      {0, 1, 4},
      {1, 2, 3}
   }));

   for (const auto &normal : normals) {
      std::cout << glm::to_string(normal) << std::endl;
   }

   return 0;
}