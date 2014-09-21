#!/usr/bin/env python3

import random
import sys


def main():
    for _ in range(25):
        x1 = random.randint(0, 499)
        y1 = random.randint(0, 499)
        x2 = random.randint(x1, 499)
        y2 = random.randint(y1, 499)
        print('R', x1, y1, x2, y2)
    for _ in range(50):
        print('I', random.randint(0, 499), random.randint(0, 499))
    for x in range(499 + 1):
        for y in range(499 + 1):
            print('M', x, y)
    print('#')


if __name__ == '__main__':
    main()
