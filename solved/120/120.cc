#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

const int MAX_NUM_PANCAKES = 30;
const int MAX_DIAMETER = 100;

class PancakeStack {
 public:
  friend std::istream& operator>>(std::istream& in, PancakeStack& stack);
  friend std::ostream& operator<<(std::ostream& out, const PancakeStack& stack);

  PancakeStack() {}
  PancakeStack(const PancakeStack& other) : pancakes_(other.pancakes_) {}

  int num_pancakes() const { return pancakes_.size(); }
  int diameter(int i) const {
    assert(0 <= i && i < static_cast<int>(pancakes_.size()));
    return pancakes_[i];
  }

  typedef std::vector<int>::const_iterator const_iterator;
  const_iterator begin() const { return pancakes_.begin(); }
  const_iterator end() const { return pancakes_.end(); }

  // Insert the spatula between i-th and (i+1)-th pancake; reverse from
  // pancakes 0 to pancake i (both ends are inclusive).
  void Flip(int i) {
    std::reverse(pancakes_.begin(), pancakes_.begin() + i + 1);
  }

  void Sort() { std::sort(pancakes_.begin(), pancakes_.end()); }

 private:
  // Top pancake at index 0, not n.
  std::vector<int> pancakes_;
};

class Flips {
 public:
  explicit Flips(int num_pancakes) : num_pancakes_(num_pancakes) {}

  // Top pancake at index 0, convert to position n.
  void Append(int i);

  typedef std::vector<int>::const_iterator const_iterator;
  const_iterator begin() const { return flips_.begin(); }
  const_iterator end() const { return flips_.end(); }

 private:
  int num_pancakes_;
  std::vector<int> flips_;
};

class PositionMap {
 public:
  // Work on copy of stack, not the original one.
  explicit PositionMap(PancakeStack stack);

  int pos(int diameter) const;

 private:
  int num_pancakes_;
  int positions_[MAX_DIAMETER];
};

// Work on copy of stack, not the original one.
void Solve(PancakeStack stack, Flips* flips) {
  PositionMap pos_map(stack);
  while (1) {
    int i_max = -1, d_max = -1;
    for (int i = 0; i < stack.num_pancakes(); i++) {
      int d = stack.diameter(i);
      if (pos_map.pos(d) != i && d > d_max) {
        i_max = i;
        d_max = d;
      }
    }
    if (i_max == -1)
      break;

    if (i_max > 0) {
      stack.Flip(i_max);
      flips->Append(i_max);
    }
    stack.Flip(pos_map.pos(d_max));
    flips->Append(pos_map.pos(d_max));
  }
}

int main() {
  PancakeStack stack;
  while (std::cin >> stack) {
    Flips flips(stack.num_pancakes());
    Solve(stack, &flips);
    std::cout << stack << std::endl;
    for (Flips::const_iterator i = flips.begin(); i != flips.end(); ++i)
      std::cout << *i << ' ';
    std::cout << 0 << std::endl;
  }
  return 0;
}

std::istream& operator>>(std::istream& in, PancakeStack& stack) {
  std::string line;
  stack.pancakes_.clear();
  if (std::getline(in, line)) {
    std::stringstream sin(line);
    int diameter;
    while (sin >> diameter) {
      assert(1 <= diameter && diameter <= MAX_DIAMETER);
      stack.pancakes_.push_back(diameter);
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const PancakeStack& stack) {
  for (PancakeStack::const_iterator b = stack.begin(), i = b; i != stack.end();
       ++i) {
    (i == b ? out : out << ' ') << *i;
  }
  return out;
}

void Flips::Append(int i) {
  assert(0 <= i && i < num_pancakes_);
  flips_.push_back(num_pancakes_ - i);
}

PositionMap::PositionMap(PancakeStack stack)
    : num_pancakes_(stack.num_pancakes()) {
  stack.Sort();
  std::fill(positions_, positions_ + MAX_DIAMETER, -1);
  for (int i = 0; i < num_pancakes_; i++)
    positions_[stack.diameter(i) - 1] = i;
}

int PositionMap::pos(int diameter) const {
  assert(1 <= diameter && diameter <= MAX_DIAMETER);
  int i = positions_[diameter - 1];
  assert(0 <= i && i < num_pancakes_);
  return i;
}
