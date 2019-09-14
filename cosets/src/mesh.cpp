#include "util/mesh.hpp"

int main(int argc, char *argv[]) {
   const auto &mults = schlafli<4>({4, 3, 3});
   const std::vector<glm::vec4> &vs = vertices<4>(mults, {10, 1, 1, 1});

   for (const auto &v:vs) {
      std::cout << glm::to_string(v) << std::endl;
   }

//   Table *table = solve({}, mults);
//   std::cout << table->size() << std::endl;
//
//   for (const auto &rel : mults.relations()) {
//      for (const auto &e : rel) {
//         std::cout << e << " ";
//      }
//      std::cout << std::endl;
//   }
}