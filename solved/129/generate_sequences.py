#!/usr/bin/env python

'''Generate test data.'''


def is_hard_sequence(sequence):
    '''Test if the sequence is "hard".'''
    length = len(sequence)
    for step in xrange(1, length + 1):
        if step * 2 > length:
            break
        for i in xrange(length):
            j = i + step
            if j + step > length:
                break
            if sequence[i:i+step] == sequence[j:j+step]:
                return False
    return True


def generate(elements, num_sequences, sequence_index, sequence,
             problem_input, problem_output):
    '''Generate sequences.'''
    if sequence_index > num_sequences:
        return sequence_index

    sequence.append(None)
    for element in elements:
        sequence[-1] = element
        if not is_hard_sequence(sequence):
            continue

        sequence_index += 1
        if sequence_index > num_sequences:
            break

        problem_input.write('%d %d\n' % (sequence_index, len(elements)))
        for i in xrange(0, len(sequence), 4):
            if i > 0 and i % 64 == 0:
                problem_output.write('\n')
            elif i % 64 != 0:
                problem_output.write(' ')
            problem_output.write(''.join(sequence[i:i + 4]))
        problem_output.write('\n%d\n' % len(sequence))

        sequence_index = generate(elements, num_sequences,
                                  sequence_index, sequence,
                                  problem_input, problem_output)
        if sequence_index > num_sequences:
            break

    sequence.pop()
    return sequence_index


def main():
    '''Main function.'''
    num_sequences = int(raw_input('num_sequences = '))
    num_elements = int(raw_input('num_elements = '))
    problem_input_path = raw_input('problem_input = ')
    problem_output_path = raw_input('problem_output = ')
    elements = [chr(ord('A') + i) for i in xrange(num_elements)]
    with open(problem_input_path, 'w') as problem_input:
        with open(problem_output_path, 'w') as problem_output:
            generate(elements, num_sequences, 0, [],
                     problem_input, problem_output)
        problem_input.write('0 0\n')

if __name__ == '__main__':
    main()
