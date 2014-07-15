#!/usr/bin/env python

'''Number theory utility.'''

import argparse
import os
import sys


class PrimeSequence:
    '''Generate a sequence of prime numbers.'''

    def __init__(self):
        '''__init__().'''
        self._sequence = [2, 3]

    def __iter__(self):
        '''__iter__().'''
        for prime in self._sequence:
            yield prime
        prime = self._sequence[-1]
        while True:
            prime += 2
            if is_prime(prime, self._sequence):
                self._sequence.append(prime)
                yield prime


def factorize(target, prime_sequence):
    '''Factorize target.'''
    if target == 1:
        return [(1, 1)]
    prime_factors = []
    remainder = target
    for prime in prime_sequence:
        if prime * prime > remainder:
            break
        exponent = 0
        while remainder % prime == 0:
            remainder /= prime
            exponent += 1
        if exponent > 0:
            prime_factors.append((prime, exponent))
    if remainder > 1:
        prime_factors.append((remainder, 1))
    return prime_factors


def is_prime(target, prime_sequence):
    '''Test if target is a prime number.'''
    if target == 1:
        return False
    for prime in prime_sequence:
        if prime * prime > target:
            break
        if target % prime == 0:
            return False
    return True


def list_factors(prime_factors):
    '''List factors.'''
    def list_factors_recursive(index, factor, factors):
        '''Implementation of list_factor().'''
        if index >= len(prime_factors):
            factors.append(factor)
            return
        for _ in xrange(prime_factors[index][1] + 1):
            list_factors_recursive(index + 1, factor, factors)
            factor *= prime_factors[index][0]
    factors = []
    list_factors_recursive(0, 1, factors)
    factors.sort()
    return factors


def program_factorize(argv):
    '''Prime factorize natural numbers.'''
    parser = argparse.ArgumentParser(description=
                                     'Prime factorize natural numbers.')
    parser.add_argument('n', type=int, nargs='+', help='Natural number(s)')
    args = parser.parse_args(argv[1:])
    prime_sequence = PrimeSequence()
    for target in args.n:
        prime_factors = factorize(target, prime_sequence)
        print ' * '.join('%d**%d' % (prime, exp) if exp > 1 else str(prime)
                         for prime, exp in prime_factors)
    return 0


def program_is_prime(argv):
    '''Test if a natural number is prime.'''
    parser = argparse.ArgumentParser(description=
                                     'Test if a natural number is prime.')
    parser.add_argument('n', type=int, help='Natural number')
    args = parser.parse_args(argv[1:])
    prime_sequence = PrimeSequence()
    if is_prime(args.n, prime_sequence):
        return 0  # shell's true == 0
    else:
        return 1  # shell's false == 1


def program_list_factors(argv):
    '''List factors of natural numbers.'''
    parser = argparse.ArgumentParser(description=
                                     'List factors of natural numbers.')
    parser.add_argument('n', type=int, nargs='+', help='Natural number(s)')
    args = parser.parse_args(argv[1:])
    prime_sequence = PrimeSequence()
    for target in args.n:
        if target == 1:
            print 1
            continue
        prime_factors = factorize(target, prime_sequence)
        print ', '.join(str(factor) for factor in list_factors(prime_factors))
    return 0


def program_next_prime(argv):
    '''Return next prime number.'''
    parser = argparse.ArgumentParser(description=
                                     'Generate next prime number.')
    parser.add_argument('n', type=int, nargs='+', help='Natural number(s)')
    args = parser.parse_args(argv[1:])
    prime_sequence = PrimeSequence()
    for target in args.n:
        if is_prime(target, prime_sequence):
            print target
            continue
        if target % 2 == 0:
            target -= 1
        while True:
            target += 2
            if is_prime(target, prime_sequence):
                break
        print target
    return 0


def main(argv):
    '''main().'''
    programs = {
        'factorize': program_factorize,
        'is-prime': program_is_prime,
        'list-factors': program_list_factors,
        'next-prime': program_next_prime,
    }
    return programs[os.path.basename(argv[0])](argv)


if __name__ == '__main__':
    sys.exit(main(sys.argv))
