#pragma once

#include <map>
#include <vector>
#include <algorithm>

struct Mults;
struct Table;
struct IRow;

struct Mults {
   int num_gens;
   std::map<std::tuple<int, int>, int> mults;

   Mults();

   void set(int a, int b, int mult);

   [[nodiscard]] int get(int a, int b) const;

   [[nodiscard]] std::vector<int> relation(int a, int b) const;

   [[nodiscard]] std::vector<std::vector<int>> relations(const std::vector<int> &gens) const;

   [[nodiscard]] std::vector<std::vector<int>> irelations(const Table *table, const std::vector<int> &gens) const;

   [[nodiscard]] Table *solve(const std::vector<int> &subgens) const;
   [[nodiscard]] Table *solve(const std::vector<int> &gens, const std::vector<int> &subgens) const;
};

struct Table {
   const std::vector<int> gens;
   const Mults mults;
   std::vector<int> gen_inds;
   std::vector<std::vector<int>> fwd;
   std::vector<std::vector<int>> rev;

   explicit Table(const std::vector<int> &gens, Mults mults);

   [[nodiscard]] int gen_index(int gen) const;

   [[nodiscard]] std::vector<int> gen_index_each(std::vector<int> rel) const;

   void add_row();

   int add_coset();

   [[nodiscard]] unsigned size() const;

   void set(int from, int gen, int to);

   [[nodiscard]] int get(int from, int gen) const;

   [[nodiscard]] int rget(int gen, int to) const;

   void iset(int from, int igen, int to);

   [[nodiscard]] int iget(int from, int igen) const;

   [[nodiscard]] int irget(int igen, int to) const;

   [[nodiscard]] std::vector<std::vector<int>> words() const;

   [[nodiscard]] int apply(int e, const std::vector<int> &word) const;

   [[nodiscard]] int apply(const std::vector<int> &word) const;

   [[nodiscard]] std::vector<int> apply_each(int e, const std::vector<std::vector<int>> &words) const;

   [[nodiscard]] std::vector<int> apply_each(const std::vector<std::vector<int>> &words) const;
};

struct IRow {
   std::vector<int>::const_iterator l;
   std::vector<int>::const_iterator r;

   int from;
   int to;

   IRow(const std::vector<int> &rel, int cos);

   [[nodiscard]] bool learn(Table *table);
};

//Table *solve(const std::vector<int> &gens, const std::vector<int> &subgens, const Mults &mults);
//
//Table *solve(const std::vector<int> &subgens, const Mults &mults);
//
//Table *solve_elems(const Mults &mults);

Mults schlafli(const std::vector<int> &symbol);

std::vector<int> all_gens(int N);
