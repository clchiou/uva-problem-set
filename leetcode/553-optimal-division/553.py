#!/usr/bin/env python3


class Solution:
    def optimalDivision(self, nums):
        """
        :type nums: List[int]
        :rtype: str
        """

        if len(nums) == 1:
            return str(nums[0])

        MAX = float('+inf')
        MIN = float('-inf')

        #
        # table_max[i][j] is the division (k_max, r_max) where
        #   nums[i:k_max] / nums[k_max:j]
        # yields the maximal division r_max.
        #
        table_max = [
            [(None, MIN) for _ in range(len(nums) + 1)]
            for _ in range(len(nums))
        ]

        table_min = [
            [(None, MAX) for _ in range(len(nums) + 1)]
            for _ in range(len(nums))
        ]

        # Base case (one number).
        for i in range(len(nums)):
            table_max[i][i+1] = (None, nums[i])
            table_min[i][i+1] = (None, nums[i])

        # Base case (two numbers).
        for i in range(len(nums) - 1):
            table_max[i][i+2] = (i + 1, nums[i] / nums[i+1])
            table_min[i][i+2] = (i + 1, nums[i] / nums[i+1])

        for n in range(3, len(nums) + 1):
            for i in range(0, len(nums) - n + 1):
                j = i + n
                k_max = k_min = None
                r_max = MIN
                r_min = MAX
                for k in range(i + 1, j):
                    r = table_max[i][k][1] / table_min[k][j][1]
                    if r > r_max:
                        k_max = k
                        r_max = r
                    r = table_min[i][k][1] / table_max[k][j][1]
                    if r < r_min:
                        k_min = k
                        r_min = r
                if k_max is not None:
                    table_max[i][j] = (k_max, r_max)
                if k_min is not None:
                    table_min[i][j] = (k_min, r_min)

        def format_max_expr(pieces, i, j):
            k = table_max[i][j][0]
            if i + 1 == k:
                pieces.append(str(nums[i]))
            else:
                format_max_expr(pieces, i, k)
            pieces.append('/')
            if k + 1 == j:
                pieces.append(str(nums[k]))
            else:
                pieces.append('(')
                format_min_expr(pieces, k, j)
                pieces.append(')')

        def format_min_expr(pieces, i, j):
            k = table_min[i][j][0]
            if i + 1 == k:
                pieces.append(str(nums[i]))
            else:
                format_min_expr(pieces, i, k)
            pieces.append('/')
            if k + 1 == j:
                pieces.append(str(nums[k]))
            else:
                pieces.append('(')
                format_max_expr(pieces, k, j)
                pieces.append(')')

        pieces = []
        format_max_expr(pieces, 0, len(nums))
        return ''.join(pieces)


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.optimalDivision(list(map(int, line.split()))))
