#include <algorithm>
#include <iostream>
#include <cmath>

int ComputeNumPipesInGrid(double a, double b) { return int(a) * int(b); }

int ComputeNumPipesInSkew_Impl(double a, double b) {
  const double UNIT = sqrt(3) / 2;
  int width1 = a;
  int width2 = (width1 + 0.5 <= a) ? width1 : width1 - 1;
  int height = 1 + floor((b - 1) / UNIT);
  return (height / 2 + height % 2) * width1 + (height / 2) * width2;
}

int ComputeNumPipesInSkew(double a, double b) {
  return std::max(ComputeNumPipesInSkew_Impl(a, b),
                  ComputeNumPipesInSkew_Impl(b, a));
}

int main() {
  for (double a, b; std::cin >> a >> b;) {
    int num_pipes_in_grid = ComputeNumPipesInGrid(a, b);
    int num_pipes_in_skew = ComputeNumPipesInSkew(a, b);
    if (num_pipes_in_grid >= num_pipes_in_skew)
      std::cout << num_pipes_in_grid << " grid" << std::endl;
    else
      std::cout << num_pipes_in_skew << " skew" << std::endl;
  }
  return 0;
}
