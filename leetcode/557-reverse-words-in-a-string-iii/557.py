#!/usr/bin/env python3


class Solution:
    def reverseWords(self, s):
        """
        :type s: str
        :rtype: str
        """
        return ' '.join(''.join(reversed(w)) for w in s.split())


if __name__ == '__main__':
    solution = Solution()
    print(solution.reverseWords("Let's take LeetCode contest"))
