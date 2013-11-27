#define main _main
#include "108.cc"

#include <cstdlib>
#include <sstream>

#include "gtest/gtest.h"

#include "test_fixtures.h"

static void Input(const std::string &input, int* n_out, Matrix m) {
  std::stringstream in(input);
  ReadMatrix(in, n_out, m);
}

static void Output(int n, Matrix m, std::ostream &out) {
  for (int i = 0; i <= n; i++) {
    out << m[i][0];
    for (int j = 1; j <= n; j++) {
      out << ' ' << m[i][j];
    }
    out << std::endl;
  }
}

static void Compare(int n, Matrix actual, const std::string &expected) {
  std::stringstream act;
  Output(n, actual, act);
  ASSERT_EQ(expected, act.str());
}

TEST(Test108, TestComputeIntegralImage) {
  int n;
  Matrix matrix, integral_image;

  Input("3\n"
      "1 2 3\n"
      "4 5 6\n"
      "7 8 9\n",
      &n, matrix);
  ComputeIntegralImage(n, matrix, integral_image);
  Compare(n, integral_image,
      "0 0 0 0\n"
      "0 1 3 6\n"
      "0 5 12 21\n"
      "0 12 27 45\n");

  n = N_MAX;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      matrix[i][j] = V_MAX;
    }
  }
  ComputeIntegralImage(n, matrix, integral_image);
  ASSERT_EQ(V_MAX * N_MAX * N_MAX, integral_image[N_MAX][N_MAX]);

  n = N_MAX;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      matrix[i][j] = V_MIN;
    }
  }
  ComputeIntegralImage(n, matrix, integral_image);
  ASSERT_EQ(V_MIN * N_MAX * N_MAX, integral_image[N_MAX][N_MAX]);
}

TEST(Test108, TestComputeRectangleSum) {
  int n;
  Matrix matrix, integral_image;

  srand(1);

  n = N_MAX;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      matrix[i][j] = rand() % V_MAX;
    }
  }
  ComputeIntegralImage(n, matrix, integral_image);
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      Rectangle rect = {
        .x1 = i,
        .y1 = j,
        .x2 = i,
        .y2 = j,
      };
      ASSERT_EQ(matrix[i][j], ComputeRectangleSum(n, integral_image, rect));
    }
  }
}

TEST(Test108, TestFindMaximalSubRectangle) {
  int n, sum;
  Matrix matrix;
  Rectangle rect;

  n = N_MAX;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      matrix[i][j] = V_MAX;
    }
  }
  FindMaximalSubRectangle(n, matrix, &sum, &rect);
  ASSERT_EQ(V_MAX * N_MAX * N_MAX, sum);
  ASSERT_EQ(1, rect.x1);
  ASSERT_EQ(1, rect.y1);
  ASSERT_EQ(n, rect.x2);
  ASSERT_EQ(n, rect.y2);

  n = N_MAX;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      matrix[i][j] = V_MIN;
    }
  }
  FindMaximalSubRectangle(n, matrix, &sum, &rect);
  ASSERT_EQ(V_MIN, sum);
  ASSERT_TRUE(rect.x1 == rect.x2);
  ASSERT_TRUE(rect.y1 == rect.y2);
}

TEST_F(IntegrationTest, TestSampleData) {
  RunTestData(_main, "fin1", "fout1");
  RunTestData(_main, "fin2", "fout2");
}
