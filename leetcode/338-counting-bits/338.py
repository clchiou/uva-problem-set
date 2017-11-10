#!/usr/bin/env python3


class Solution:
    def countBits(self, num):
        """
        :type num: int
        :rtype: List[int]
        """
        output = [0]
        num_ones = 0
        binary = [0] * (32 + 1)  # Assume num < 2^32.
        for _ in range(1, num + 1):
            # Add 1 to `binary`.
            for i in range(len(binary)):
                if binary[i] == 0:
                    binary[i] = 1
                    num_ones += 1
                    break
                else:
                    binary[i] = 0
                    num_ones -= 1
            output.append(num_ones)
        return output


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.countBits(int(line)))
