#!/usr/bin/env python3


class Solution:
    def getPermutation(self, n, k):
        """
        :type n: int
        :type k: int
        :rtype: str
        """

        def factorial(n):
            acc = 1
            for x in range(1, n + 1):
                acc *= x
            return acc

        k -= 1

        output = []

        elements = list(map(str, range(1, n + 1)))
        unit = factorial(n - 1)
        for m in range(n - 1, 0, -1):
            output.append(elements.pop(k // unit))
            k %= unit
            unit //= m
        output.append(elements.pop(0))
        assert not elements

        return ''.join(output)


if __name__ == '__main__':

    solution = Solution()

    assert '1' == solution.getPermutation(1, 1)

    assert '12' == solution.getPermutation(2, 1)
    assert '21' == solution.getPermutation(2, 2)

    assert '123' == solution.getPermutation(3, 1)
    assert '132' == solution.getPermutation(3, 2)
    assert '213' == solution.getPermutation(3, 3)
    assert '231' == solution.getPermutation(3, 4)
    assert '312' == solution.getPermutation(3, 5)
    assert '321' == solution.getPermutation(3, 6)
