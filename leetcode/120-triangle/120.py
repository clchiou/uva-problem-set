#!/usr/bin/env python3


class Solution:
    def minimumTotal(self, triangle):
        """
        :type triangle: List[List[int]]
        :rtype: int
        """

        if not triangle:
            return 0

        for i in range(1, len(triangle)):
            triangle[i][0] += triangle[i-1][0]
            triangle[i][i] += triangle[i-1][i-1]
            for j in range(1, i):
                triangle[i][j] += min(
                    triangle[i-1][j-1],
                    triangle[i-1][j],
                )

        return min(triangle[-1])


if __name__ == '__main__':
    solution = Solution()
    assert 0 == solution.minimumTotal([])
    assert 9 == solution.minimumTotal([[9]])
    assert 3 == solution.minimumTotal([[1], [2, 3]])
    assert 11 == solution.minimumTotal([[2], [3, 4], [6, 5, 7], [4, 1, 8, 3]])
