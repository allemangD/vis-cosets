#include "util/coxeter.hpp"

int main(int argc, char *argv[]) {
   std::vector<std::vector<int>> ids{
      {0, 0},
      {1, 1},
      {2, 2},
      {0, 1, 0, 1, 0, 1, 0, 1},
      {1, 2, 1, 2, 1, 2},
      {0, 2, 0, 2}
   };

   Table *table = solve(3, {0, 1}, ids);

   std::cout << table->size() << std::endl;
   std::cout << *table << std::endl;

   for (const auto &v : table->words()) {
      std::cout << "[ ";
      for (auto e : v) {
         std::cout << e << " ";
      }
      std::cout << "]\n";
   }

   return 0;
}
