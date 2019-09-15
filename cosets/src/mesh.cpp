#include "util/mesh.hpp"

int main(int argc, char *argv[]) {
   const int N = 3;
   const Mults &mults = schlafli<N>({4, 3});
   const std::vector<glm::vec4> &vs = vertices<N>(mults, {10, 1, 1});

   std::cout << "# verts: " << vs.size() << std::endl;

   const auto es = edges<N>(mults);


   return 0;
}
