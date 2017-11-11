#!/usr/bin/env python3


class Solution:
    def singleNonDuplicate(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """

        # Let's handle corner cases first.
        if len(nums) == 1:
            return nums[0]
        if nums[0] != nums[1]:
            return nums[0]
        if nums[-2] != nums[-1]:
            return nums[-1]

        for i in range(1, len(nums) - 1):
            if nums[i] != nums[i-1] and nums[i] != nums[i+1]:
                return nums[i]

        raise AssertionError


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.singleNonDuplicate(list(map(int, line.split()))))
