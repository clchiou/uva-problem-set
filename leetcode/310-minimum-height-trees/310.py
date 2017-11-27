#!/usr/bin/env python3


from collections import defaultdict


class Solution:
    def findMinHeightTrees(self, n, edges):
        """
        :type n: int
        :type edges: List[List[int]]
        :rtype: List[int]
        """

        graph = defaultdict(list)
        for i, j in edges:
            graph[i].append(j)
            graph[j].append(i)

        def find_distances(start):
            distances = [-1] * n
            distances[start] = 0
            i_max = start
            queue = [start]
            while queue:
                i = queue.pop(0)
                if distances[i] > distances[i_max]:
                    i_max = i
                for j in graph[i]:
                    if distances[j] == -1:
                        distances[j] = distances[i] + 1
                        queue.append(j)
            return distances, i_max

        # Compute distances from node 0.
        _, start = find_distances(0)

        # Compute distances from start (this finds the longest path).
        distances, end = find_distances(start)

        def find_path(i, target, visited, path):
            if i == target:
                return True
            for j in graph[i]:
                if j in visited:
                    continue
                visited.add(j)
                path.append(j)
                if find_path(j, target, visited, path):
                    return True
                path.pop()
                visited.remove(j)
            return False

        path = [start]
        okay = find_path(start, end, set(), path)
        assert okay
        assert len(path) == distances[end] + 1

        diameter = distances[end]

        output = [path[diameter // 2]]
        if diameter % 2 != 0:
            output.append(path[diameter // 2 + 1])
        output.sort()

        return output


if __name__ == '__main__':

    solution = Solution()

    assert [0] == solution.findMinHeightTrees(1, [])

    assert [0, 1] == solution.findMinHeightTrees(2, [[0, 1]])

    assert [1] == solution.findMinHeightTrees(3, [[0, 1], [1, 2]])

    assert [1] == solution.findMinHeightTrees(4, [[0, 1], [1, 2], [1, 3]])

    edges = [[0, 2], [1, 2], [2, 3], [3, 4]]
    assert [2, 3] == solution.findMinHeightTrees(5, edges)

    edges = [
        [0, 1],
        [0, 2],
        [0, 3],
        [3, 4],
        [3, 5],
        [1, 6],
        [4, 7],
        [2, 8],
        [0, 9],
        [0, 10],
        [2, 11],
    ]
    assert [0, 3] == solution.findMinHeightTrees(12, edges)
