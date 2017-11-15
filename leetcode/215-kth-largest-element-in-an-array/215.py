#!/usr/bin/env python3


class Solution:
    def findKthLargest(self, nums, k):
        """
        :type nums: List[int]
        :type k: int
        :rtype: int
        """

        import random

        def solve(nums, nth):

            if len(nums) == 1:
                return nums[0]

            pivot_i = random.randint(0, len(nums) - 1)
            pivot = nums[pivot_i]

            larger = [
                x for i, x in enumerate(nums)
                if x > pivot and i != pivot_i
            ]
            if len(larger) >= nth:
                return solve(larger, nth)

            not_larger = [
                x for i, x in enumerate(nums)
                if x <= pivot and i != pivot_i
            ]
            if len(larger) == nth - 1 and len(not_larger) == len(nums) - nth:
                return pivot

            return solve(not_larger, nth - len(larger) - 1)

        return solve(nums, k)


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        ints = list(map(int, line.split()))
        print(solution.findKthLargest(ints[:-1], ints[-1]))
