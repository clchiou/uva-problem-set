#!/usr/bin/env python3


class Solution:
    def rob(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """

        if len(nums) <= 2:
            return max(nums, default=0)

        #
        # m0[i] is the max payoff from house 0 to house i
        # where i <= n - 2 (the house n - 1 is excluded).
        #
        # We could use a slide window for m0 to save space since we do
        # not actually care about earlier values, but meh.
        #
        m0 = [nums[0]]
        for i in range(1, len(nums) - 1):
            # Either rob house i.
            if i >= 2:
                p = m0[i - 2] + nums[i]
            else:
                p = nums[i]
            # Or not.
            q = m0[i - 1]
            m0.append(max(p, q))

        #
        # m1[i] is the max payoff from house 1 to house i
        # where i <= n - 1 (the house 0 is excluded).
        #
        m1 = [None, nums[1]]
        for i in range(2, len(nums)):
            # Either rob house i.
            if i >= 3:
                p = m1[i - 2] + nums[i]
            else:
                p = nums[i]
            # Or not.
            q = m1[i - 1]
            m1.append(max(p, q))

        # The maximum value is either from house 0 to house n - 2, or
        # from house 1 to house n - 1.
        return max(m0[-1], m1[-1])


if __name__ == '__main__':
    solution = Solution()
    assert 0 == solution.rob([])
    assert 9 == solution.rob([9])
    assert 9 == solution.rob([3, 9])
    assert 1 == solution.rob([1, 1, 0])
    assert 1 == solution.rob([1, 0, 1])
    assert 1 == solution.rob([0, 1, 1])
    assert 1 == solution.rob([1, 0, 0, 1])
    assert 1 == solution.rob([0, 1, 1, 0])
    assert 3 == solution.rob([1, 0, 2, 0])
    assert 3 == solution.rob([0, 1, 0, 2])
