#define main _main
#include "103.cc"

#include "gtest/gtest.h"

#include "test_fixtures.h"

TEST_F(IntegrationTest, TestSampleData) {
  RunTestData(_main, "fin1", "fout1");
  RunTestData(_main, "fin2", "fout2");
}
