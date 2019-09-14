#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <map>
#include <tuple>

template<int N>
struct Mults {
   std::map<std::tuple<int, int>, int> mults{};

   void set(int a, int b, int mult) {
      if (a > b) std::swap(a, b);
      mults[std::make_tuple(a, b)] = mult;
   }

   [[nodiscard]] int get(int a, int b) const {
      if (a == b) return 1;
      if (a > b) std::swap(a, b);

      const auto &tup = std::make_tuple(a, b);
      const auto &res = mults.find(tup);

      if (res == mults.end()) return 2;
      return res->second;
   }

   [[nodiscard]] std::vector<int> relation(int a, int b) const {
      std::vector<int> res{};
      int mult = get(a, b);

      for (int i = 0; i < mult; ++i) {
         res.push_back(a);
         res.push_back(b);
      }

      return res;
   }

   [[nodiscard]] std::vector<std::vector<int>> relations() const {
      std::vector<std::vector<int>> res{};
      for (int a = 0; a < N; ++a) {
         for (int b = a; b < N; ++b) {
            res.push_back(relation(a, b));
         }
      }
      return res;
   }
};

template<int N>
Mults<N> schlafli(const int (&symbol)[N - 1]) {
   Mults<N> mults{};
   for (int i = 0; i < N; ++i) {
      mults.set(i, i + 1, symbol[i]);
   }
   return mults;
}

struct Table {
   int N;
   std::vector<std::vector<int>> fwd{};
   std::vector<std::vector<int>> rev{};

   explicit Table(int N) {
      this->N = N;
      add_row();
   }

   void add_row() {
      fwd.emplace_back(N, -1);
      rev.emplace_back(N, -1);
   }

   int add_coset() {
      for (int from = 0; from < (int) size(); ++from) {
         for (int gen = 0; gen < N; ++gen) {
            if (get(from, gen) < 0) {
               int to = (int) size();
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

   std::vector<std::vector<int>> words() {
      std::vector<std::vector<int> *> vecs(size());
      vecs[0] = new std::vector<int>();

      while (std::find(vecs.begin(), vecs.end(), nullptr) != vecs.end()) {
         for (int from = 0; from < (int) vecs.size(); ++from) {
            std::vector<int> *word = vecs[from];
            if (word == nullptr) {
               continue;
            }

            for (int gen = 0; gen < (int) N; ++gen) {
               int to = get(from, gen);
               if (vecs[to] != nullptr) {
                  continue;
               }

               vecs[to] = new std::vector<int>(*word);
               vecs[to]->push_back(gen);
            }
         }
      }

      std::vector<std::vector<int>> res(size());
      for (int i = 0; i < (int) size(); ++i) {
         res[i] = *vecs[i];
         delete vecs[i];
      }

      return res;
   }
};

struct Row {
   std::vector<int>::const_iterator l;
   std::vector<int>::const_iterator r;

   int from;
   int to;

   Row(const std::vector<int> &vec, int cos) {
      this->l = vec.begin();
      this->r = vec.end() - 1;
      this->from = cos;
      this->to = cos;
   }

   [[nodiscard]] bool learn(Table *table) {
      if (r - l == 0) {
         return false;
      }

      while (r - l > 0) {
         int next = table->get(from, *l);
         if (next < 0) break;
         l++;
         from = next;
      }

      while (r - l > 0) {
         int next = table->rget(*r, to);
         if (next < 0) break;
         r--;
         to = next;
      }

      if (r - l == 0) {
         table->set(from, *l, to);
         return true;
      }

      return false;
   }
};

std::ostream &operator<<(std::ostream &out, const Row &row) {
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
Table *solve(const std::vector<int> &subgens, const Mults<N> &mults) {
   const auto rels = mults.relations();
   int gens = N;
   auto *table = new Table(gens);

   for (int gen : subgens)
      table->set(0, gen, 0);

   std::vector<Row> rows;
   rows.reserve(rels.size());
   for (const auto &rel : rels)
      rows.emplace_back(rel, 0);

   while (!rows.empty()) {
      while (true) {
         bool learned = false;
         for (int i = (int) rows.size() - 1; i >= 0; i--) {
            if (rows[i].learn(table)) {
               learned = true;
               rows.erase(rows.begin() + i);
            }
         }
         if (!learned)
            break;
      }

      int i = (int) table->size();
      if (table->add_coset() > 0) {
         for (const auto &rel : rels)
            rows.emplace_back(rel, i);
      } else {
         break;
      }
   }

   return table;
}

std::ostream &operator<<(std::ostream &out, const Table &table) {
   int k = ceil(log10(table.size()));

   out << "[";
   for (unsigned j = 0; j < table.size(); ++j) {
      auto arr = table.fwd[j];
      out << " " << std::setw(k) << j << " [";
      for (int i = 0; i < table.N; ++i) {
         out << arr[i];

         if (i < table.N - 1)
            out << " ";
      }

      out << "]";
      if (j < table.fwd.size() - 1)
         out << "\n ";
   }
   out << "]\n";

   return out;
}
