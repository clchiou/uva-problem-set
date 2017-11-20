#!/usr/bin/env python3


#
# NOTE: You cannot use recursive descent parser as test data
# exceeds Python call stack limit.
#
# def parse_expr(tokens):
#     expr = parse_term(tokens)
#     while tokens and tokens[0] in '+-':
#         operator = tokens.pop(0)
#         expr = (operator, expr, parse_term(tokens))
#     return expr
#
# def parse_term(tokens):
#     term = parse_factor(tokens)
#     while tokens and tokens[0] in '*/':
#         operator = tokens.pop(0)
#         term = (operator, term, parse_factor(tokens))
#     return term
#
# def parse_factor(tokens):
#     assert isinstance(tokens[0], int)
#     return tokens.pop(0)
#
# def eval_expr(expr):
#     if isinstance(expr, int):
#         return expr
#     operator, operand1, operand2 = expr
#     operand1 = eval_expr(operand1)
#     operand2 = eval_expr(operand2)
#     if operator == '+':
#         return operand1 + operand2
#     elif operator == '-':
#         return operand1 - operand2
#     elif operator == '*':
#         return operand1 * operand2
#     else:
#         assert operator == '/'
#         return operand1 // operand2
#


class Solution:
    def calculate(self, s):
        """
        :type s: str
        :rtype: int
        """

        def token_stream():
            i = 0
            while i < len(s):
                if s[i].isspace():
                    i += 1
                elif s[i].isdigit():
                    j = i
                    while j < len(s) and s[j].isdigit():
                        j += 1
                    yield int(s[i:j])
                    i = j
                else:
                    assert s[i] in '+-*/'
                    yield s[i]
                    i += 1

        def parse():
            stack = []
            for token in token_stream():
                if isinstance(token, int):
                    yield token
                elif token in '+-':
                    while stack:
                        yield stack.pop()
                    stack.append(token)
                else:
                    assert token in '*/'
                    while stack and stack[-1] in '*/':
                        yield stack.pop()
                    stack.append(token)
            while stack:
                yield stack.pop()

        stack = []
        for token in parse():
            if isinstance(token, int):
                stack.append(token)
                continue
            op2 = stack.pop()
            op1 = stack.pop()
            if token == '+':
                stack.append(op1 + op2)
            elif token == '-':
                stack.append(op1 - op2)
            elif token == '*':
                stack.append(op1 * op2)
            else:
                assert token == '/'
                stack.append(op1 // op2)
        assert len(stack) == 1

        return stack[0]


if __name__ == '__main__':
    import sys
    solution = Solution()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        print(solution.calculate(line.strip()))
