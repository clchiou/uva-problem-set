#!/usr/bin/env python3


class Solution:
    def findComplement(self, num):
        """
        :type num: int
        :rtype: int
        """
        bits = []
        while num > 0:
            bits.append(num & 1)
            num >>= 1
        for i in range(len(bits)):
            bits[i] = 0 if bits[i] == 1 else 1
        complement = 0
        for i, b in enumerate(bits):
            complement += (b << i)
        return complement


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.findComplement(int(line)))
