#include <iostream>
#include <vector>

struct Row {
   std::vector<int>::const_iterator l;
   std::vector<int>::const_iterator r;

   int from;
   int to;

   Row(const std::vector<int> &vec, int from, int to) {
      this->l = vec.begin();
      this->r = vec.end();
      this->from = from;
      this->to = to;
   }
};

std::ostream &operator<<(std::ostream &out, const Row &row) {
   out << "[ " << row.from << " | ";
   auto it = row.l;
   while (it != row.r) {
      out << *it << " ";
      it++;
   }
   out << "| " << row.to << " ]";
   return out;
}

int main(int argc, char *argv[]) {
   int gens = 2;
   std::vector<std::vector<int>> ids{
      {0, 0},
      {1, 1},
      {0, 1, 0, 1, 0, 1}
   };

   std::vector<int> &id = ids[2];
   Row row(id, 8, 9);

   std::cout << row << std::endl;

   row.l++;
   row.from = 6;

   std::cout << row << std::endl;

//   std::vector<std::vector<char>> vecs{
//      {'a', 'o', 'e', 'u'},
//      {'p'},
//      {'q', 'j', 'k'},
//   };
//
//   for (const auto& vec : vecs) {
//      for (const auto& ch : vec) {
//         std::cout << ch;
//      }
//      std::cout << std::endl;
//   }

   return 0;
}