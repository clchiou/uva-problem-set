#!/usr/bin/env python3


class Solution:
    def totalNQueens(self, n):
        """
        :type n: int
        :rtype: int
        """

        x_taken = set()
        x_plus_y_taken = set()
        x_minus_y_taken = set()

        def count_n_queens(y):
            if y >= n:
                return 1

            count = 0
            for x in range(n):
                if x in x_taken:
                    continue
                x_plus_y = x + y
                if x_plus_y in x_plus_y_taken:
                    continue
                x_minus_y = x - y
                if x_minus_y in x_minus_y_taken:
                    continue

                x_taken.add(x)
                x_plus_y_taken.add(x_plus_y)
                x_minus_y_taken.add(x_minus_y)

                count += count_n_queens(y + 1)

                x_minus_y_taken.remove(x_minus_y)
                x_plus_y_taken.remove(x_plus_y)
                x_taken.remove(x)

            return count

        return count_n_queens(0)


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.totalNQueens(int(line)))
