#!/usr/bin/env


class Solution:
    def findTargetSumWays(self, nums, S):
        """
        :type nums: List[int]
        :type S: int
        :rtype: int
        """

        from collections import defaultdict

        # Number of assignments just from nums[0:i].
        num_assignments_this = defaultdict(int)

        # Number of assignments from nums[0:i+1].
        num_assignments_next = defaultdict(int)

        #
        # Start with i = 0.
        #
        # NOTE: Positive zero (+0) and negative zero (-0) are considered
        # **different** in this question.
        #
        num_assignments_this[+nums[0]] += 1
        num_assignments_this[-nums[0]] += 1

        for i in range(1, len(nums)):
            for sum, num_assignments in num_assignments_this.items():
                num_assignments_next[sum + nums[i]] += num_assignments
                num_assignments_next[sum - nums[i]] += num_assignments
            num_assignments_this = num_assignments_next
            num_assignments_next = defaultdict(int)

        return num_assignments_this[S]


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        ns = list(map(int, line.split()))
        print(solution.findTargetSumWays(ns[:-1], ns[-1]))
