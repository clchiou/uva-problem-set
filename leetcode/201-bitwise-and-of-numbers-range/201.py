#!/usr/bin/env python3


class Solution:
    def rangeBitwiseAnd(self, m, n):
        """
        :type m: int
        :type n: int
        :rtype: int
        """

        if m == 0 or m == n:
            return m

        # Find the most significant bit that is different.
        b = 1
        while b <= n:
            b <<= 1
        while (m & b) == (n & b):
            b >>= 1

        # Set all following bits to 0.
        b = (b << 1) - 1
        return (n | b) ^ b


if __name__ == '__main__':

    def solve(m, n):
        acc = n
        for x in range(m, n):
            acc &= x
        return acc

    testcases = (
        (0, 0),
        (0, 19),
        (1, 1),
        (1, 2),
        (1, 3),
        (5, 7),
        (0b0010_0100, 0b0010_1101),
        (0b0000_1000, 0b0010_0010),
    )

    solution = Solution()
    for m, n in testcases:
        assert solve(m, n) == solution.rangeBitwiseAnd(m, n), (m, n)
