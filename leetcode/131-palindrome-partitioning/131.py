#!/usr/bin/env python3


class Solution:
    def partition(self, s):
        """
        :type s: str
        :rtype: List[List[str]]
        """

        if not s:
            return []

        # p_indexes[j] = [i, ...] where s[i:j] is a palindrome.
        p_indexes = [[] for _ in range(len(s) + 1)]
        for k in range(len(s)):
            # Find odd-length palindrome.
            for diff in range(len(s)):
                if k - diff < 0 or k + diff >= len(s):
                    break
                if s[k - diff] != s[k + diff]:
                    break
                p_indexes[k + diff + 1].append(k - diff)
            # Find even-length palindrome.
            for diff in range(len(s)):
                if k - diff < 0 or k + diff + 1 >= len(s):
                    break
                if s[k - diff] != s[k + diff + 1]:
                    break
                p_indexes[k + diff + 2].append(k - diff)
        for indexes in p_indexes:
            indexes.sort()

        # p_partitions[j] is all palindrome partitions of s[:j].
        p_partitions = []
        for j in range(len(s) + 1):
            partitions = []
            # Divide s[:j] into s[:i] and s[i:j] where 0 <= i < j.
            for i in p_indexes[j]:
                if i > 0:
                    for partition in p_partitions[i]:
                        # Make a copy.
                        partition = list(partition)
                        partition.append(s[i:j])
                        partitions.append(partition)
                else:
                    partitions.append([s[:j]])
            p_partitions.append(partitions)

        return p_partitions[len(s)]


if __name__ == '__main__':
    solution = Solution()
    assert [] == solution.partition('')
    assert [['a']] == solution.partition('a')
    assert [['a', 'b']] == solution.partition('ab')
    assert [['aba'], ['a', 'b', 'a']] == solution.partition('aba')
    assert [['aa', 'b'], ['a', 'a', 'b']] == solution.partition('aab')
