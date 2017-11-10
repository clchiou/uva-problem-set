#!/usr/bin/env python3


class Solution:
    def findDuplicates(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """

        #
        # HACK: We may use negative sign at `i`-th element to represent
        # whether we have seen `i + 1` before; in other words, we get a
        # free extra bit from the input nums array, thus satisfying the
        # "no extra space" requirement.
        #

        def mark_as_seen(x):
            nums[x-1] = -abs(nums[x-1])

        def has_seen(x):
            return nums[x-1] < 0

        def get(i):
            return abs(nums[i])

        output = []
        for i in range(len(nums)):
            x = get(i)
            if has_seen(x):
                output.append(x)
            mark_as_seen(x)

        return output


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.findDuplicates(list(map(int, line.split()))))
