#!/usr/bin/env python3


class Solution:
    def findBottomLeftValue(self, root):
        """
        :type root: TreeNode
        :rtype: int
        """
        leftmost = root.val
        queue = [root]
        while queue:
            leftmost = queue[0].val
            next_queue = []
            for node in queue:
                if node.left:
                    next_queue.append(node.left)
                if node.right:
                    next_queue.append(node.right)
            queue = next_queue
        return leftmost


class TreeNode:

    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

    def __repr__(self):
        return '(%d %r %r)' % (self.val, self.left or (), self.right or ())


if __name__ == '__main__':
    solution = Solution()

    root = TreeNode(2, TreeNode(1), TreeNode(3))
    print(root)
    print(solution.findBottomLeftValue(root))

    root = TreeNode(
        1,
        TreeNode(
            2,
            TreeNode(4),
            None,
        ),
        TreeNode(
            3,
            TreeNode(
                5,
                TreeNode(7),
                None,
            ),
            TreeNode(6),
        ),
    )
    print(root)
    print(solution.findBottomLeftValue(root))
