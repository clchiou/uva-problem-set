#include <algorithm>
#include <iostream>
#include <iomanip>

enum {
  CLOCKWISE = -1,
  COUNTER_CLOCKWISE = 1,
};

void Move(const bool doles[],
          int num_doles,
          int num_moves,
          int dir,
          int* position) {
  int pos = *position;
  while (num_moves > 0) {
    pos += dir;
    if (pos == 0)
      pos = num_doles;
    else if (pos == num_doles + 1)
      pos = 1;

    if (doles[pos])
      num_moves--;
  }
  *position = pos;
}

int main() {
  for (int num_doles, num_moves_1, num_moves_2;
       std::cin >> num_doles >> num_moves_1 >> num_moves_2 &&
       num_doles && num_moves_1 && num_moves_2;) {
    // Position starts from 1 to num_doles.
    bool doles[21];
    std::fill(doles, doles + num_doles + 1, true);

    for (int pos_1 = num_doles, pos_2 = 1, remaining = num_doles;
         remaining > 0;) {
      Move(doles, num_doles, num_moves_1, COUNTER_CLOCKWISE, &pos_1);
      Move(doles, num_doles, num_moves_2, CLOCKWISE, &pos_2);

      doles[pos_1] = doles[pos_2] = false;
      remaining--;
      if (pos_1 != pos_2)
        remaining--;

      if (pos_1 == pos_2)
        std::cout << std::setw(3) << pos_1;
      else
        std::cout << std::setw(3) << pos_1 << std::setw(3) << pos_2;
      if (remaining)
        std::cout << ',';
      else
        std::cout << std::endl;
    }
  }
  return 0;
}
