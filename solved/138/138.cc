#include <cmath>
#include <cstdio>

int main() {
  int count = 10;
  for (long long n = 2; count > 0; n++) {
    long long x = n * n + n;
    if ((x & 1) != 0) {
      continue;
    }
    double y = sqrt(x >> 1);
    if (floor(y) != y) {
      continue;
    }
    long long k = y;
    count--;
    printf("%10lld%10lld\n", k, n);
  }
  return 0;
}
