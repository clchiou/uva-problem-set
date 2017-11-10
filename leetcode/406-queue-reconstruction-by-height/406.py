#!/usr/bin/env python3


class Solution:
    def reconstructQueue(self, people):
        """
        :type people: List[List[int]]
        :rtype: List[List[int]]
        """

        heights = list(set(p[0] for p in people))
        heights.sort()
        heights.reverse()

        output = []
        for height in heights:
            group = [p for p in people if p[0] == height]
            group.sort(key=lambda p: p[1])
            for person in group:
                num_higher = 0
                for i in range(len(output)):
                    if num_higher == person[1]:
                        output.insert(i, person)
                        break
                    if output[i][0] >= person[0]:
                        num_higher += 1
                else:
                    output.append(person)

        return output


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        ns = list(map(int, line.split()))
        people = [ns[i:i+2] for i in range(0, len(ns), 2)]
        print(solution.reconstructQueue(people))
