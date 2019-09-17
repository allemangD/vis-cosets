#include "coxeter.hpp"

Mults::Mults() {
   num_gens = 0;
   mults = std::map<std::tuple<int, int>, int>();
}

void Mults::set(int a, int b, int mult) {
   if (a > b) std::swap(a, b);
   if (b + 1 > num_gens) num_gens = b + 1;
   mults[std::make_tuple(a, b)] = mult;
}

int Mults::get(int a, int b) const {
   if (a == b) return 1;
   if (a > b) std::swap(a, b);

   const auto &tup = std::make_tuple(a, b);
   const auto &res = mults.find(tup);

   if (res == mults.end()) return 2;
   return res->second;
}

std::vector<int> Mults::relation(int a, int b) const {
   std::vector<int> res{};
   int mult = get(a, b);

   for (int i = 0; i < mult; ++i) {
      res.push_back(a);
      res.push_back(b);
   }

   return res;
}

std::vector<std::vector<int>> Mults::relations(const std::vector<int> &gens) const {
   std::vector<std::vector<int>> res{};
   for (int a = 0; a < (int) gens.size(); ++a) {
      for (int b = a; b < (int) gens.size(); ++b) {
         res.push_back(relation(gens[a], gens[b]));
      }
   }
   return res;
}

std::vector<std::vector<int>> Mults::irelations(const Table *table, const std::vector<int> &gens) const {
   std::vector<std::vector<int>> rels = relations(gens);
   std::vector<std::vector<int>> irels(rels.size());
   for (int i = 0; i < (int) rels.size(); ++i) {
      irels[i] = table->gen_index_each(rels[i]);
   }
   return irels;
}

Table::Table(const std::vector<int> &gens, const Mults mults) : gens(gens), mults(mults) {
   add_row();

   gen_inds = std::vector<int>(gens[gens.size() - 1] + 1, -1);
   for (int i = 0; i <(int) gens.size(); i++) {
      gen_inds[gens[i]] = i;
   }
}

int Table::gen_index(int gen) const {
   if (gen >= gen_inds.size())
      throw std::logic_error("Referencing nonexistant generator (too large)");
   int i = gen_inds[gen];
   if (i < 0)
      throw std::logic_error("Referencing nonexistant generator (not present)");
   return i;
}

std::vector<int> Table::gen_index_each(std::vector<int> rel) const {
   std::vector<int> res(rel.size(), 0);
   for (int i = 0; i < (int)rel.size(); ++i) {
      res[i] = gen_index(rel[i]);
   }
   return res;
}

void Table::add_row() {
   int N = gens.size();
   fwd.emplace_back(N, -1);
   rev.emplace_back(N, -1);
}

int Table::add_coset() {
   for (int from = 0; from < (int) size(); ++from) {
      for (int igen = 0; igen < (int) gens.size(); igen++) {
         if (iget(from, igen) < 0) {
            int to = (int) size();
            add_row();
            iset(from, igen, to);
            return to;
         }
      }
   }
   return 0;
}

unsigned Table::size() const {
   return fwd.size();
}

void Table::set(int from, int gen, int to) {
   int i = gen_index(gen);
   fwd[from][i] = to;
   rev[to][i] = from;
}

int Table::get(int from, int gen) const {
   int i = gen_index(gen);
   return fwd[from][i];
}

int Table::rget(int gen, int to) const {
   int i = gen_index(gen);
   return rev[to][i];
}

void Table::iset(int from, int igen, int to) {
   fwd[from][igen] = to;
   rev[to][igen] = from;
}

int Table::iget(int from, int igen) const {
   return fwd[from][igen];
}

int Table::irget(int igen, int to) const {
   return rev[to][igen];
}

std::vector<std::vector<int>> Table::words() const {
   std::vector<std::vector<int> *> vecs(size());
   vecs[0] = new std::vector<int>();

   while (std::find(vecs.begin(), vecs.end(), nullptr) != vecs.end()) {
      for (int from = 0; from < (int) vecs.size(); ++from) {
         std::vector<int> *word = vecs[from];
         if (word == nullptr) {
            continue;
         }

         for (int igen = 0; igen < gens.size(); igen++) {
            int to = iget(from, igen);
            if (vecs[to] != nullptr) {
               continue;
            }

            vecs[to] = new std::vector<int>(*word);
            vecs[to]->push_back(gens[igen]);
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

int Table::apply(int e, const std::vector<int> &word) const {
   for (const auto &gen : word) {
      e = get(e, gen);
   }
   return e;
}

int Table::apply(const std::vector<int> &word) const {
   return apply(0, word);
}

std::vector<int> Table::apply_each(int e, const std::vector<std::vector<int>> &words) const {
   std::vector<int> res{};

   for (const auto &word:words) {
      res.push_back(apply(e, word));
   }

   return res;
}

std::vector<int> Table::apply_each(const std::vector<std::vector<int>> &words) const {
   return apply_each(0, words);
}

IRow::IRow(const std::vector<int> &rel, int cos) {
   this->l = rel.begin();
   this->r = rel.end() - 1;
   this->from = cos;
   this->to = cos;
}

bool IRow::learn(Table *table) {
   if (r - l == 0) {
      return false;
   }

   while (r - l > 0) {
      int next = table->iget(from, *l);
      if (next < 0) break;
      l++;
      from = next;
   }

   while (r - l > 0) {
      int next = table->irget(*r, to);
      if (next < 0) break;
      r--;
      to = next;
   }

   if (r - l == 0) {
      table->iset(from, *l, to);
      return true;
   }

   return false;
}

Table *solve(const std::vector<int> &gens, const std::vector<int> &subgens, const Mults &mults) {
   auto *table = new Table(gens, mults);
   const auto irels = mults.irelations(table, gens);

   for (int gen : subgens)
      table->set(0, gen, 0);

   std::vector<IRow> irows;
   irows.reserve(irels.size());
   for (const auto &irel : irels)
      irows.emplace_back(irel, 0);

   while (!irows.empty()) {
      while (true) {
         bool learned = false;
         for (int i = (int) irows.size() - 1; i >= 0; i--) {
            if (irows[i].learn(table)) {
               learned = true;
               irows.erase(irows.begin() + i);
            }
         }
         if (!learned)
            break;
      }

      int i = (int) table->size();
      if (table->add_coset() > 0) {
         for (const auto &irel : irels)
            irows.emplace_back(irel, i);
      } else {
         break;
      }
   }

   return table;
}

Table *solve(const std::vector<int> &subgens, const Mults &mults) {
   return solve(all_gens(mults.num_gens), subgens, mults);
}

Table *solve_elems(const Mults &mults) {
   return solve({}, mults);
}

Mults schlafli(const std::vector<int> &symbol) {
   Mults mults{};
   for (int i = 0; i < symbol.size(); ++i) {
      mults.set(i, i + 1, symbol[i]);
   }
   return mults;
}

std::vector<int> all_gens(int N) {
   std::vector<int> gens(N);
   for (int i = 0; i < N; ++i) {
      gens[i] = i;
   }
   return gens;
}
