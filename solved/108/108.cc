#include <algorithm>
#include <climits>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

enum {
  N_MAX = 100,
  V_MAX = 127,
  V_MIN = -127,
};

// Reserve 1 row & column as sentinels.
typedef int Matrix[N_MAX + 1][N_MAX + 1];

struct Rectangle {
  int x1, y1, x2, y2;
};

void ReadMatrix(std::istream& in, int* n_out, Matrix matrix) {
  int n;
  in >> n;

  for (int j = 0; j <= n; j++) {
    matrix[0][j] = 0;
  }
  for (int i = 1; i <= n; i++) {
    matrix[i][0] = 0;
    for (int j = 1; j <= n; j++) {
      in >> matrix[i][j];
    }
  }

  *n_out = n;
}

void ComputeIntegralImage(int n, Matrix matrix, Matrix integral_image) {
  for (int j = 0; j <= n; j++) {
    integral_image[0][j] = 0;
  }
  for (int i = 1; i <= n; i++) {
    integral_image[i][0] = 0;
    int sum = 0;
    for (int j = 1; j <= n; j++) {
      sum += matrix[i][j];
      integral_image[i][j] = integral_image[i - 1][j] + sum;
    }
  }
}

int ComputeRectangleSum(int n, Matrix integral_image, const Rectangle &rect) {
  return
      integral_image[rect.x2    ][rect.y2    ]
    - integral_image[rect.x1 - 1][rect.y2    ]
    - integral_image[rect.x2    ][rect.y1 - 1]
    + integral_image[rect.x1 - 1][rect.y1 - 1];
}

void FindMaximalSubRectangle(int n, Matrix matrix,
    int* sum_out, Rectangle* rect_out) {
  Matrix integral_image;
  ComputeIntegralImage(n, matrix, integral_image);

  int max_sum = INT_MIN;
  Rectangle max_rect;

  for (int x1 = 1; x1 <= n; x1++) {
    for (int y1 = 1; y1 <= n; y1++) {
      for (int x2 = x1; x2 <= n; x2++) {
        for (int y2 = y1; y2 <= n; y2++) {
          Rectangle rect = {
            .x1 = x1,
            .y1 = y1,
            .x2 = x2,
            .y2 = y2,
          };
          int sum = ComputeRectangleSum(n, integral_image, rect);
          if (sum > max_sum) {
            max_sum = sum;
            max_rect = rect;
          }
        }
      }
    }
  }

  *sum_out = max_sum;
  *rect_out = max_rect;
}

int main() {
  int n;
  Matrix matrix;
  ReadMatrix(std::cin, &n, matrix);

  int sum = 0;
  Rectangle rect;
  FindMaximalSubRectangle(n, matrix, &sum, &rect);

  std::cout << sum << std::endl;

  return 0;
}
