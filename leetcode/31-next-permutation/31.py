#!/usr/bin/env python3


class Solution:
    def nextPermutation(self, nums):
        """
        :type nums: List[int]
        :rtype: void Do not return anything, modify nums in-place instead.
        """

        for i in range(len(nums) - 1, 0, -1):
            if nums[i-1] < nums[i]:
                break
        else:
            nums.sort()
            return

        j_min = i
        for j in range(i, len(nums)):
            if nums[i-1] < nums[j] < nums[j_min]:
                j_min = j

        nums[i-1], nums[j_min] = nums[j_min], nums[i-1]

        nums[i:] = sorted(nums[i:])


if __name__ == '__main__':
    solution = Solution()

    def run(list_):
        solution.nextPermutation(list_)
        return list_

    assert [] == run([])

    assert [1] == run([1])

    assert [1, 2] == run([2, 1])

    assert [1, 3, 2] == run([1, 2, 3])
    assert [2, 1, 3] == run([1, 3, 2])
    assert [2, 3, 1] == run([2, 1, 3])
    assert [3, 1, 2] == run([2, 3, 1])
    assert [3, 2, 1] == run([3, 1, 2])
    assert [1, 2, 3] == run([3, 2, 1])
