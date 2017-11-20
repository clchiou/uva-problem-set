#!/usr/bin/env python3


class LinkedList:

    def __init__(self):
        self.head = None
        self.tail = None
        self.length = 0

    def __bool__(self):
        return len(self) > 0

    def __len__(self):
        return self.length

    def append(self, node):
        assert node.next is None
        if self.head:
            self.tail.next = node
            self.tail = node
        else:
            self.head = self.tail = node
        self.length += 1

    def popleft(self):
        assert self.head
        node = self.head
        self.head = self.head.next
        node.next = None
        self.length -= 1
        return node

    def disown(self):
        owned_data = (self.head, self.tail, self.length)
        self.head = None
        self.tail = None
        self.length = 0
        return owned_data

    def adopt(self, owned_data):
        head, tail, length = owned_data
        if self.head:
            self.tail.next = head
            self.tail = tail
            self.length += length
        else:
            self.head = head
            self.tail = tail
            self.length = length


class SubLinkedList:

    def __init__(self, parent, remaining):
        self.parent = parent
        self.remaining = remaining

    def __bool__(self):
        return len(self) > 0

    def __len__(self):
        return min(len(self.parent), self.remaining)

    @property
    def head(self):
        assert self
        return self.parent.head

    def popleft(self):
        assert self
        node = self.parent.popleft()
        self.remaining -= 1
        return node


def merge(output, unit, inputs):
    input0 = SubLinkedList(inputs[0], unit)
    input1 = SubLinkedList(inputs[1], unit)
    while input0 and input1:
        if input0.head.val < input1.head.val:
            output.append(input0.popleft())
        else:
            output.append(input1.popleft())
    input = input0 or input1
    assert len(input) <= unit
    while input:
        output.append(input.popleft())


class Solution:
    def sortList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """

        if not (head and head.next):
            return head

        sublists = (LinkedList(), LinkedList())
        sublist_index = 0

        # Base case: merge 1-element array.

        this = head
        while this and this.next:
            p = this
            q = this.next
            this = this.next.next
            p.next = q.next = None
            if p.val < q.val:
                sublists[sublist_index].append(p)
                sublists[sublist_index].append(q)
            else:
                sublists[sublist_index].append(q)
                sublists[sublist_index].append(p)
            sublist_index = (sublist_index + 1) % 2
        if this:
            sublists[sublist_index].append(this)

        # Bottom-up merge sort.

        next_sublists = (LinkedList(), LinkedList())
        unit = 2
        while sublists[0] and sublists[1]:
            assert abs(len(sublists[0]) - len(sublists[1])) <= unit
            next_sublist_index = 0
            while sublists[0] and sublists[1]:
                merge(next_sublists[next_sublist_index], unit, sublists)
                next_sublist_index = (next_sublist_index + 1) % 2
            sublist = sublists[0] or sublists[1]
            if sublist:
                assert len(sublist) <= unit
                next_sublists[next_sublist_index].adopt(sublist.disown())
            sublists, next_sublists = next_sublists, sublists
            unit *= 2

        return (sublists[0] or sublists[1]).head


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
