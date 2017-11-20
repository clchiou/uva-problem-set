#!/usr/bin/env python3


class Solution:
    def canJump(self, nums):
        """
        :type nums: List[int]
        :rtype: bool
        """
        max_reachable = 0
        for i, length in enumerate(nums):
            if i <= max_reachable:
                max_reachable = max(max_reachable, i + length)
        return max_reachable >= len(nums) - 1
