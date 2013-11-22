#define main _main
#include "100.cc"

#include "gtest/gtest.h"

#include "test_fixtures.h"

TEST(Test100, TestComputeCycle) {
  ASSERT_EQ(1, ComputeCycle(1));
  ASSERT_EQ(2, ComputeCycle(2));
  ASSERT_EQ(3, ComputeCycle(4));
  ASSERT_EQ(4, ComputeCycle(8));
}

TEST(Test100, TestComputeMaxCycle) {
  ASSERT_EQ(ComputeCycle(1), ComputeMaxCycle(1, 1));
  ASSERT_EQ(ComputeCycle(2), ComputeMaxCycle(2, 2));
  ASSERT_EQ(ComputeCycle(3), ComputeMaxCycle(3, 3));

  ASSERT_EQ(ComputeMaxCycle(1, 3), ComputeMaxCycle(3, 1));
  ASSERT_EQ(ComputeMaxCycle(2, 4), ComputeMaxCycle(4, 2));
}

TEST_F(IntegrationTest, TestSampleData) {
  RunTestData(_main, "fin1", "fout1");
  RunTestData(_main, "fin2", "fout2");
}
