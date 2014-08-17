#include <algorithm>
#include <iostream>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

typedef std::vector<long long> LLVector;

int main() {
  // Define a min-heap.
  auto compare = [](long long p, long long q) { return q < p; };
  LLVector ugly_numbers{1};
  int count = 1500;  // Output 1500-th ugly number.
  while (--count > 0) {
    std::pop_heap(ugly_numbers.begin(), ugly_numbers.end(), compare);
    long long ugly_number = ugly_numbers.back();
    ugly_numbers.pop_back();
    if (DEBUG) {
      std::cerr << ugly_number << std::endl;
    }
    for (auto n : {2, 3, 5}) {
      long long new_ugly_number = ugly_number * n;
      if (std::find(ugly_numbers.cbegin(),
                    ugly_numbers.cend(),
                    new_ugly_number) == ugly_numbers.cend()) {
        ugly_numbers.push_back(new_ugly_number);
        std::push_heap(ugly_numbers.begin(), ugly_numbers.end(), compare);
      }
    }
  }
  std::cout << "The 1500'th ugly number is " << ugly_numbers.front() << "."
            << std::endl;
  return 0;
}
