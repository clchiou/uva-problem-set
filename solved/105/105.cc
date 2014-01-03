#include <algorithm>
#include <iostream>

const int MAX_POSITION = 10000;

int main() {
  int x_max = 0;
  int heights[MAX_POSITION];
  std::fill(heights, heights + MAX_POSITION, 0);

  for (int left, height, right; std::cin >> left >> height >> right;) {
    for (int x = left; x < right; x++)
      heights[x] = std::max(heights[x], height);
    x_max = std::max(x_max, right);
  }

  bool first = true;
  for (int x = 0, x_next; x <= x_max; x = x_next) {
    for (x_next = x + 1; x_next <= x_max; x_next++) {
      if (heights[x_next] != heights[x])
        break;
    }
    if (x_next <= x_max) {
      if (!first)
        std::cout << ' ';
      std::cout << x_next << ' ' << heights[x_next];
      first = false;
    }
  }
  std::cout << std::endl;

  return 0;
}
