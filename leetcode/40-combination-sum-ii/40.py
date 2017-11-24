#!/usr/bin/env python3


class Solution:
    def combinationSum2(self, candidates, target):
        """
        :type candidates: List[int]
        :type target: int
        :rtype: List[List[int]]
        """

        candidates.sort()

        def find(candidates, target, combination, output):
            if target == 0:
                output.append(list(combination))
                return
            last_candidate = None
            for i in range(len(candidates)):
                candidate = candidates[i]
                if candidate is None or target < candidate:
                    continue
                if combination and combination[-1] > candidate:
                    continue  # Remove non-monotonic combination.
                if candidate == last_candidate:
                    continue  # Remove duplicates.
                candidates[i] = None
                combination.append(candidate)
                find(candidates, target - candidate, combination, output)
                combination.pop()
                candidates[i] = candidate
                last_candidate = candidate

        output = []
        find(candidates, target, [], output)

        return output


if __name__ == '__main__':
    solution = Solution()
    print(solution.combinationSum2([10, 1, 2, 7, 6, 1, 5], 8))
