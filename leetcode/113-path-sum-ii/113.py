#!/usr/bin/env python3


class Solution:
    def pathSum(self, root, sum):
        """
        :type root: TreeNode
        :type sum: int
        :rtype: List[List[int]]
        """

        if not root:
            return []

        def solve(node, remaining, path, output):
            remaining -= node.val
            if not node.left and not node.right:
                # This is a leaf.
                if remaining == 0:
                    output.append(list(path))
                    output[-1].append(node.val)
            else:
                path.append(node.val)
                for child in (node.left, node.right):
                    if child:
                        solve(child, remaining, path, output)
                path.pop()

        output = []
        solve(root, sum, [], output)
        return output


if __name__ == '__main__':

    class TreeNode:
        def __init__(self, x):
            self.val = x
            self.left = None
            self.right = None

    solution = Solution()

    assert [] == solution.pathSum(None, 0)

    root = TreeNode(1)
    root.left = TreeNode(2)
    root.right = TreeNode(3)
    assert [] == solution.pathSum(root, 1)
    assert [] == solution.pathSum(root, 2)
    assert [[1, 2]] == solution.pathSum(root, 3)
    assert [[1, 3]] == solution.pathSum(root, 4)

    root = TreeNode(-2)
    root.right = TreeNode(-3)
    assert [[-2, -3]] == solution.pathSum(root, -5)
