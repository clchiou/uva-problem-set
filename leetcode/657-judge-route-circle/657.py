#!/usr/bin/env python3


class Solution:
    def judgeCircle(self, moves):
        """
        :type moves: str
        :rtype: bool
        """
        coordination = [0, 0]
        directions = {
            'U': (0, 1),
            'D': (0, -1),
            'L': (-1, 0),
            'R': (1, 0),
        }
        moves = moves.strip()
        for move in moves:
            delta = directions[move]
            coordination[0] += delta[0]
            coordination[1] += delta[1]
        return coordination == [0, 0]


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.judgeCircle(line.strip()))
