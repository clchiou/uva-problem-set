#include <algorithm>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

enum Color {
  BROWN,
  GREEN,
  CLEAR
};

class Bin {
 public:
  Bin() : brown_(0), green_(0), clear_(0) {}
  Bin(int brown, int green, int clear)
      : brown_(brown), green_(green), clear_(clear) {}

  friend std::istream& operator>>(std::istream& in, Bin& bin) {
    in >> bin.brown_ >> bin.green_ >> bin.clear_;
    return in;
  }

  int CountMoveOut(Color color) const {
    int move = 0;
    if (color != BROWN)
      move += brown_;
    if (color != GREEN)
      move += green_;
    if (color != CLEAR)
      move += clear_;
    return move;
  }

 private:
  int brown_, green_, clear_;
};

int CountMove(const Bin bins[3], const Color order[3]) {
  int move = 0;
  for (int i = 0; i < 3; i++) {
    move += bins[i].CountMoveOut(order[i]);
  }
  return move;
}

void FindMinMove(const Bin bins[3], int* min_move_out, Color min_order[3]) {
  // Alphabetical order.
  const Color all_colors[] = {BROWN, CLEAR, GREEN};
  const int permutations[][3] = {
    { 0, 1, 2 },
    { 0, 2, 1 },
    { 1, 0, 2 },
    { 1, 2, 0 },
    { 2, 0, 1 },
    { 2, 1, 0 },
  };
  const int size = sizeof(permutations) / sizeof(permutations[0]);

  const int* min_p = NULL;
  int min_move = 0;
  for (int i = 0; i < size; i++) {
    const Color order[3] = {
      all_colors[permutations[i][0]],
      all_colors[permutations[i][1]],
      all_colors[permutations[i][2]],
    };
    int move = CountMove(bins, order);
    if (!min_p || move < min_move) {
      min_p = permutations[i];
      min_move = move;
    }
  }

  for (int j = 0; j < 3; j++) {
    min_order[j] = all_colors[min_p[j]];
  }
  *min_move_out = min_move;
}

char ToChar(Color color) {
  switch (color) {
    case BROWN:
      return 'B';
    case GREEN:
      return 'G';
    case CLEAR:
      return 'C';
  }
  assert(0);
  return '\0';
}

int main() {
  Bin bins[3];

  while (std::cin >> bins[0] >> bins[1] >> bins[2]) {
    int min_move = 0;
    Color min_order[3];
    FindMinMove(bins, &min_move, min_order);
    std::cout
      << ToChar(min_order[0])
      << ToChar(min_order[1])
      << ToChar(min_order[2])
      << ' '
      << min_move
      << std::endl;
  }

  return 0;
}
