#!/usr/bin/env python3


class Solution:
    def countBattleships(self, board):
        """
        :type board: List[List[str]]
        :rtype: int
        """
        num_ships = 0
        for i in range(len(board)):
            for j in range(len(board[i])):
                if board[i][j] != 'X':
                    pass
                elif i > 0 and board[i-1][j] == 'X':
                    pass
                elif j > 0 and board[i][j-1] == 'X':
                    pass
                else:
                    num_ships += 1
        return num_ships


if __name__ == '__main__':
    import sys
    board = list(map(list, sys.stdin.readlines()))
    print(Solution().countBattleships(board))
