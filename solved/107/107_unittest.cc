#define main _main
#include "107.cc"

#include "gtest/gtest.h"

#include "test_fixtures.h"

#include <cmath>

TEST(Test107, TestPower) {
  for (int exponent = 0; exponent < 20; exponent++) {
    ASSERT_EQ((int)pow(2, exponent), Power(2, exponent));
  }
  for (int exponent = 0; exponent < 10; exponent++) {
    ASSERT_EQ((int)pow(3, exponent), Power(3, exponent));
  }
  for (int exponent = 0; exponent < 6; exponent++) {
    ASSERT_EQ((int)pow(10, exponent), Power(10, exponent));
  }
}

TEST_F(IntegrationTest, TestSampleData) {
  RunTestData(_main, "fin1", "fout1");
  RunTestData(_main, "fin2", "fout2");
}
