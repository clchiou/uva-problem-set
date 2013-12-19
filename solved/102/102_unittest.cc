#define main _main
#include "102.cc"

#include "gtest/gtest.h"

#include "test_fixtures.h"

TEST(Test102, TestCountMoveOut) {
  Bin bin(3, 4, 5);
  ASSERT_EQ(bin.CountMoveOut(BROWN), 4 + 5);
  ASSERT_EQ(bin.CountMoveOut(GREEN), 3 + 5);
  ASSERT_EQ(bin.CountMoveOut(CLEAR), 3 + 4);
}

TEST(Test102, TestCountMove) {
  Bin bins[3] = {
    Bin(  3,   4,   5),
    Bin( 30,  40,  50),
    Bin(300, 400, 500),
  };
  Color order[3];

  order[0] = BROWN;
  order[1] = GREEN;
  order[2] = CLEAR;
  ASSERT_EQ(CountMove(bins, order), 4 + 5 + 30 + 50 + 300 + 400);

  order[0] = CLEAR;
  order[1] = GREEN;
  order[2] = BROWN;
  ASSERT_EQ(CountMove(bins, order), 3 + 4 + 30 + 50 + 400 + 500);

  order[0] = GREEN;
  order[1] = CLEAR;
  order[2] = BROWN;
  ASSERT_EQ(CountMove(bins, order), 3 + 5 + 30 + 40 + 400 + 500);
}

static void Compare(const Bin bins[3],
                    int min_move,
                    const std::string& min_order) {
  int min_move_out = 0;
  Color min_order_out[3];
  FindMinMove(bins, &min_move_out, min_order_out);
  ASSERT_EQ(min_move, min_move_out);
  ASSERT_EQ(min_order[0], ToChar(min_order_out[0]));
  ASSERT_EQ(min_order[1], ToChar(min_order_out[1]));
  ASSERT_EQ(min_order[2], ToChar(min_order_out[2]));
}

TEST(Test102, TestFindMinMove) {
  Bin bins[3];

  bins[0] = Bin(  3,   4,   5);
  bins[1] = Bin( 30,  40,  50);
  bins[2] = Bin(300, 400, 500);
  Compare(bins, 4 + 5 + 30 + 50 + 300 + 400, "BGC");

  bins[0] = Bin(  1, 100,   1);
  bins[1] = Bin(  1,   1, 100);
  bins[2] = Bin(100,   1,   1);
  Compare(bins, 6, "GCB");
}

TEST(Test102, TestFindMinMoveAlphabetically) {
  Bin bins[3];

  bins[0] = Bin(1, 1, 1);
  bins[1] = Bin(1, 1, 1);
  bins[2] = Bin(1, 1, 1);
  Compare(bins, 6, "BCG");
}

TEST_F(IntegrationTest, TestSampleData) { RunTestData(_main, "fin1", "fout1"); }
