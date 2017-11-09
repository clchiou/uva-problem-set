#!/usr/bin/env python3


class Solution:
    def hammingDistance(self, x, y):
        """
        :type x: int
        :type y: int
        :rtype: int
        """
        distance = 0
        while x != 0 or y != 0:
            if (x & 1) != (y & 1):
                distance += 1
            x >>= 1
            y >>= 1
        return distance


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        x, y = tuple(map(int, line.split()))
        print(solution.hammingDistance(x, y))
