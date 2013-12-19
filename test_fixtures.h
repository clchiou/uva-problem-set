#ifndef TEST_FIXTURES_H_
#define TEST_FIXTURES_H_

#include <fstream>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

class IntegrationTest : public ::testing::Test {
 protected:
  IntegrationTest()
      : cin_buf_(std::cin.rdbuf()), cout_buf_(std::cout.rdbuf()) {}

  virtual ~IntegrationTest() { RedirectIO(); }

  void RedirectIO(std::streambuf* in, std::streambuf* out) {
    std::cin.rdbuf(in);
    std::cout.rdbuf(out);
  }

  void RedirectIO() { RedirectIO(cin_buf_, cout_buf_); }

  void RunTestData(int (*main_function)(),
                   const std::string& input_path,
                   const std::string& output_path) {
    std::ifstream input;
    input.open(input_path.c_str());

    std::stringstream actual_output;

    RedirectIO(input.rdbuf(), actual_output.rdbuf());
    int ret = main_function();
    RedirectIO();

    input.close();

    std::ifstream expected_output;
    expected_output.open(output_path.c_str());

    ASSERT_EQ(0, ret);
    std::string expected_line, actual_line;
    while (std::getline(expected_output, expected_line)) {
      ASSERT_TRUE(std::getline(actual_output, actual_line));
      ASSERT_EQ(expected_line, actual_line);
    }
    ASSERT_FALSE(std::getline(actual_output, actual_line));

    expected_output.close();
  }

  std::streambuf* cin_buf_;
  std::streambuf* cout_buf_;
};

#endif  // TEST_FIXTURES_H_
