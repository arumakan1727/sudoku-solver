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
    using candidate_bitset_type = uint16_t;

    value_type value;
    candidate_bitset_type candidate;
};

// 空欄マスを表す値
constexpr Cell::value_type BLANK = 0;

// 区間 [1, N] のビットが 1 になっている値 (0ビット目はゼロ)
constexpr Cell::candidate_bitset_type BITS_ALL_ENABLED = (1 << (N + 1)) - 1 ^ 1;
static_assert(__builtin_popcount(BITS_ALL_ENABLED) == N);
static_assert((BITS_ALL_ENABLED >> 1 & 1) == 1);
static_assert((BITS_ALL_ENABLED >> 9 & 1) == 1);
static_assert((BITS_ALL_ENABLED >> 0 & 1) == 0);

struct Board {
    std::array<Cell, N * N> cells;

    inline Cell& at(size_t y, size_t x) { return cells[N * y + x]; }

    inline const Cell& at(size_t y, size_t x) const { return cells[N * y + x]; }

    void store_candidates() {
        for (size_t i = 0; i < cells.size(); ++i) {
            cells[i].candidate = calc_candidates(i);
        }
    }

    /**
     * マス i に value を書き込んでから、他の空欄マスの候補集合を更新する。
     *
     * 更新とは具体的に、
     * 書き込んだマスが属する 行・列・3x3区画 のうちの空欄マスについて、
     * 候補の数値集合から書き込んだ値を取り除くことを指す。
     */
    void put_with_overwrite(size_t i, Cell::value_type value) {
        cells[i].value = value;
        cells[i].candidate = 0;

        const auto f = [&](size_t k) { cells[k].candidate &= BITS_ALL_ENABLED ^ (1 << value); };

        row_elements_foreach(i, f);
        col_elements_foreach(i, f);
        square3x3_elements_foreach(i, f);
    }

    Board put_with_copy(size_t i, Cell::value_type value) const {
        auto copy = *this;
        copy.put_with_overwrite(i, value);
        return copy;
    }

    // 未定マスなのに候補が一つもない場合は false を返す (=手詰まり)
    [[nodiscard]] bool fill_obvious_candidates_and_check_solvability() {
        for (size_t i = 0; i < cells.size(); ++i) {
            if (cells[i].value != BLANK) continue;

            if (cells[i].candidate == 0) {
                return false;
            }

            if (__builtin_popcount(cells[i].candidate) == 1) {
                const auto value = static_cast<Cell::value_type>(__builtin_ctz(cells[i].candidate));
                put_with_overwrite(i, value);
                return fill_obvious_candidates_and_check_solvability();
            }
        }

        return true;
    }

    // returns index
    // 手詰まりの盤面の状態で呼び出してはならない (多分 assertion faild になる)
    std::optional<size_t> least_cardinality_candidate_index() const {
        size_t cardinality_min = std::numeric_limits<size_t>::max();
        size_t index;

        for (size_t i = 0; i < cells.size(); ++i) {
            if (cells[i].value != BLANK) continue;
            assert(cells[i].candidate != 0);

            const auto card = static_cast<size_t>(__builtin_popcount(cells[i].candidate));
            if (card < cardinality_min) {
                cardinality_min = card;
                index = i;
            }
        }

        if (cardinality_min == std::numeric_limits<size_t>::max()) {
            return std::nullopt;
        }
        return index;
    }

  private:
    Cell::candidate_bitset_type calc_candidates(size_t target_index) const {
        if (cells[target_index].value != BLANK) {
            return 0;
        }

        Cell::candidate_bitset_type candidate = BITS_ALL_ENABLED;

        const auto f = [&](size_t i) {
            const auto v = cells[i].value;
            if (v == BLANK) return;
            candidate &= BITS_ALL_ENABLED ^ (1 << v);
        };

        row_elements_foreach(target_index, f);
        col_elements_foreach(target_index, f);
        square3x3_elements_foreach(target_index, f);

        return candidate;
    }
};

static Board read_board(std::istream&);

static bool solve(Board& input, Board& output);

static void print_board(const Board& b, std::ostream&);

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    test();

    Board input_board = read_board(std::cin);

    Board answer;
    const bool solve_succeeded = solve(input_board, answer);
    assert(solve_succeeded);

    print_board(answer, std::cout);

    return 0;
}

// is から空白区切りで盤面を受け取って、各空欄マスについて候補値の集合が計算された状態の board を返す。
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

/**
 * input に対する解を求めて output に格納する。
 * 解が求まった場合は true, そうでなければ false を返す。
 * input は store_candidates() が行われた状態でなければならない。
 */
static bool solve(Board& input, Board& output) {
    const bool solvable = input.fill_obvious_candidates_and_check_solvability();
    if (not solvable) {
        return false;
    }

    // 空欄を埋める対象のマスが無い ==> 全マスが埋まったの完了
    const auto index_opt = input.least_cardinality_candidate_index();
    if (not index_opt.has_value()) {
        output = input;
        return true;
    }

    const auto index = *index_opt;
    const auto candidate = input.cells[index].candidate;
    for (Cell::value_type v = 1; v <= N; ++v) {
        if (candidate >> v & 1) {
            auto next_state = input.put_with_copy(index, v);

            const bool solve_succeeded = solve(next_state, output);
            if (solve_succeeded) {
                return true;
            }
        }
    }

    return false;
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
 * each [0][col], [1][col], ... , [N - 1][col]
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
