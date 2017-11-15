#!/usr/bin/env python3


class Solution:
    def rotate(self, matrix):
        """
        :type matrix: List[List[int]]
        :rtype: void Do not return anything, modify matrix in-place instead.
        """

        def matrix_multiply(m1, m2):
            """Multiply two square matrices."""
            m = [[0] * len(m1) for _ in range(len(m1))]
            for k in range(len(m1)):
                for i in range(len(m1)):
                    for j in range(len(m1)):
                        m[i][j] += m1[i][k] * m2[k][j]
            return m

        def multiply(m, v):
            """Multiply a square matrix and a vector."""
            return [
                sum(m[i][j] * v[j] for j in range(len(v)))
                for i in range(len(v))
            ]

        n = len(matrix)

        rotation = matrix_multiply(
            [[1, 0, (n-1)/2],
             [0, 1, (n-1)/2],
             [0, 0, 1]],
            matrix_multiply(
                [[0, -1, 0],
                 [1, 0, 0],
                 [0, 0, 1]],
                [[1, 0, -(n-1)/2],
                 [0, 1, -(n-1)/2],
                 [0, 0, 1]],
            ),
        )

        def get_element(coord):
            return matrix[int(coord[0])][int(coord[1])]

        def set_element(coord, element):
            matrix[int(coord[0])][int(coord[1])] = element

        def rotate_elements(i, j):
            coord0 = [i, j, 1]
            element = get_element(coord0)

            coord1 = multiply(rotation, coord0)
            set_element(coord0, get_element(coord1))
            coord0 = coord1

            coord1 = multiply(rotation, coord0)
            set_element(coord0, get_element(coord1))
            coord0 = coord1

            coord1 = multiply(rotation, coord0)
            set_element(coord0, get_element(coord1))
            coord0 = coord1

            set_element(coord0, element)

        mid = n // 2
        for i in range(mid):
            for j in range(mid):
                rotate_elements(i, j)

        if n % 2 != 0:
            i = n // 2
            for j in range(0, n // 2):
                rotate_elements(i, j)


if __name__ == '__main__':
    solution = Solution()

    matrix = [[1]]
    solution.rotate(matrix)
    print(matrix)

    matrix = [[1, 2],
              [3, 4]]
    solution.rotate(matrix)
    print(matrix)

    matrix = [[1, 2, 3],
              [4, 5, 6],
              [7, 8, 9]]
    solution.rotate(matrix)
    print(matrix)

    matrix = [[1, 2, 3, 4],
              [5, 6, 7, 8],
              [9, 10, 11, 12],
              [13, 14, 15, 16]]
    solution.rotate(matrix)
    print(matrix)
