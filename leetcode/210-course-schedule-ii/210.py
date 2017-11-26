#!/usr/bin/env python3


from collections import defaultdict


class Solution:
    def findOrder(self, numCourses, prerequisites):
        """
        :type numCourses: int
        :type prerequisites: List[List[int]]
        :rtype: List[int]
        """

        graph = defaultdict(list)
        indegrees = defaultdict(int)
        for course, prerequisite in prerequisites:
            graph[prerequisite].append(course)
            indegrees[course] += 1

        output = []

        queue = [
            course for course in range(numCourses)
            if indegrees[course] == 0
        ]
        while queue:
            course = queue.pop(0)
            output.append(course)
            for c in graph[course]:
                indegrees[c] -= 1
                ind = indegrees[c]
                assert ind >= 0
                if ind == 0:
                    queue.append(c)

        if len(output) == numCourses:
            return output
        else:
            return []


if __name__ == '__main__':
    solution = Solution()

    assert [] == solution.findOrder(0, [])

    assert [0] == solution.findOrder(1, [])

    assert [0, 1] == solution.findOrder(2, [[1, 0]])

    expect = [[0, 1, 2, 3], [0, 2, 1, 3]]
    assert solution.findOrder(4, [[1, 0], [2, 0], [3, 1], [3, 2]]) in expect

    assert [] == solution.findOrder(2, [[1, 0], [0, 1]])

    expect = [[0, 2, 1], [2, 0, 1]]
    assert solution.findOrder(3, [[1, 0]]) in expect
