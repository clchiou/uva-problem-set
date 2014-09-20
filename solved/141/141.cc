#include <bitset>
#include <iostream>
#include <unordered_set>

enum { MAX_SIZE = 50, MAX_SIZE_SQUARE = MAX_SIZE * MAX_SIZE, };

typedef std::bitset<MAX_SIZE_SQUARE> Board;

int ComputeIndex(int x, int y, int size) { return (x - 1) * size + (y - 1); }

Board Rotate(const Board& board, int size) {
  Board out;
  for (int x = 1; x <= size; x++) {
    for (int y = 1; y <= size; y++) {
      out[ComputeIndex(y, size - x + 1, size)] =
          board[ComputeIndex(x, y, size)];
    }
  }
  return out;
}

void DeclareWinner(int move) {
  int winner = move % 2 + 1;
  std::cout << "Player " << winner << " wins on move " << move << std::endl;
}

int main() {
  std::unordered_set<Board> boards;
  Board board, rotated;
  int size;
  while (std::cin >> size && size != 0) {
    boards.clear();
    board.reset();
    bool is_game_over = false;
    for (int move = 1; move <= size * 2; move++) {
      int x, y;
      std::string action;
      std::cin >> x >> y >> action;
      if (!is_game_over) {
        board.flip(ComputeIndex(x, y, size));
        rotated = board;
        for (int i = 0; i < 4; i++) {
          if (boards.find(rotated) != boards.end()) {
            DeclareWinner(move);
            is_game_over = true;
            break;
          }
          rotated = Rotate(rotated, size);
        }
        boards.insert(board);
      }
    }
    if (!is_game_over) {
      std::cout << "Draw" << std::endl;
    }
  }
  return 0;
}
