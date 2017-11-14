#!/usr/bin/env python3


class Solution:
    def countOfAtoms(self, formula):
        """
        :type formula: str
        :rtype: str
        """

        from collections import Counter

        def parse(formula, offset, atom_counter):
            i = offset
            while i < len(formula):
                if formula[i].isupper():
                    # Parse atom name.
                    j = i + 1
                    while j < len(formula) and formula[j].islower():
                        j += 1
                    atom = formula[i:j]
                    # Parse optional atom number.
                    k = j
                    while k < len(formula) and formula[k].isdigit():
                        k += 1
                    num_atoms = 1
                    if k > j:
                        num_atoms = int(formula[j:k])
                    # Record what we find and move on.
                    atom_counter[atom] += num_atoms
                    i = k
                elif formula[i] == '(':
                    # Parse sub-formula.
                    sub_formula_atom_counter = Counter()
                    i = parse(formula, i + 1, sub_formula_atom_counter)
                    # Parse optional multiplication.
                    j = i
                    while j < len(formula) and formula[j].isdigit():
                        j += 1
                    multiplication = 1
                    if j > i:
                        multiplication = int(formula[i:j])
                    # Record what we find and move on.
                    for atom, num_atoms in sub_formula_atom_counter.items():
                        atom_counter[atom] += multiplication * num_atoms
                    i = j
                elif formula[i] == ')':
                    # End of sub-formula.
                    i += 1
                    break
                else:
                    raise AssertionError
            return i

        atom_counter = Counter()
        parse(formula, 0, atom_counter)

        pieces = []
        for atom in sorted(atom_counter):
            pieces.append(atom)
            if atom_counter[atom] > 1:
                pieces.append(str(atom_counter[atom]))
        return ''.join(pieces)


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.countOfAtoms(line.strip()))
