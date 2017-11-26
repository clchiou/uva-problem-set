#!/usr/bin/env python3


class Solution:
    def pacificAtlantic(self, matrix):
        """
        :type matrix: List[List[int]]
        :rtype: List[List[int]]
        """

        if not matrix or not matrix[0]:
            return []

        num_rows = len(matrix)
        num_cols = len(matrix[0])

        PACIFIC = 0x01
        ATLANTIC = 0x02
        BOTH = PACIFIC | ATLANTIC

        may_reach = [[0] * num_cols for _ in range(num_rows)]

        def traverse(queue, ocean):
            while queue:
                i, j = queue.pop(0)
                may_reach[i][j] |= ocean
                for di, dj in ((-1, 0), (1, 0), (0, -1), (0, 1)):
                    ii = i + di
                    jj = j + dj
                    if not (0 <= ii < num_rows and 0 <= jj < num_cols):
                        continue
                    if (may_reach[ii][jj] & ocean) != 0:
                        continue
                    if matrix[i][j] <= matrix[ii][jj]:
                        queue.append((ii, jj))

        queue = [(i, 0) for i in range(num_rows)]
        queue.extend((0, j) for j in range(1, num_cols))
        traverse(queue, PACIFIC)

        queue = [(i, num_cols - 1) for i in range(num_rows)]
        queue.extend((num_rows - 1, j) for j in range(num_cols - 1))
        traverse(queue, ATLANTIC)

        return [
            [i, j]
            for i in range(num_rows)
            for j in range(num_cols)
            if may_reach[i][j] == BOTH
        ]


if __name__ == '__main__':

    solution = Solution()

    matrix = [
        [1, 1],
        [1, 1],
    ]
    expect = [[0, 0], [0, 1], [1, 0], [1, 1]]
    assert expect == solution.pacificAtlantic(matrix)

    matrix = [
        [1, 1, 2],
        [1, 0, 1],
        [2, 1, 1],
    ]
    expect = [[0, 2], [2, 0]]
    assert expect == solution.pacificAtlantic(matrix)

    matrix = [
        [1, 2, 2, 3, 5],
        [3, 2, 3, 4, 4],
        [2, 4, 5, 3, 1],
        [6, 7, 1, 4, 5],
        [5, 1, 1, 2, 4],
    ]
    expect = [[0, 4], [1, 3], [1, 4], [2, 2], [3, 0], [3, 1], [4, 0]]
    assert expect == solution.pacificAtlantic(matrix)
