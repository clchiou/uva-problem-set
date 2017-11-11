#!/usr/bin/env


class Solution:
    def countSubstrings(self, s):
        """
        :type s: str
        :rtype: int
        """
        # Use "center expansion" scheme.
        count = 0
        for i in range(len(s)):
            # For odd-length palindorme.
            for delta in range(len(s)):
                left = i - delta
                right = i + delta
                if left < 0 or right >= len(s):
                    break
                if s[left] != s[right]:
                    break
                count += 1
            # For even-length palindorme.
            for delta in range(len(s)):
                left = i - delta
                right = i + delta + 1
                if left < 0 or right >= len(s):
                    break
                if s[left] != s[right]:
                    break
                count += 1
        return count


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.countSubstrings(line.strip()))
