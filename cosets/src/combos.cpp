#include <algorithm>
#include <iostream>
#include <vector>
#include <array>

template<class RandIt>
bool next_k_permutation(RandIt first, RandIt mid, RandIt last) {
    typedef typename std::iterator_traits<RandIt>::value_type value_type;

    std::sort(mid, last, std::greater<value_type>());
    return std::next_permutation(first, last);
}

template<class BiDiIt>
bool next_combination(BiDiIt first, BiDiIt mid, BiDiIt last) {
    bool result;
    do {
        result = next_k_permutation(first, mid, last);
    } while (std::adjacent_find(first, mid, std::greater<>()) != mid);
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<int> nums{0, 1, 2, 3, 4, 5};

    const auto begin = nums.begin();
    const auto mid = begin + 2;
    const auto end = nums.end();

    do {
        for (auto e = begin; e < mid; ++e) {
            std::cout << *e << " ";
        }
        std::cout << std::endl;
    } while (next_combination(begin, mid, end));

    return 0;
}