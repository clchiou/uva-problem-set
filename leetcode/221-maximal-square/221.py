#!/usr/bin/env python3


class Solution:
    def maximalSquare(self, matrix):
        """
        :type matrix: List[List[str]]
        :rtype: int
        """

        if not matrix:
            return 0

        w = len(matrix[0])
        h = len(matrix)

        #
        # Construct prefix sum matrix where scan[i][j] is the sum of
        # sub-matrix[0:i][0:j].
        #
        scan = [[0] * (w + 1) for i in range(h + 1)]
        for i in range(1, h + 1):
            for j in range(1, w + 1):
                scan[i][j] = int(matrix[i - 1][j - 1])
                scan[i][j] += scan[i - 1][j]
                scan[i][j] += scan[i][j - 1]
                scan[i][j] -= scan[i - 1][j - 1]

        def square_sum(i, j, size):
            total = 0
            total += scan[i + size][j + size]
            total += scan[i][j]
            total -= scan[i][j + size]
            total -= scan[i + size][j]
            return total

        max_size = 0
        for i in range(h):
            for j in range(w):
                for size in range(max(max_size, 1), min(w, h) + 1):
                    if i + size > h or j + size > w:
                        break
                    if size * size != square_sum(i, j, size):
                        break
                    max_size = max(max_size, size)

        return max_size * max_size


if __name__ == '__main__':
    def assert_eq(expect, actaul):
        if expect != actaul:
            raise AssertionError('expect %s, not %s' % (expect, actaul))
    solution = Solution()
    assert_eq(0, solution.maximalSquare(['0']))
    assert_eq(0, solution.maximalSquare(['00', '00']))
    assert_eq(1, solution.maximalSquare(['1']))
    assert_eq(1, solution.maximalSquare(['10', '01']))
    assert_eq(4, solution.maximalSquare(['10100', '10111', '11111', '10010']))
