#define main _main
#include "112.cc"

#include <cstdlib>
#include <sstream>

#include "gtest/gtest.h"

#include "test_fixtures.h"

TEST(Test112, TestEmptyTree) {
  std::stringstream in("()");
  Tree* tree = Tree::Parse(in);

  srand(0);
  for (int times = 0; times < 100; times++) {
    ASSERT_FALSE(Tree::FindSumOfPaths(tree, rand()));
    ASSERT_FALSE(Tree::FindSumOfPaths(tree, -rand()));
  }

  delete tree;
}

TEST(Test112, TestOneNodeTree) {
  srand(0);

  for (int times = 0; times < 100; times++) {
    int sum = rand();

    std::stringstream in;
    in << "(" << sum << "()())";
    Tree* tree = Tree::Parse(in);

    ASSERT_TRUE(Tree::FindSumOfPaths(tree, sum));
    ASSERT_FALSE(Tree::FindSumOfPaths(tree, -sum));

    delete tree;
  }

  for (int times = 0; times < 100; times++) {
    int sum = rand();

    std::stringstream in;
    in << "(" << -sum << "()())";
    Tree* tree = Tree::Parse(in);

    ASSERT_FALSE(Tree::FindSumOfPaths(tree, sum));
    ASSERT_TRUE(Tree::FindSumOfPaths(tree, -sum));

    delete tree;
  }
}

TEST(Test112, TestGenericTree) {
  {
    std::stringstream in("(1\n(\n2\n(\n3(4(\n)(\n))(\n)\n)()\n)())\n");
    Tree* tree = Tree::Parse(in);
    for (int sum = -10; sum < 10; sum++)
      ASSERT_FALSE(Tree::FindSumOfPaths(tree, sum));
    ASSERT_TRUE(Tree::FindSumOfPaths(tree, 10));
    delete tree;
  }

  {
    std::stringstream in("(-1(\n\n\n-2(-3(\n\n-4()()\n)()\n)()\n)\n\n())\n");
    Tree* tree = Tree::Parse(in);
    for (int sum = -9; sum <= 10; sum++)
      ASSERT_FALSE(Tree::FindSumOfPaths(tree, sum));
    ASSERT_TRUE(Tree::FindSumOfPaths(tree, -10));
    delete tree;
  }
}

TEST_F(IntegrationTest, TestSampleData) {
  RunTestData(_main, "fin1", "fout1");
  RunTestData(_main, "fin2", "fout2");
  RunTestData(_main, "fin3", "fout3");
}
