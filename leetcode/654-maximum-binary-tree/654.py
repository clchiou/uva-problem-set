#!/usr/bin/env python3


class Solution:
    def constructMaximumBinaryTree(self, nums):
        """
        :type nums: List[int]
        :rtype: TreeNode
        """

        def find_max(array):
            assert array
            return max(enumerate(array), key=lambda p: p[1])

        def solve(nums):
            if not nums:
                return None
            i_max, n_max = find_max(nums)
            root = TreeNode(n_max)
            root.left = solve(nums[:i_max])
            root.right = solve(nums[i_max+1:])
            return root

        return solve(nums)


class TreeNode:

    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None

    def __repr__(self):
        return '(%d %r %r)' % (self.val, self.left or (), self.right or ())


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        nums = list(map(int, line.split()))
        print(solution.constructMaximumBinaryTree(nums))
