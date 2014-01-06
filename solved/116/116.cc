#include <algorithm>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

const int MAX_NUM_ROWS = 10;
const int MAX_NUM_COLS = 100;

class Matrix {
 public:
  friend std::istream& operator>>(std::istream& in, Matrix& matrix);
  friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

  Matrix() : num_rows_(0), num_cols_(0) {}
  Matrix(int num_rows, int num_cols);

  int num_rows() const { return num_rows_; }
  int num_cols() const { return num_cols_; }
  int canonical(int index) const { return (index + num_rows_) % num_rows_; }

  int at(int i, int j) const { return const_cast<Matrix*>(this)->at(i, j); }
  int& at(int i, int j);

 private:
  int num_rows_;
  int num_cols_;
  int matrix_[MAX_NUM_ROWS][MAX_NUM_COLS];
};

int Weight(const Matrix& weight,
           const Matrix& min_weight,
           int i0,
           int i1,
           int j0) {
  return weight.at(i0, j0) + min_weight.at(i1, j0 + 1);
}

bool IsSmallerLexicographically(const Matrix& matrix, int i0, int i1) {
  i0 = matrix.canonical(i0);
  i1 = matrix.canonical(i1);
  return i0 < i1;
}

void FindMinPath(const Matrix& weight,
                 int i,
                 int j,
                 Matrix* min_path,
                 Matrix* min_weight) {
  int i_min = i - 1;
  int w_min = Weight(weight, *min_weight, i, i_min, j);
  for (int i_delta = 0; i_delta <= 1; i_delta++) {
    int i1 = i + i_delta;
    int w = Weight(weight, *min_weight, i, i1, j);
    if (w < w_min ||
        (w == w_min && IsSmallerLexicographically(weight, i1, i_min))) {
      i_min = i1;
      w_min = w;
    }
  }
  min_path->at(i, j) = i_min;
  min_weight->at(i, j) = w_min;
}

int NodeId(const Matrix& matrix, int index) {
  return matrix.canonical(index) + 1;
}

int main() {
  for (Matrix weight; std::cin >> weight;) {
    Matrix min_path(weight.num_rows(), weight.num_cols());
    Matrix min_weight(weight.num_rows(), weight.num_cols());
    for (int i = 0; i < weight.num_rows(); i++) {
      int j = weight.num_cols() - 1;
      min_path.at(i, j) = i;
      min_weight.at(i, j) = weight.at(i, j);
    }

    for (int j = weight.num_cols() - 2; j >= 0; j--)
      for (int i = 0; i < weight.num_rows(); i++)
        FindMinPath(weight, i, j, &min_path, &min_weight);

    int i_min = 0;
    for (int i = 1; i < weight.num_rows(); i++)
      if (min_weight.at(i, 0) < min_weight.at(i_min, 0))
        i_min = i;
    for (int i = i_min, j = 0; j < weight.num_cols(); j++) {
      (j ? std::cout << ' ' : std::cout) << NodeId(weight, i);
      i = min_path.at(i, j);
    }
    std::cout << std::endl << min_weight.at(i_min, 0) << std::endl;
  }
  return 0;
}

std::istream& operator>>(std::istream& in, Matrix& matrix) {
  in >> matrix.num_rows_ >> matrix.num_cols_;
  for (int i = 0; i < matrix.num_rows_; i++) {
    for (int j = 0; j < matrix.num_cols_; j++)
      in >> matrix.matrix_[i][j];
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
  out << matrix.num_rows_ << ' ' << matrix.num_cols_ << std::endl;
  for (int i = 0; i < matrix.num_rows_; i++) {
    for (int j = 0; j < matrix.num_cols_; j++)
      (j ? out << ' ' : out) << matrix.matrix_[i][j];
    out << std::endl;
  }
  return out;
}

Matrix::Matrix(int num_rows, int num_cols)
    : num_rows_(num_rows), num_cols_(num_cols) {
  for (int i = 0; i < num_rows_; i++)
    std::fill(matrix_[i], matrix_[i] + num_cols_, 0);
}

int& Matrix::at(int i, int j) {
  assert(-1 <= i && i <= num_rows_ && 0 <= j && j < num_cols_);
  return matrix_[canonical(i)][j];
}
