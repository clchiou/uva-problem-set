#!/usr/bin/env python3


class Solution:
    def findLength(self, A, B):
        """
        :type A: List[int]
        :type B: List[int]
        :rtype: int
        """

        # lcs_table[i][j] is the length of LCS of A[0:i] and B[0:j].
        lcs_table = [[0] * (len(B) + 1) for _ in range(len(A) + 1)]

        lcs = 0

        for i in range(len(A) + 1):
            for j in range(len(B) + 1):
                if i == 0 or j == 0:
                    pass
                elif A[i-1] == B[j-1]:
                    lcs_table[i][j] = lcs_table[i-1][j-1] + 1
                    lcs = max(lcs, lcs_table[i][j])

        return lcs


if __name__ == '__main__':
    import sys
    solution = Solution()
    to_ints = lambda ints_str: list(map(int, ints_str.split()))
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.findLength(
            to_ints(line),
            to_ints(sys.stdin.readline()),
        ))
