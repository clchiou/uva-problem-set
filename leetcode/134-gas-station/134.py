#!/usr/bin/env python3


class Solution:
    def canCompleteCircuit(self, gas, cost):
        """
        :type gas: List[int]
        :type cost: List[int]
        :rtype: int
        """

        start = len(gas) - 1
        end = 0
        tank = gas[start] - cost[start]
        while start > end:
            if tank >= 0:
                # If we start from this `start`, we have enough gas to
                # drive all the way to `end`; let's move `end` one
                # station further and see if we can still reach it.
                tank += gas[end] - cost[end]
                end += 1
            else:
                # tank < 0 means that we cannot start from this `start`
                # point; let's move back one station and see if we can
                # start from there.
                start -= 1
                tank += gas[start] - cost[start]
        if tank >= 0:
            return start
        else:
            return -1


if __name__ == '__main__':
    solution = Solution()
    assert -1 == solution.canCompleteCircuit([0], [1])
    assert -1 == solution.canCompleteCircuit([1, 1], [1, 2])
    assert 0 == solution.canCompleteCircuit([1], [1])
    assert 1 == solution.canCompleteCircuit([1, 1], [1, 1])
    assert 2 == solution.canCompleteCircuit([1, 1, 1], [1, 1, 1])
    assert 1 == solution.canCompleteCircuit([1, 2], [2, 1])
    assert 1 == solution.canCompleteCircuit([1, 2, 3], [2, 1, 3])
    assert -1 == solution.canCompleteCircuit([1, 1, 2], [2, 2, 1])
    assert 2 == solution.canCompleteCircuit([1, 1, 3], [2, 2, 1])
