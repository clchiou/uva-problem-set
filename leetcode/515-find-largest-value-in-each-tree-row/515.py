#!/usr/bin/env python3


class Solution:
    def largestValues(self, root):
        """
        :type root: TreeNode
        :rtype: List[int]
        """
        if not root:
            return []
        output = []
        queue = [root]
        while queue:
            next_queue = []
            max_of_row = None
            for node in queue:
                if max_of_row is None or node.val > max_of_row:
                    max_of_row = node.val
                if node.left:
                    next_queue.append(node.left)
                if node.right:
                    next_queue.append(node.right)
            output.append(max_of_row)
            queue = next_queue
        return output


if __name__ == '__main__':

    class TreeNode:

        def __init__(self, val, left, right):
            self.val = val
            self.left = left
            self.right = right

    root = TreeNode(
        1,
        TreeNode(
            3,
            TreeNode(
                5,
                None,
                None,
            ),
            TreeNode(
                3,
                None,
                None,
            ),
        ),
        TreeNode(
            2,
            None,
            TreeNode(
                9,
                None,
                None,
            ),
        ),
    )
    print(Solution().largestValues(root))
