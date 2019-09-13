#include <iostream>
#include <vector>

struct Row;

template<int N>
struct Table;

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

template<int N>
struct Table {
   std::vector<std::vector<int>> fwd;
   std::vector<std::vector<int>> rev;

   void add_row() {
      fwd.push_back(std::vector<int>(N));
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

template<int N>
std::ostream &operator<<(std::ostream &out, const Table<N> &table) {
   out << "[";
   for (int j = 0; j < table.fwd.size(); ++j) {
      auto arr = table.fwd[j];
      out << "[";
      for (int i = 0; i < N; ++i) {
         out << arr[i];

         if (i < N - 1)
            out << " ";
      }

      out << "]";
      if (j < table.fwd.size() - 1)
         out << "\n ";
   }
   out << "]\n";

   return out;
}

int main(int argc, char *argv[]) {
   int gens = 2;
   std::vector<std::vector<int>> ids{
      {0, 0},
      {1, 1},
      {0, 1, 0, 1, 0, 1}
   };

   Table<3> table;
   table.add_row();
   table.add_row();
   table.add_row();
   table.add_row();
   std::cout << table;

   return 0;
}