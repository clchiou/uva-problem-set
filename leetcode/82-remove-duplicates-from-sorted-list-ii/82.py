#!/usr/bin/env python3


class Solution:
    def deleteDuplicates(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """

        if not head:
            return None

        delete_value = None
        prev_node = None
        this_node = head
        next_node = head.next
        while this_node:
            if this_node.val == delete_value:
                if prev_node:
                    prev_node.next = next_node
                else:
                    head = next_node
                this_node = next_node
                if this_node:
                    next_node = this_node.next
            elif next_node:
                if this_node.val == next_node.val:
                    delete_value = this_node.val
                else:
                    prev_node = this_node
                    this_node = next_node
                    next_node = next_node.next
            else:
                this_node = None

        return head


if __name__ == '__main__':

    class ListNode:
        def __init__(self, x, next):
            self.val = x
            self.next = next

    def tolist(head):
        if head:
            return [head.val] + tolist(head.next)
        else:
            return []

    solution = Solution()

    print(tolist(solution.deleteDuplicates(
        ListNode(1, None)
    )))

    print(tolist(solution.deleteDuplicates(
        ListNode(1, ListNode(1, None))
    )))

    print(tolist(solution.deleteDuplicates(
        ListNode(1, ListNode(2, ListNode(2, None)))
    )))

    print(tolist(solution.deleteDuplicates(
        ListNode(1, ListNode(1, ListNode(1, ListNode(2, ListNode(3, None)))))
    )))

    print(tolist(solution.deleteDuplicates(
        ListNode(1, ListNode(2, ListNode(3, ListNode(3, ListNode(4, ListNode(4, ListNode(5, None)))))))
    )))
