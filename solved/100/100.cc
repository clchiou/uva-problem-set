#include <iostream>
#include <algorithm>

int ComputeCycle(int n) {
  int cycle;

  for (cycle = 1; n != 1; cycle++) {
    if (n % 2 == 1) {
      n = 3 * n + 1;
    } else {
      n = n / 2;
    }
  }

  return cycle;
}

int ComputeMaxCycle(int i, int j) {
  int n, max_cycle;

  if (i > j) {
    std::swap(i, j);
  }

  max_cycle = -1;
  for (n = i; n <= j; n++) {
    max_cycle = std::max(ComputeCycle(n), max_cycle);
  }

  return max_cycle;
}

int main() {
  int i, j;

  while (std::cin >> i >> j) {
    std::cout << i << " " << j << " " << ComputeMaxCycle(i, j) << std::endl;
  }

  return 0;
}
