#!/usr/bin/env python3
from typing import List, Iterator

N = 9


def main():
    mat = [[int(e) for e in input().split()] for _ in range(N)]
    assert_board_validity(mat)


def is_valid_sequence(seq: List[int]) -> bool:
    n = len(seq)
    return all(seq.count(i + 1) == 1 for i in range(n))


def assert_board_validity(board: List[List[int]]) -> None:
    assert len(board) == N
    assert all(len(row) == N for row in board)
    assert all(1 <= e <= 9 for e in cells(board))

    assert all(is_valid_sequence(row) for row in rows(board))
    assert all(is_valid_sequence(col) for col in cols(board))
    assert all(is_valid_sequence(square) for square in squares_3x3(board))


def cells(mat: List[List[int]]) -> Iterator[int]:
    for row in mat:
        for cell in row:
            yield cell


def rows(mat: List[List[int]]) -> Iterator[List[int]]:
    for row in mat:
        yield row


def cols(mat: List[List[int]]) -> Iterator[List[int]]:
    H = len(mat)
    W = len(mat[0])

    for x in range(W):
        yield [mat[y][x] for y in range(H)]


def squares_3x3(mat: List[List[int]]) -> Iterator[List[int]]:
    for top in range(0, 9, 3):
        for left in range(0, 9, 3):
            seq = []

            for dy in range(3):
                for dx in range(3):
                    seq.append(mat[top + dy][left + dx])

            yield seq


if __name__ == "__main__":
    main()
