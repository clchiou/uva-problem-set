#!/usr/bin/env python3


class Solution:
    def superPow(self, a, b):
        """
        :type a: int
        :type b: List[int]
        :rtype: int
        """

        if not b:
            return 1

        output = 1
        for digit in b:
            # output = output ^ 10 % 1337
            x = output
            output = output * output % 1337
            output = output * output % 1337
            output = output * x % 1337
            output = output * output % 1337
            # output = output * a ^ digit % 1337
            for _ in range(digit):
                output = (output * a) % 1337
        return output


if __name__ == '__main__':
    solution = Solution()
    assert 1 == solution.superPow(1999, [0])
    assert 1024 == solution.superPow(2, [1, 0])
    assert (17 ** 1234 % 1337) == solution.superPow(17, [1, 2, 3, 4])

    import timeit

    with open('in') as testdata_file:
        a = int(testdata_file.readline())
        b = eval(testdata_file.readline())

    print(timeit.timeit(
        'solution.superPow(a, b)',
        number=100,
        globals=globals(),
    ))
