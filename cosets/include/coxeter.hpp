#pragma once

#include <map>
#include <vector>
#include <algorithm>

struct Mults;
struct Table;
struct IRow;

struct Mults {
    const int dim;
    std::map<std::tuple<int, int>, int> mults;

    explicit Mults(const int dim) : dim(dim) {
        mults = std::map<std::tuple<int, int>, int>();
    }

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

    [[nodiscard]] std::vector<int> irelation(int a, int b) const {
        std::vector<int> res{};
        int mult = get(a, b);

        for (int i = 0; i < mult; ++i) {
            res.push_back(a);
            res.push_back(b);
        }

        return res;
    }

    [[nodiscard]] Mults sub(const std::vector<int> &igens) const {
        Mults res(igens.size());
        for (int a = 0; a < (int) igens.size(); ++a) {
            for (int b = a + 1; b < (int) igens.size(); ++b) {
                res.set(a, b, get(igens[a], igens[b]));
            }
        }
        return res;
    }

    [[nodiscard]] Table *isolve(const std::vector<int> &isubgens) const;
};

struct Table {
    const Mults mults;
    std::vector<std::vector<int>> fwd;

    explicit Table(Mults mults) : mults(mults) {
        add_row();
    }

    void add_row() {
        fwd.emplace_back(mults.dim, -1);
    }

    int add_coset() {
        for (int from = 0; from < (int) size(); ++from) {
            for (int igen = 0; igen < (int) mults.dim; igen++) {
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

    [[nodiscard]] unsigned size() const {
        return fwd.size();
    }

    void iset(int from, int igen, int to) {
        fwd[from][igen] = to;
        fwd[to][igen] = from;
    }

    [[nodiscard]] int iget(int from, int igen) const {
        return fwd[from][igen];
    }

    [[nodiscard]] int irget(int igen, int to) const {
        return fwd[to][igen];
    }

};

struct IRow {
    std::vector<int>::const_iterator l;
    std::vector<int>::const_iterator r;

    int from;
    int to;

    IRow(const std::vector<int> &rel, int cos)
        : l(rel.begin()), r(rel.end() - 1), from(cos), to(cos) {
    }

    [[nodiscard]] bool learn(Table *table) {
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
};

Mults schlafli(const std::vector<int> &symbol) {
    unsigned int dim = symbol.size();
    Mults mults(dim + 1);
    for (int i = 0; i < dim; ++i) {
        mults.set(i, i + 1, symbol[i]);
    }
    return mults;
}

Table *Mults::isolve(const std::vector<int> &isubgens) const {
    auto *table = new Table(*this);
    for (int igen : isubgens)
        table->iset(0, igen, 0);

    std::vector<std::vector<int>> irels{};
    for (unsigned a = 0; a < dim; ++a) {
        for (unsigned b = a + 1; b < dim; ++b) {
            irels.push_back(irelation(a, b));
        }
    }

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

