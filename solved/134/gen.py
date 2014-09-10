#!/usr/bin/env python

import itertools
import random
import sys


def main(argv):
    if len(argv) < 2:
        print >>sys.stderr, 'Usage: gen.py num'
        return 1

    vowels = 'aeiou'
    consonants = 'bcdfghjklmnpqrstvwxyz'
    alphabet = vowels + consonants
    candidates = list(vowels)
    candidates.extend(''.join(pair)
                      for pair in itertools.product('gbdl', vowels))
    candidates.append(1)
    candidates.append(2)
    candidates.append(3)

    def gen_name():
        length = random.randint(0, 100)
        letters = [random.choice(alphabet) for _ in xrange(length)]
        letters.append(random.choice(consonants))
        return ''.join(letters)

    def gen_pred_ccvcv():
        return ''.join([
            random.choice(consonants),
            random.choice(consonants),
            random.choice(vowels),
            random.choice(consonants),
            random.choice(vowels),
        ])

    def gen_pred_cvccv():
        return ''.join([
            random.choice(consonants),
            random.choice(vowels),
            random.choice(consonants),
            random.choice(consonants),
            random.choice(vowels),
        ])

    def gen():
        candidate = random.choice(candidates)
        if candidate == 1:
            return gen_name()
        elif candidate == 2:
            return gen_pred_ccvcv()
        elif candidate == 3:
            return gen_pred_cvccv()
        else:
            return candidate

    for _ in xrange(int(argv[1])):
        length = random.randint(0, 1e4)
        for _ in xrange(length):
            print gen(),
        print '%s.' % gen()
    print '#'
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
