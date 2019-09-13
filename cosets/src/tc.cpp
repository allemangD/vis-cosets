#include <iostream>
#include <vector>

template<int N>
struct Row;

template<int N>
struct Table;

template<int N>
struct Row {
   std::vector<int>::const_iterator l;
   std::vector<int>::const_iterator r;

   int from;
   int to;

   Row(const std::vector<int> &vec, int from, int to) {
      this->l = vec.begin();
      this->r = vec.end() - 1;
      this->from = from;
      this->to = to;
   }

   bool learn(Table<N> &table) {
      if (r - l == 0) {
         return false;
      }

      while (r - l > 0) {
         int next = table.get(from, *l);
         if (next < 0) break;
         l++;
         from = next;
      }

      while (r - l > 0) {
         int next = table.rget(*r, to);
         if (next < 0) break;
         r--;
         to = next;
      }

      if (r - l == 0) {
         table.set(from, *l, to);
         return true;
      }

      return false;
   }
};

template<int N>
struct Table {
   std::vector<std::vector<int>> fwd;
   std::vector<std::vector<int>> rev;

   Table() {
      add_row();
   }

   void add_row() {
      fwd.push_back(std::vector<int>(N, -1));
      rev.push_back(std::vector<int>(N, -1));
   }

   int add_coset() {
      for (int from = 0; from < size(); ++from) {
         for (int gen = 0; gen < N; ++gen) {
            if (get(from, gen) < 0) {
               int to = size();
               add_row();
               set(from, gen, to);
               return to;
            }
         }
      }
      return 0;
   }

   [[nodiscard]] unsigned size() const {
      return fwd.size();
   }

   void set(int from, int gen, int to) {
      fwd[from][gen] = to;
      rev[to][gen] = from;
   }

   [[nodiscard]] int get(int from, int gen) const {
      return fwd[from][gen];
   }

   [[nodiscard]] int rget(int gen, int to) const {
      return rev[to][gen];
   }
};

template<int N>
std::ostream &operator<<(std::ostream &out, const Row<N> &row) {
   out << "[ " << row.from << " | ";
   auto it = row.l;
   while (row.r - it > 0) {
      out << *it << " ";
      it++;
   }
   out << "| " << row.to << " ]";
   return out;
}

template<int N>
std::ostream &operator<<(std::ostream &out, const Table<N> &table) {
   out << "[";
   for (unsigned j = 0; j < table.size(); ++j) {
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
   std::vector<std::vector<int>> ids{
      {0, 0},
      {1, 1},
      {0, 1, 0, 1, 0, 1}
   };

   Row<3> row(ids[2], 0, 0);
   Table<3> table;

   table.add_row();
   table.add_row();
   table.add_row();

   table.set(0, 0, 1);
   table.set(1, 1, 2);

   std::cout << table << std::endl;
   std::cout << row << std::endl;

   row.learn(table);
   std::cout << row << std::endl;

//   std::cout << table << std::endl;
//   table.add_coset();
//   std::cout << table << std::endl;
//
//   row.learn(table);

   return 0;
}
