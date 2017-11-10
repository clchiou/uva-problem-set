#!/usr/bin/env python3


class Solution:
    def complexNumberMultiply(self, a, b):
        """
        :type a: str
        :type b: str
        :rtype: str
        """

        def parse(complex_number_str):
            real, imaginary = complex_number_str.split('+', 1)
            return int(real), int(imaginary[:-1])

        a = parse(a)
        b = parse(b)
        c = (a[0] * b[0] - a[1] * b[1], a[0] * b[1] + a[1] * b[0])
        return '%d+%di' % c


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        a, b = line.split()
        print(solution.complexNumberMultiply(a, b))
