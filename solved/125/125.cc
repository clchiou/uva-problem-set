#include <algorithm>
#include <iostream>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

enum { MAX_NUM_INTERSECTIONS = 30, };

class Matrix {
 public:
  Matrix();
  explicit Matrix(const Matrix& other);

  void Reset();

  int at(int i, int j) const;
  int& at(int i, int j);

 private:
  int matrix_[(MAX_NUM_INTERSECTIONS + 1)][(MAX_NUM_INTERSECTIONS + 1)];
};

void PrintMatrix(const Matrix& matrix, int size, std::ostream& out);

int main() {
  for (int city = 0, num_streets; std::cin >> num_streets; city++) {
    if (num_streets == 0) {
      std::cout << "matrix for city " << city << std::endl;
      continue;
    }

    Matrix matrix;
    int num_intersections = 0;
    while (num_streets-- > 0) {
      int i, j;
      std::cin >> i >> j;
      matrix.at(i, j)++;
      num_intersections = std::max(num_intersections, i + 1);
      num_intersections = std::max(num_intersections, j + 1);
    }

    Matrix m0_(matrix), m1_, m_paths(matrix);
    Matrix* m0 = &m0_;
    Matrix* m1 = &m1_;
    for (int n = 0; n < num_intersections; n++) {
      for (int k = 0; k < num_intersections; k++) {
        for (int i = 0; i < num_intersections; i++) {
          for (int j = 0; j < num_intersections; j++) {
            m1->at(i, j) += m0->at(i, k) * matrix.at(k, j);
          }
        }
      }
      if (DEBUG) {
        std::cerr << "m1" << std::endl;
        PrintMatrix(*m1, num_intersections, std::cerr);
      }
      for (int i = 0; i < num_intersections; i++) {
        for (int j = 0; j < num_intersections; j++) {
          m_paths.at(i, j) += m1->at(i, j);
        }
      }
      std::swap(m0, m1);
      m1->Reset();
    }

    for (int k = 0; k < num_intersections; k++) {
      if (!m_paths.at(k, k)) {
        continue;
      }
      for (int i = 0; i < num_intersections; i++) {
        for (int j = 0; j < num_intersections; j++) {
          if (m_paths.at(i, k) && m_paths.at(k, j)) {
            m_paths.at(i, j) = -1;
          }
        }
      }
    }

    std::cout << "matrix for city " << city << std::endl;
    for (int i = 0; i < num_intersections; i++) {
      for (int j = 0; j < num_intersections; j++) {
        if (j > 0) {
          std::cout << ' ';
        }
        std::cout << m_paths.at(i, j);
      }
      std::cout << std::endl;
    }
  }
  return 0;
}

void PrintMatrix(const Matrix& matrix, int size, std::ostream& out) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out << ' ' << matrix.at(i, j);
    }
    out << std::endl;
  }
}

Matrix::Matrix() { Reset(); }

Matrix::Matrix(const Matrix& other) {
  for (int i = 0; i < MAX_NUM_INTERSECTIONS + 1; i++) {
    for (int j = 0; j < MAX_NUM_INTERSECTIONS + 1; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

void Matrix::Reset() {
  for (int i = 0; i < MAX_NUM_INTERSECTIONS + 1; i++) {
    for (int j = 0; j < MAX_NUM_INTERSECTIONS + 1; j++) {
      matrix_[i][j] = 0;
    }
  }
}

int Matrix::at(int i, int j) const { return matrix_[i][j]; }
int& Matrix::at(int i, int j) { return matrix_[i][j]; }
