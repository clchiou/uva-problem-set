#!/usr/bin/env python

'''Generate ugly numbers.'''

import heapq
import sys


def main(argv):
    '''main().'''
    if len(argv) < 2:
        print 'Usage: ./uglynumbers.py num_ugly_numbers'
        return 1

    num_ugly_numbers = int(argv[1])
    ugly_numbers = [(1, 0, 0, 0)]
    children = [(2, 1, 0, 0), (3, 0, 1, 0), (5, 0, 0, 1)]
    heapq.heapify(children)

    while len(ugly_numbers) < num_ugly_numbers:
        child = heapq.heappop(children)
        for multiple in (2, 3, 5):
            ugly_number, two, three, five = child
            ugly_number *= multiple
            if multiple == 2:
                two += 1
            elif multiple == 3:
                three += 1
            else:
                five += 1
            new_child = (ugly_number, two, three, five)
            if new_child not in children:
                heapq.heappush(children, new_child)
        ugly_numbers.append(child)

    for ugly_number in ugly_numbers:
        print '%d %d %d %d' % ugly_number
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
