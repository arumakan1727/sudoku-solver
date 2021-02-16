#include <bits/stdc++.h>


constexpr size_t N = 9;

using Board = std::array<std::array<int, N>, N>;


static Board read_board(std::istream& is) {
    Board b;

    for (size_t y = 0; y < N; ++y) {
        for (size_t x = 0; x < N; ++x) {
            is >> b[y][x];
        }
    }

    return b;
}


static void print_board(const Board& b, std::ostream& os) {
    for (size_t y = 0; y < N; ++y) {
        for (size_t x = 0; x < N; ++x) {
            os << b[y][x] << (x + 1 == N ? '\n' : ' ');
        }
    }

    return;
}


int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    Board board = read_board(std::cin);

    print_board(board, std::cout);

    return 0;
}
