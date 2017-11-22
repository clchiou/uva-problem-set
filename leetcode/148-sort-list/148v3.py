#!/usr/bin/env python3


def merge(
    sublist0_head,
    sublist0_tail,
    sublist1_head,
    sublist1_tail,
    next_sublist_head,
    next_sublist_tail,
    unit):

    i = j = 0
    while sublist0_head and i < unit and sublist1_head and j < unit:
        if sublist0_head.val < sublist1_head.val:
            node = sublist0_head
            sublist0_head = sublist0_head.next
            i += 1
        else:
            node = sublist1_head
            sublist1_head = sublist1_head.next
            j += 1
        if next_sublist_head:
            next_sublist_tail.next = node
            next_sublist_tail = node
        else:
            next_sublist_head = next_sublist_tail = node

    for _ in range(i, unit):
        if not sublist0_head:
            break
        if next_sublist_head:
            next_sublist_tail.next = sublist0_head
            next_sublist_tail = sublist0_head
        else:
            next_sublist_head = next_sublist_tail = sublist0_head
        sublist0_head = sublist0_head.next

    if not sublist0_head:
        sublist0_tail = None

    for _ in range(j, unit):
        if not sublist1_head:
            break
        if next_sublist_head:
            next_sublist_tail.next = sublist1_head
            next_sublist_tail = sublist1_head
        else:
            next_sublist_head = next_sublist_tail = sublist1_head
        sublist1_head = sublist1_head.next

    if not sublist1_head:
        sublist1_tail = None

    next_sublist_tail.next = None

    return (
        sublist0_head,
        sublist0_tail,
        sublist1_head,
        sublist1_tail,
        next_sublist_head,
        next_sublist_tail,
    )


class Solution:
    def sortList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """

        if not (head and head.next):
            return head

        # Base case: merge 1-element array.

        sublist0_head = None
        sublist0_tail = None

        sublist1_head = None
        sublist1_tail = None

        sublist_index = 0

        this = head
        while this and this.next:

            if sublist_index == 0:
                sublist_head = sublist0_head
                sublist_tail = sublist0_tail
            else:
                sublist_head = sublist1_head
                sublist_tail = sublist1_tail

            p = this
            q = this.next
            this = this.next.next
            if p.val >= q.val:
                p, q = q, p

            p.next = q
            q.next = None
            if sublist_head:
                sublist_tail.next = p
            else:
                sublist_head = p
            sublist_tail = q

            if sublist_index == 0:
                sublist0_head = sublist_head
                sublist0_tail = sublist_tail
            else:
                sublist1_head = sublist_head
                sublist1_tail = sublist_tail

            sublist_index = (sublist_index + 1) % 2

        if this:
            if sublist_index == 0:
                if sublist0_head:
                    sublist0_tail.next = this
                    sublist0_tail = this
                else:
                    sublist0_head = sublist0_tail = this
            else:
                if sublist1_head:
                    sublist1_tail.next = this
                    sublist1_tail = this
                else:
                    sublist1_head = sublist1_tail = this

        # Bottom-up merge sort.

        unit = 2

        while sublist0_head and sublist1_head:

            next_sublist0_head = None
            next_sublist0_tail = None

            next_sublist1_head = None
            next_sublist1_tail = None

            next_sublist_index = 0

            while sublist0_head and sublist1_head:

                if next_sublist_index == 0:
                    next_sublist_head = next_sublist0_head
                    next_sublist_tail = next_sublist0_tail
                else:
                    next_sublist_head = next_sublist1_head
                    next_sublist_tail = next_sublist1_tail

                sublist0_head, sublist0_tail, \
                sublist1_head, sublist1_tail, \
                next_sublist_head, next_sublist_tail = merge(
                    sublist0_head, sublist0_tail,
                    sublist1_head, sublist1_tail,
                    next_sublist_head, next_sublist_tail,
                    unit,
                )

                if next_sublist_index == 0:
                    next_sublist0_head = next_sublist_head
                    next_sublist0_tail = next_sublist_tail
                else:
                    next_sublist1_head = next_sublist_head
                    next_sublist1_tail = next_sublist_tail

                next_sublist_index = (next_sublist_index + 1) % 2

            if sublist0_head:
                if next_sublist_index == 0:
                    if next_sublist0_head:
                        next_sublist0_tail.next = sublist0_head
                        next_sublist0_tail = sublist0_tail
                    else:
                        next_sublist0_head = sublist0_head
                        next_sublist0_tail = sublist0_tail
                else:
                    if next_sublist1_head:
                        next_sublist1_tail.next = sublist0_head
                        next_sublist1_tail = sublist0_tail
                    else:
                        next_sublist1_head = sublist0_head
                        next_sublist1_tail = sublist0_tail

            if sublist1_head:
                if next_sublist_index == 0:
                    if next_sublist0_head:
                        next_sublist0_tail.next = sublist1_head
                        next_sublist0_tail = sublist1_tail
                    else:
                        next_sublist0_head = sublist1_head
                        next_sublist0_tail = sublist1_tail
                else:
                    if next_sublist1_head:
                        next_sublist1_tail.next = sublist1_head
                        next_sublist1_tail = sublist1_tail
                    else:
                        next_sublist1_head = sublist1_head
                        next_sublist1_tail = sublist1_tail

            sublist0_head = next_sublist0_head
            sublist0_tail = next_sublist0_tail

            sublist1_head = next_sublist1_head
            sublist1_tail = next_sublist1_tail

            unit *= 2

        return sublist0_head or sublist1_head


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
