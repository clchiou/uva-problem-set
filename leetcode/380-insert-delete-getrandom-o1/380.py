#!/usr/bin/env python3


import random


class RandomizedSet:

    def __init__(self):
        """
        Initialize your data structure here.
        """
        # Implement a bi-directional map.
        self._forward = {}
        self._backward = {}
        self._last = 0

    def insert(self, val):
        """
        Inserts a value to the set. Returns true if the set did not already contain the specified element.
        :type val: int
        :rtype: bool
        """
        if val in self._forward:
            return False
        self._forward[val] = self._last
        self._backward[self._last] = val
        self._last += 1
        return True

    def remove(self, val):
        """
        Removes a value from the set. Returns true if the set contained the specified element.
        :type val: int
        :rtype: bool
        """
        if val not in self._forward:
            return False
        x = self._forward.pop(val)
        prev_last = self._last - 1
        if x == prev_last:
            self._backward.pop(x)
        else:
            v = self._backward[prev_last]
            self._forward[v] = x
            self._backward[x] = v
        self._last = prev_last
        return True

    def getRandom(self):
        """
        Get a random element from the set.
        :rtype: int
        """
        if self._last == 0:
            raise AssertionError
        return self._backward[random.randint(0, self._last - 1)]


if __name__ == '__main__':
    rs = RandomizedSet()
    assert rs.insert(1)
    assert not rs.remove(2)
    assert rs.insert(2)
    assert rs.getRandom() in [1, 2]
    assert rs.remove(1)
    assert not rs.insert(2)
    assert rs.getRandom() in [2]
