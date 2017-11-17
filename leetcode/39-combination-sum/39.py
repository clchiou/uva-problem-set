#!/usr/bin/env python3


class Solution:
    def combinationSum(self, candidates, target):
        """
        :type candidates: List[int]
        :type target: int
        :rtype: List[List[int]]
        """

        from collections import defaultdict

        #
        # target_table[T] = [c, ...] where T is a target value, and c is
        # a candidate value (and you may recursively construct the
        # combination by looking target_table[T - c]).   If T = c, T is
        # in the input candidates list.
        #
        target_table = defaultdict(set)
        for c in candidates:
            target_table[c].add(c)

        queue = list(candidates)
        while queue:
            t = queue.pop(0)
            if t > target:
                continue
            for c in candidates:
                next_t = t + c
                if next_t > target:
                    continue
                target_table[next_t].add(c)
                queue.append(next_t)

        output = []

        def generate_combinations(t, combination):
            if t == 0:
                output.append(list(combination))
                return
            for c in sorted(target_table[t]):
                # Output should be monotonic (to prevent duplicates).
                if combination and c < combination[-1]:
                    continue
                combination.append(c)
                generate_combinations(t - c, combination)
                combination.pop()

        generate_combinations(target, [])

        return output


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        ns = list(map(int, line.split()))
        print(solution.combinationSum(ns[:-1], ns[-1]))
