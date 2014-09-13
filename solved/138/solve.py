#!/usr/bin/env python3

import math
import sys


def main():
    count = 10
    n = 2
    while count > 0:
        k = math.sqrt((n * n + n) / 2)
        if math.trunc(k) == k:
            print('%10d%10d' % (k, n))
            count -= 1
        n += 1


if __name__ == '__main__':
    main()
