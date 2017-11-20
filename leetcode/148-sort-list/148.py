#!/usr/bin/env python3


#
# NOTE: You cannot use recursion because stack growth is not
# constant space complexity.
#


class Solution:
    def sortList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """

        if not head:
            return None

        if not head.next:
            return head

        length = 0
        this = head
        while this:
            length += 1
            this = this.next

        #
        # Bottom-up merge sort.
        #

        def merge(left, right, unit):
            assert left and right

            head = tail = None
            i = j = 0
            while left and right and i < unit and j < unit:
                if left.val < right.val:
                    node = left
                    left = left.next
                    i += 1
                else:
                    node = right
                    right = right.next
                    j += 1
                if head:
                    tail.next = node
                    tail = tail.next
                else:
                    head = tail = node

            if left and i < unit:
                assert not (right and j < unit)
                tail.next = left
                while left and i < unit:
                    tail = left
                    left = left.next
                    i += 1

            if right and j < unit:
                tail.next = right
                while right and j < unit:
                    tail = right
                    right = right.next
                    j += 1

            assert head
            return head, tail, left, right

        unit = 1
        while unit < length:

            # Find multiple of unit close to half.
            prev = None
            this = head
            for _ in range(max(unit, length // unit // 2 * unit)):
                prev = this
                this = this.next

            left_tail = prev
            right_head = this

            # Break up left half and right half.
            left_tail.next = None

            tail = None
            left = head
            right = right_head

            while left or right:

                merged_head, merged_tail, next_left, next_right = \
                    merge(left, right, unit)

                if left is head:
                    head = merged_head

                if tail:
                    tail.next = merged_head
                tail = merged_tail

                left = next_left
                right = next_right

                # One half is empty; let's try to balance it.
                if not left and right:
                    left = right
                    for _ in range(unit - 1):
                        right = right.next
                        if right is None:
                            break
                    if right:
                        this = right
                        right = right.next
                        this.next = None
                elif left and not right:
                    right = left
                    for _ in range(unit - 1):
                        left = left.next
                        if left is None:
                            break
                    if left:
                        this = left
                        left = left.next
                        this.next = None

                # If there is really nothing left for merging.
                if not left:
                    tail.next = right
                    break
                if not right:
                    tail.next = left
                    break

            unit *= 2

        return head


if __name__ == '__main__':

    import random

    class ListNode:

        @classmethod
        def from_list(cls, list_):
            head = tail = None
            for val in list_:
                node = cls(val)
                if head:
                    tail.next = node
                    tail = tail.next
                else:
                    head = tail = node
            return head

        def __init__(self, val):
            self.val = val
            self.next = None

        def __str__(self):
            return '<%d%s>' % (self.val, ', ...' if self.next else '')

        def to_list(self, bound=-1):
            list_ = []
            node = self
            while node:
                list_.append(node.val)
                node = node.next
                if bound > 0 and len(list_) > bound:
                    list_.append('...')
                    break
            return list_

    def assert_eq(expect, actual):
        if expect != actual:
            raise AssertionError('expect %r, not %r' % (expect, actual))

    solution = Solution()

    assert_eq(None, solution.sortList(ListNode.from_list([])))
    assert_eq([1], solution.sortList(ListNode.from_list([1])).to_list())
    assert_eq([1, 2], solution.sortList(ListNode.from_list([1, 2])).to_list())
    assert_eq([1, 2], solution.sortList(ListNode.from_list([2, 1])).to_list())
    assert_eq(
        [1, 2, 3],
        solution.sortList(ListNode.from_list([2, 1, 3])).to_list(),
    )
    assert_eq(
        [1, 2, 3, 4],
        solution.sortList(ListNode.from_list([2, 1, 4, 3])).to_list(),
    )

    random.seed(7)
    num_repeat = 10
    for n in range(4, 128):
        for _ in range(num_repeat):
            testdata = [random.randint(0, 1000) for _ in range(n)]
            expect = sorted(testdata)
            assert_eq(
                expect,
                solution.sortList(ListNode.from_list(testdata)).to_list(),
            )

    with open('in') as testdata_file:
        expect = eval(testdata_file.read())
    testdata = ListNode.from_list(expect)
    expect.sort()
    assert_eq(expect, solution.sortList(testdata).to_list())
