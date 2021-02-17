#include <bits/stdc++.h>

constexpr size_t N = 9;

// ----------------------------------------------------------------------------------------
template <class IndexConsumerFunc>
void row_elements_foreach(const size_t, IndexConsumerFunc&&);

template <class IndexConsumerFunc>
void col_elements_foreach(const size_t, IndexConsumerFunc&&);

template <class IndexConsumerFunc>
void square3x3_elements_foreach(const size_t, IndexConsumerFunc&&);

static void test_row_elements_foreach();

static void test_col_elements_foreach();

static void test_square3x3_elements_foreach();

static void test();
// ----------------------------------------------------------------------------------------

struct Cell {
    using value_type = uint16_t;
    using candidates_bitset_type = uint16_t;

    value_type value;
    candidates_bitset_type candidates;
};


struct Board {
    std::array<Cell, N * N> cells;

    inline Cell& at(size_t y, size_t x) {
        return cells[N * y + x];
    }

    inline const Cell& at(size_t y, size_t x) const {
        return cells[N * y + x];
    }

    void store_candidates() {
        for (size_t i = 0; i < cells.size(); ++i) {
            cells[i].candidates = calc_candidates(i);
        }
    }

    Cell::candidates_bitset_type calc_candidates(size_t target_index) const {
        if (cells[target_index].value != 0) {
            return 0;
        }

        constexpr Cell::candidates_bitset_type all_bits_enabled = (1 << (N + 1)) - 1 ^ 1;
        assert(__builtin_popcount(all_bits_enabled) == N);
        assert((all_bits_enabled >> 1 & 1) == 1);
        assert((all_bits_enabled >> 9 & 1) == 1);
        assert((all_bits_enabled >> 0 & 1) == 0);
        Cell::candidates_bitset_type candidates = all_bits_enabled;

        const auto f =  [&](size_t i) {
            const auto v = cells[i].value;
            if (v == 0) return;
            candidates &= all_bits_enabled ^ (1 << v);
        };

        row_elements_foreach(target_index, f);
        col_elements_foreach(target_index, f);
        square3x3_elements_foreach(target_index, f);

        return candidates;
    }

};

// ----------------------------------------------------------------------------------------
static Board read_board(std::istream&);

static void print_board(const Board& b, std::ostream&);
// ----------------------------------------------------------------------------------------


int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    test();

    Board board = read_board(std::cin);

    print_board(board, std::cout);

    return 0;
}


static Board read_board(std::istream& is) {
    Board b;

    for (size_t y = 0; y < N; ++y) {
        for (size_t x = 0; x < N; ++x) {
            is >> b.at(y, x).value;
        }
    }

    b.store_candidates();

    return b;
}


static void print_board(const Board& b, std::ostream& os) {
    for (size_t y = 0; y < N; ++y) {
        for (size_t x = 0; x < N; ++x) {
            os << b.at(y, x).value << (x + 1 == N ? '\n' : ' ');
        }
    }

    return;
}


/**
 * each [row][0], [row1][1], ... , [row][N - 1]
 */
template <class IndexConsumerFunc>
void row_elements_foreach(const size_t i, IndexConsumerFunc&& indexConsumer) {
    const auto begin = N * (i / N);
    const auto end = begin + N;
    for (size_t k = begin; k < end; ++k) {
        indexConsumer(k);
    }
    return;
}


/**
 * each [0][col], [1][col], ... , [col][N - 1]
 */
template <class IndexConsumerFunc>
void col_elements_foreach(const size_t i, IndexConsumerFunc&& indexConsumer) {
    const auto begin = i % N;
    const auto last = begin + N * (N - 1);
    for (size_t k = begin; k <= last; k += N) {
        indexConsumer(k);
    }
    return;
}


template <class IndexConsumerFunc>
void square3x3_elements_foreach(const size_t i, IndexConsumerFunc&& indexConsumer) {
    const auto received_y = i / N;
    const auto received_x = i % N;
    const auto sy = 3 * (received_y / 3);
    const auto sx = 3 * (received_x / 3);
    for (size_t y = sy; y < sy + 3; ++y) {
        for (size_t x = sx; x < sx + 3; ++x) {
            indexConsumer(N * y + x);
        }
    }
    return;
}


static void test() {
#ifndef NDEBUG
    test_row_elements_foreach();
    test_col_elements_foreach();
    test_square3x3_elements_foreach();

    std::clog << "All test passed!!" << std::endl;
#endif
    return;
}


static void test_row_elements_foreach() {
    const auto collect = [](const size_t target_index) {
        std::vector<size_t> indexes;
        indexes.reserve(N);
        row_elements_foreach(target_index, [&](size_t i) { indexes.push_back(i); });
        return indexes;
    };

    assert(collect(0) == std::vector<size_t>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
    assert(collect(1) == std::vector<size_t>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
    assert(collect(2) == std::vector<size_t>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
    assert(collect(8) == std::vector<size_t>({0, 1, 2, 3, 4, 5, 6, 7, 8}));

    assert(collect(9) == std::vector<size_t>({9, 10, 11, 12, 13, 14, 15, 16, 17}));
    assert(collect(10) == std::vector<size_t>({9, 10, 11, 12, 13, 14, 15, 16, 17}));
    assert(collect(11) == std::vector<size_t>({9, 10, 11, 12, 13, 14, 15, 16, 17}));
    assert(collect(17) == std::vector<size_t>({9, 10, 11, 12, 13, 14, 15, 16, 17}));

    assert(collect(72) == std::vector<size_t>({72, 73, 74, 75, 76, 77, 78, 79, 80}));
    assert(collect(73) == std::vector<size_t>({72, 73, 74, 75, 76, 77, 78, 79, 80}));
    assert(collect(74) == std::vector<size_t>({72, 73, 74, 75, 76, 77, 78, 79, 80}));
    assert(collect(80) == std::vector<size_t>({72, 73, 74, 75, 76, 77, 78, 79, 80}));

    std::clog << "[test_row_elements_foreach()] all tests passed!" << std::endl;
    return;
}


static void test_col_elements_foreach() {
    const auto collect = [](const size_t target_index) {
        std::vector<size_t> indexes;
        indexes.reserve(N);
        col_elements_foreach(target_index, [&](size_t i) { indexes.push_back(i); });
        return indexes;
    };

    assert(collect(0) == std::vector<size_t>({0, 9, 18, 27, 36, 45, 54, 63, 72}));
    assert(collect(9) == std::vector<size_t>({0, 9, 18, 27, 36, 45, 54, 63, 72}));
    assert(collect(18) == std::vector<size_t>({0, 9, 18, 27, 36, 45, 54, 63, 72}));
    assert(collect(72) == std::vector<size_t>({0, 9, 18, 27, 36, 45, 54, 63, 72}));

    assert(collect(1) == std::vector<size_t>({1, 10, 19, 28, 37, 46, 55, 64, 73}));
    assert(collect(10) == std::vector<size_t>({1, 10, 19, 28, 37, 46, 55, 64, 73}));
    assert(collect(19) == std::vector<size_t>({1, 10, 19, 28, 37, 46, 55, 64, 73}));
    assert(collect(73) == std::vector<size_t>({1, 10, 19, 28, 37, 46, 55, 64, 73}));

    assert(collect(8) == std::vector<size_t>({8, 17, 26, 35, 44, 53, 62, 71, 80}));
    assert(collect(17) == std::vector<size_t>({8, 17, 26, 35, 44, 53, 62, 71, 80}));
    assert(collect(26) == std::vector<size_t>({8, 17, 26, 35, 44, 53, 62, 71, 80}));
    assert(collect(80) == std::vector<size_t>({8, 17, 26, 35, 44, 53, 62, 71, 80}));

    std::clog << "[test_col_elements_foreach()] all tests passed!" << std::endl;
    return;
}


static void test_square3x3_elements_foreach() {
    const auto collect = [](const size_t target_index) {
        std::vector<size_t> indexes;
        indexes.reserve(N);
        square3x3_elements_foreach(target_index, [&](size_t i) { indexes.push_back(i); });
        return indexes;
    };

    assert(collect(0) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(1) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(2) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(9) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(10) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(11) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(18) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(19) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    assert(collect(20) == std::vector<size_t>({0, 1, 2, 9, 10, 11, 18, 19, 20}));

    assert(collect(3) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(4) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(5) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(12) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(13) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(14) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(21) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(22) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));
    assert(collect(23) == std::vector<size_t>({3, 4, 5, 12, 13, 14, 21, 22, 23}));

    assert(collect(30) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(31) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(32) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(39) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(40) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(41) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(48) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(49) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));
    assert(collect(50) == std::vector<size_t>({30, 31, 32, 39, 40, 41, 48, 49, 50}));

    assert(collect(60) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(61) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(62) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(69) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(70) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(71) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(78) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(79) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));
    assert(collect(80) == std::vector<size_t>({60, 61, 62, 69, 70, 71, 78, 79, 80}));

    std::clog << "[test_square3x3_elements_foreach()] all tests passed!" << std::endl;
}
