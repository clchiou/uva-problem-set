#define main _main
#include "101.cc"

#include <sstream>

#include "gtest/gtest.h"

#include "test_fixtures.h"

static void Compare(const BlockWorld &world, const std::string &expected) {
  std::ostringstream out;
  world.Output(out);
  ASSERT_EQ(expected, out.str());
}

TEST(Test101, TestMoveTopBlock) {
  BlockWorld world(9);

  world.MoveTopBlock(0, 8);
  Compare(world,
      "0:\n"
      "1: 1\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5: 5\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8 0\n");

  world.MoveTopBlock(1, 8);
  Compare(world,
      "0:\n"
      "1:\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5: 5\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8 0 1\n");

  world.MoveTopBlock(5, 8);
  Compare(world,
      "0:\n"
      "1:\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5:\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8 0 1 5\n");
}

TEST(Test101, TestMoveBlocks) {
  BlockWorld world(9);

  world.MoveBlocks(0, 8, 0);
  Compare(world,
      "0:\n"
      "1: 1\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5: 5\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8 0\n");

  world.MoveBlocks(8, 1, 8);
  Compare(world,
      "0:\n"
      "1: 1 8 0\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5: 5\n"
      "6: 6\n"
      "7: 7\n"
      "8:\n");

  world.MoveBlocks(1, 4, 1);
  Compare(world,
      "0:\n"
      "1:\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4 1 8 0\n"
      "5: 5\n"
      "6: 6\n"
      "7: 7\n"
      "8:\n");

  world.MoveBlocks(4, 3, 1);
  Compare(world,
      "0:\n"
      "1:\n"
      "2: 2\n"
      "3: 3 1 8 0\n"
      "4: 4\n"
      "5: 5\n"
      "6: 6\n"
      "7: 7\n"
      "8:\n");
}

TEST(Test101, TestReturnBlocksAbove) {
  BlockWorld world(9);

  world.MoveTopBlock(1, 0);
  world.MoveTopBlock(2, 0);
  world.MoveTopBlock(3, 0);
  world.MoveTopBlock(4, 0);
  world.MoveTopBlock(5, 0);
  world.MoveTopBlock(6, 0);
  world.MoveTopBlock(7, 0);
  world.MoveTopBlock(8, 0);
  Compare(world,
      "0: 0 1 2 3 4 5 6 7 8\n"
      "1:\n"
      "2:\n"
      "3:\n"
      "4:\n"
      "5:\n"
      "6:\n"
      "7:\n"
      "8:\n");

  world.ReturnBlocksAbove(0, 0);
  Compare(world,
      "0: 0\n"
      "1: 1\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5: 5\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8\n");

  world.MoveTopBlock(8, 0);
  world.MoveTopBlock(7, 8);
  world.ReturnBlocksAbove(0, 0);
  Compare(world,
      "0: 0\n"
      "1: 1\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5: 5\n"
      "6: 6\n"
      "7:\n"
      "8: 7 8\n");
}

TEST(Test101, TestIllegalCommand) {
  BlockWorld world(9);

  world.MoveTopBlock(0, 8);
  world.MoveTopBlock(1, 8);
  world.MoveTopBlock(5, 8);
  Compare(world,
      "0:\n"
      "1:\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5:\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8 0 1 5\n");

  // Illegal: Same block.
  world.Execute("move 3 over 3\n");
  Compare(world,
      "0:\n"
      "1:\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5:\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8 0 1 5\n");

  // Illegal: Same stack.
  world.Execute("move 1 onto 5\n");
  Compare(world,
      "0:\n"
      "1:\n"
      "2: 2\n"
      "3: 3\n"
      "4: 4\n"
      "5:\n"
      "6: 6\n"
      "7: 7\n"
      "8: 8 0 1 5\n");
}

TEST_F(IntegrationTest, TestSampleData) {
  RunTestData(_main, "fin1", "fout1");
}
