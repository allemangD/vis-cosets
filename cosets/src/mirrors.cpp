#include "util/mirrors.hpp"

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