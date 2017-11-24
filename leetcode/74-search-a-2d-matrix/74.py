#!/usr/bin/env python3


def bsearch_lower(array, target):
    """Return index such that
    array[index] <= target < array[index + 1]
    """
    left = 0
    right = len(array)
    while left < right:
        mid = (left + right) // 2
        value = array[mid]
        if value == target:
            return mid
        elif value < target:
            left = mid + 1
        else:
            right = mid
    if left < len(array):
        return left - 1  # This may return -1.
    else:
        return len(array)


class Solution:
    def searchMatrix(self, matrix, target):
        """
        :type matrix: List[List[int]]
        :type target: int
        :rtype: bool
        """

        if not matrix or not matrix[0]:
            return False

        i = bsearch_lower([v[0] for v in matrix], target)
        if i == -1:
            return False

        if i == len(matrix):
            vector = matrix[-1]
        else:
            vector = matrix[i]
        j = bsearch_lower(vector, target)
        if j == -1 or j == len(vector):
            return False
        else:
            return vector[j] == target


if __name__ == '__main__':

    assert 0 == bsearch_lower([], 0)

    assert -1 == bsearch_lower([1], 0)
    assert 0 == bsearch_lower([1], 1)
    assert 1 == bsearch_lower([1], 2)
    assert 1 == bsearch_lower([1], 3)

    assert -1 == bsearch_lower([2, 4], 1)
    assert 0 == bsearch_lower([2, 4], 2)
    assert 0 == bsearch_lower([2, 4], 3)
    assert 1 == bsearch_lower([2, 4], 4)
    assert 2 == bsearch_lower([2, 4], 5)

    assert -1 == bsearch_lower([2, 4, 6], 1)
    assert 0 == bsearch_lower([2, 4, 6], 2)
    assert 0 == bsearch_lower([2, 4, 6], 3)
    assert 1 == bsearch_lower([2, 4, 6], 4)
    assert 1 == bsearch_lower([2, 4, 6], 5)
    assert 2 == bsearch_lower([2, 4, 6], 6)
    assert 3 == bsearch_lower([2, 4, 6], 7)

    assert -1 == bsearch_lower([2, 4, 6, 8], 1)
    assert 0 == bsearch_lower([2, 4, 6, 8], 2)
    assert 0 == bsearch_lower([2, 4, 6, 8], 3)
    assert 1 == bsearch_lower([2, 4, 6, 8], 4)
    assert 1 == bsearch_lower([2, 4, 6, 8], 5)
    assert 2 == bsearch_lower([2, 4, 6, 8], 6)
    assert 2 == bsearch_lower([2, 4, 6, 8], 7)
    assert 3 == bsearch_lower([2, 4, 6, 8], 8)
    assert 4 == bsearch_lower([2, 4, 6, 8], 9)

    matrix = [
        [1,   3,  5,  7],
        [10, 11, 16, 20],
        [23, 30, 34, 50],
    ]

    solution = Solution()

    assert not solution.searchMatrix(matrix, 0)
    assert solution.searchMatrix(matrix, 1)
    assert not solution.searchMatrix(matrix, 2)
    assert solution.searchMatrix(matrix, 3)
    assert not solution.searchMatrix(matrix, 4)
    assert solution.searchMatrix(matrix, 5)
    assert not solution.searchMatrix(matrix, 6)
    assert solution.searchMatrix(matrix, 7)
    assert not solution.searchMatrix(matrix, 8)

    assert solution.searchMatrix(matrix, 10)
    assert not solution.searchMatrix(matrix, 15)
    assert not solution.searchMatrix(matrix, 19)
    assert not solution.searchMatrix(matrix, 21)

    assert solution.searchMatrix(matrix, 30)

    assert not solution.searchMatrix(matrix, 51)
