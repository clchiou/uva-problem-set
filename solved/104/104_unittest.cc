#define main _main
#include "104.cc"

#include "gtest/gtest.h"

#include "test_fixtures.h"

static void CopyTable(const double* ptr, ConversionTable* table) {
  for (int i = 0; i < ConversionTable::NumCurrencies; i++)
    for (int j = 0; j < ConversionTable::NumCurrencies; j++)
      table->at(i, j) = *ptr++;
}

static void CompareTable(const double* ptr, const ConversionTable& table) {
  for (int i = 0; i < ConversionTable::NumCurrencies; i++)
    for (int j = 0; j < ConversionTable::NumCurrencies; j++)
      ASSERT_DOUBLE_EQ(*ptr++, table.at(i, j));
}

TEST(Test104, Test1) {
  double conv_in[] = {
    1.000, 2.000,
    0.600, 1.000,
  };
  double conv_out[] = {
    1.200, 2.000,
    0.600, 1.200,
  };
  ConversionTable::NumCurrencies = 2;
  ConversionTable table;
  CopyTable(conv_in, &table);
  Arbitrage arb;
  Iterate(table, table, &arb, &arb.output_table);
  CompareTable(conv_out, arb.output_table);
}

TEST(Test104, Test2) {
  double conv_in[] = {
    1.000, 0.000, 2.000,
    0.000, 1.000, 0.000,
    2.000, 0.000, 1.000,
  };
  double conv_out[] = {
    4.000, 0.000, 2.000,
    0.000, 1.000, 0.000,
    2.000, 0.000, 4.000,
  };
  ConversionTable::NumCurrencies = 3;
  ConversionTable table;
  CopyTable(conv_in, &table);
  Arbitrage arb;
  Iterate(table, table, &arb, &arb.output_table);
  CompareTable(conv_out, arb.output_table);
}

TEST(Test104, Test3) {
  double conv_in[] = {
    1.000, 2.000, 4.000,
    0.500, 1.000, 8.000,
    0.250, 0.125, 1.000,
  };
  double conv_out[] = {
    1.000, 2.000, 16.000,
    2.000, 1.000, 8.000,
    0.250, 0.500, 1.000,
  };
  ConversionTable::NumCurrencies = 3;
  ConversionTable table;
  CopyTable(conv_in, &table);
  Arbitrage arb;
  Iterate(table, table, &arb, &arb.output_table);
  CompareTable(conv_out, arb.output_table);
}

TEST_F(IntegrationTest, TestSampleData) {
  RunTestData(_main, "fin1", "fout1");
  RunTestData(_main, "fin2", "fout2");
}
