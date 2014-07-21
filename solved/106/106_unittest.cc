#define main _main
#include "106.cc"

#include "gtest/gtest.h"

#include <algorithm>
#include <fstream>
#include <set>

struct Triple {
  int a, b, c;
};

bool operator<(const Triple& lhs, const Triple& rhs) {
  if (lhs.c != rhs.c) {
    return lhs.c < rhs.c;
  } else if (lhs.b != rhs.b) {
    return lhs.b < rhs.b;
  } else {
    return lhs.a < rhs.a;
  }
}

static void LoadDataset(const char* const path, std::set<Triple>* triples) {
  std::ifstream dataset(path);
  for (Triple triple; dataset >> triple.a >> triple.b >> triple.c;) {
    triples->insert(triple);
  }
  dataset.close();
}

TEST(Test106, TestPrimitivePythagoreanTriples) {
  // Primitive Pythagorean triples with 'c' < 6000.
  std::set<Triple> triples_6000;
  LoadDataset("ppt-6000.txt", &triples_6000);

  for (int upper_limit = 1; upper_limit < 6000; upper_limit++) {
    std::set<Triple> generated_triples;
    PrimitivePythagoreanTriples ppt(upper_limit);
    while (ppt.Next()) {
      Triple triple = {ppt.a(), ppt.b(), ppt.c(), };
      ASSERT_EQ(generated_triples.end(), generated_triples.find(triple));
      ASSERT_NE(triples_6000.end(), triples_6000.find(triple));
      generated_triples.insert(triple);
    }
    int n = std::count_if(triples_6000.cbegin(),
                          triples_6000.cend(),
                          [upper_limit](const Triple& triple) {
      return triple.c <= upper_limit;
    });
    ASSERT_EQ(n, generated_triples.size());
  }
}
