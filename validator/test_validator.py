#!/usr/bin/env python3
import unittest
import random
from typing import List
from . import validator


class TestMatrixIterators(unittest.TestCase):
    def setUp(self):
        self.board = [
            [3, 9, 1, 4, 2, 7, 8, 6, 5],
            [6, 8, 5, 1, 9, 3, 4, 2, 7],
            [4, 2, 7, 6, 8, 5, 3, 1, 9],
            [1, 3, 9, 8, 6, 4, 7, 5, 2],
            [8, 4, 6, 7, 5, 2, 9, 3, 1],
            [5, 7, 2, 3, 1, 9, 6, 8, 4],
            [2, 5, 3, 9, 7, 8, 1, 4, 6],
            [7, 1, 4, 2, 3, 6, 5, 9, 8],
            [9, 6, 8, 5, 4, 1, 2, 7, 3],
        ]

    def test_rows(self):
        self.assertListEqual(
            list(validator.rows(self.board)),
            [
                [3, 9, 1, 4, 2, 7, 8, 6, 5],
                [6, 8, 5, 1, 9, 3, 4, 2, 7],
                [4, 2, 7, 6, 8, 5, 3, 1, 9],
                [1, 3, 9, 8, 6, 4, 7, 5, 2],
                [8, 4, 6, 7, 5, 2, 9, 3, 1],
                [5, 7, 2, 3, 1, 9, 6, 8, 4],
                [2, 5, 3, 9, 7, 8, 1, 4, 6],
                [7, 1, 4, 2, 3, 6, 5, 9, 8],
                [9, 6, 8, 5, 4, 1, 2, 7, 3],
            ],
        )

    def test_cols(self):
        self.assertListEqual(
            list(validator.cols(self.board)),
            [
                [3, 6, 4, 1, 8, 5, 2, 7, 9],
                [9, 8, 2, 3, 4, 7, 5, 1, 6],
                [1, 5, 7, 9, 6, 2, 3, 4, 8],
                [4, 1, 6, 8, 7, 3, 9, 2, 5],
                [2, 9, 8, 6, 5, 1, 7, 3, 4],
                [7, 3, 5, 4, 2, 9, 8, 6, 1],
                [8, 4, 3, 7, 9, 6, 1, 5, 2],
                [6, 2, 1, 5, 3, 8, 4, 9, 7],
                [5, 7, 9, 2, 1, 4, 6, 8, 3],
            ],
        )

    def test_squares_3x3(self):
        self.assertListEqual(
            list(validator.squares_3x3(self.board)),
            [
                [3, 9, 1, 6, 8, 5, 4, 2, 7],
                [4, 2, 7, 1, 9, 3, 6, 8, 5],
                [8, 6, 5, 4, 2, 7, 3, 1, 9],
                [1, 3, 9, 8, 4, 6, 5, 7, 2],
                [8, 6, 4, 7, 5, 2, 3, 1, 9],
                [7, 5, 2, 9, 3, 1, 6, 8, 4],
                [2, 5, 3, 7, 1, 4, 9, 6, 8],
                [9, 7, 8, 2, 3, 6, 5, 4, 1],
                [1, 4, 6, 5, 9, 8, 2, 7, 3],
            ],
        )


class TestIsValidSequence(unittest.TestCase):
    def seq(self, first: int, last: int) -> List[int]:
        return list(range(first, last + 1))

    def seq_reversed(self, first: int, last: int) -> List[int]:
        return list(reversed(self.seq(first, last)))

    def seq_shuffled(self, first: int, last: int) -> List[int]:
        s = self.seq(first, last)
        random.shuffle(s)
        return s

    def seq_dupulicated_random_pos(self, first: int, last: int) -> List[int]:
        s = self.seq(first, last)
        i = random.randint(0, len(s) - 1)
        s[(i + 1) % len(s)] = s[i]
        random.shuffle(s)
        return s

    def test_is_valid_sequence__truthy(self):
        func = validator.is_valid_sequence
        self.assertTrue(func(self.seq(1, 9)))
        self.assertTrue(func(self.seq(1, 16)))
        self.assertTrue(func(self.seq_reversed(1, 9)))
        self.assertTrue(func(self.seq_reversed(1, 16)))

        self.assertTrue(func(self.seq_shuffled(1, 9)))
        self.assertTrue(func(self.seq_shuffled(1, 9)))
        self.assertTrue(func(self.seq_shuffled(1, 9)))
        self.assertTrue(func(self.seq_shuffled(1, 9)))
        self.assertTrue(func(self.seq_shuffled(1, 9)))

        self.assertTrue(func(self.seq_shuffled(1, 16)))
        self.assertTrue(func(self.seq_shuffled(1, 16)))
        self.assertTrue(func(self.seq_shuffled(1, 16)))
        self.assertTrue(func(self.seq_shuffled(1, 16)))
        self.assertTrue(func(self.seq_shuffled(1, 16)))

    def test_is_valid_sequence__falsy(self):
        func = validator.is_valid_sequence
        self.assertFalse(func(self.seq(0, 8)))
        self.assertFalse(func(self.seq(0, 15)))
        self.assertFalse(func(self.seq_reversed(0, 8)))
        self.assertFalse(func(self.seq_reversed(0, 15)))
        self.assertFalse(func(self.seq(0, 9)))
        self.assertFalse(func(self.seq(0, 16)))
        self.assertFalse(func(self.seq_reversed(0, 9)))
        self.assertFalse(func(self.seq_reversed(0, 16)))

        self.assertFalse(func(self.seq_shuffled(0, 9)))
        self.assertFalse(func(self.seq_shuffled(0, 9)))
        self.assertFalse(func(self.seq_shuffled(0, 9)))
        self.assertFalse(func(self.seq_shuffled(0, 9)))
        self.assertFalse(func(self.seq_shuffled(0, 9)))

        self.assertFalse(func(self.seq_shuffled(0, 16)))
        self.assertFalse(func(self.seq_shuffled(0, 16)))
        self.assertFalse(func(self.seq_shuffled(0, 16)))
        self.assertFalse(func(self.seq_shuffled(0, 16)))
        self.assertFalse(func(self.seq_shuffled(0, 16)))

        self.assertFalse(func(self.seq_dupulicated_random_pos(1, 9)))
        self.assertFalse(func(self.seq_dupulicated_random_pos(1, 9)))
        self.assertFalse(func(self.seq_dupulicated_random_pos(1, 9)))
        self.assertFalse(func(self.seq_dupulicated_random_pos(1, 9)))
        self.assertFalse(func(self.seq_dupulicated_random_pos(1, 9)))
        self.assertFalse(func(self.seq_dupulicated_random_pos(1, 9)))
        self.assertFalse(func(self.seq_dupulicated_random_pos(1, 9)))


class TestAssertBoardValidity(unittest.TestCase):
    def gen_valid_board(self) -> List[List[int]]:
        return [
            [3, 9, 1, 4, 2, 7, 8, 6, 5],
            [6, 8, 5, 1, 9, 3, 4, 2, 7],
            [4, 2, 7, 6, 8, 5, 3, 1, 9],
            [1, 3, 9, 8, 6, 4, 7, 5, 2],
            [8, 4, 6, 7, 5, 2, 9, 3, 1],
            [5, 7, 2, 3, 1, 9, 6, 8, 4],
            [2, 5, 3, 9, 7, 8, 1, 4, 6],
            [7, 1, 4, 2, 3, 6, 5, 9, 8],
            [9, 6, 8, 5, 4, 1, 2, 7, 3],
        ]

    def gen_invalid_board01(self) -> List[List[int]]:
        return [
            list(range(1, 9 + 1)) * 9
        ]

    def gen_invalid_board02(self) -> List[List[int]]:
        return [
            [3, 9, 1, 4, 2, 7, 8, 6, 5],
            [6, 8, 5, 1, 9, 3, 4, 2, 7],
            [4, 2, 7, 6, 8, 5, 3, 1, 9],
            [1, 3, 9, 8, 6, 4, 7, 5, 2],
            [8, 4, 6, 7, 5, 2, 9, 3, 1],
            [5, 7, 2, 3, 1, 9, 6, 8, 4],
            [2, 5, 3, 9, 7, 8, 7, 4, 6],
            [7, 1, 4, 2, 3, 6, 5, 9, 8],
            [9, 6, 8, 5, 4, 1, 2, 7, 3],
        ]

    def test_assert_board_validity(self):
        validator.assert_board_validity(self.gen_valid_board())

        with self.assertRaises(AssertionError):
            validator.assert_board_validity(self.gen_invalid_board01())

        with self.assertRaises(AssertionError):
            validator.assert_board_validity(self.gen_invalid_board02())
