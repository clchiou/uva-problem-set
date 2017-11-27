#!/usr/bin/env python3


class Solution:
    def nextGreaterElement(self, n):
        """
        :type n: int
        :rtype: int
        """

        # Assume signed 32-bit integer.
        max_int = 0x7fffffff

        n = list(str(n))

        # Find the rightmost "drop".
        for i in range(len(n) - 1, 0, -1):
            if n[i-1] < n[i]:
                break
        else:
            return -1

        # Find the smallest n[j] > n[i-1].
        j_min = i
        for j in range(i, len(n)):
            if n[i-1] < n[j] < n[j_min]:
                j_min = j

        n[i-1], n[j_min] = n[j_min], n[i-1]
        n[i:] = sorted(n[i:])

        n = int(''.join(n))
        if n > max_int:
            return -1
        else:
            return n


if __name__ == '__main__':
    solution = Solution()

    assert -1 == solution.nextGreaterElement(1)
    assert -1 == solution.nextGreaterElement(2)
    assert -1 == solution.nextGreaterElement(11)
    assert -1 == solution.nextGreaterElement(999)
    assert -1 == solution.nextGreaterElement(21)
    assert -1 == solution.nextGreaterElement(321)

    assert 21 == solution.nextGreaterElement(12)
    assert 132 == solution.nextGreaterElement(123)
    assert 213 == solution.nextGreaterElement(132)
    assert 312245 == solution.nextGreaterElement(254321)

    assert 2123 == solution.nextGreaterElement(1322)
    assert 3222 == solution.nextGreaterElement(2322)

    assert -1 == solution.nextGreaterElement(2147483647)
    assert -1 == solution.nextGreaterElement(2147483646)
    assert -1 == solution.nextGreaterElement(2147483645)
    assert -1 == solution.nextGreaterElement(2147483644)
    assert -1 == solution.nextGreaterElement(2147483643)
    assert -1 == solution.nextGreaterElement(2147483642)
