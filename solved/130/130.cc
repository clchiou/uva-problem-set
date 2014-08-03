#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

typedef std::vector<int> Ring;

Ring::iterator Kill(Ring& ring, Ring::iterator i);
Ring::iterator Move(Ring& ring, Ring::iterator i, int step);
Ring::iterator Next(Ring& ring, Ring::iterator i);

const int DEAD = -1;

int main() {
  Ring ring;
  for (int num_peoples, step;
       std::cin >> num_peoples >> step && num_peoples != 0 && step != 0;) {
    ring.clear();
    for (int i = 0; i < num_peoples; i++) {
      ring.push_back(i + 1);
    }

    int num_alive = num_peoples;
    for (auto start = ring.begin(); num_alive > 1; num_alive--) {
      if (DEBUG) {
        for (auto i : ring) {
          std::cerr << ' ' << i;
        }
        std::cerr << std::endl;
        std::cerr << "start : " << *start << std::endl;
      }
      auto victim = Move(ring, start, step);
      if (DEBUG) {
        std::cerr << "victim: " << *victim << std::endl;
      }
      auto immediate_left = Kill(ring, victim);
      auto burier = Move(ring, immediate_left, step);
      if (DEBUG) {
        std::cerr << "burier: " << *burier << std::endl;
      }
      std::swap(*victim, *burier);
      start = Next(ring, victim);
    }

    int survivor =
        *std::find_if(
            ring.cbegin(), ring.cend(), [](int i) { return i != DEAD; });
    if (DEBUG) {
      std::cerr << "survivor: " << survivor << std::endl;
    }

    int pos = 1 + num_peoples - (survivor - 1);
    if (pos > num_peoples) {
      pos -= num_peoples;
    }
    std::cout << pos << std::endl;
  }
  return 0;
}

Ring::iterator Kill(Ring& ring, Ring::iterator i) {
  *i = DEAD;
  return Next(ring, i);
}

Ring::iterator Move(Ring& ring, Ring::iterator i, int step) {
  while (step-- > 1) {
    i = Next(ring, i);
  }
  return i;
}

Ring::iterator Next(Ring& ring, Ring::iterator i) {
  do {
    ++i;
    if (i == ring.end()) {
      i = ring.begin();
    }
  } while (*i == DEAD);
  return i;
}
