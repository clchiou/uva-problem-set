#include <iostream>
#include <cmath>

int main() {
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(0);
  for (double n, p; std::cin >> n >> p;) {
    std::cout << pow(p, 1 / n) << std::endl;
  }
  return 0;
}
