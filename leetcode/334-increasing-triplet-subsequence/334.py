#!/usr/bin/env python3


class Solution:
    def increasingTriplet(self, nums):
        """
        :type nums: List[int]
        :rtype: bool
        """

        #
        # NOTE: Instead of tracking i, j, and k (then you need to
        # backtrace when the condition is not met), you may track the
        # smallest two values you have ever seen, and if you see a third
        # value greater than the smallest two value, you have found the
        # subsequence.
        #
        # Here is the trick that I cannot explain fully: the "ever seen"
        # part makes these two values somewhat encode the information of
        # the past values necessary to find the subseqnence; that is the
        # key of this algorithm.
        #

        s0 = float('inf')
        s1 = float('inf')
        for x in nums:
            if x <= s0:
                s0 = x
            elif x <= s1:
                s1 = x
            else:
                return True

        return False
