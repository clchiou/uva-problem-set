#!/usr/bin/env python3


class Solution:
    def hIndex(self, citations):
        """
        :type citations: List[int]
        :rtype: int
        """

        if not citations:
            return 0

        h = 0
        left = 0
        right = len(citations)
        while left < right:
            mid = (left + right) // 2
            num_papers = len(citations) - mid
            num_citations = citations[mid]
            h = max(h, min(num_papers, num_citations))
            if num_papers == num_citations:
                break
            elif num_papers < num_citations:
                right = mid
            else:
                left = mid + 1

        return h


if __name__ == '__main__':
    solution = Solution()
    assert 3 == solution.hIndex([0, 1, 3, 5, 6])
    assert 0 == solution.hIndex([])
    assert 1 == solution.hIndex([99])
    assert 2 == solution.hIndex([98, 99])
    assert 3 == solution.hIndex([97, 98, 99])
    assert 4 == solution.hIndex([96, 97, 98, 99])
    assert 5 == solution.hIndex([95, 96, 97, 98, 99])
