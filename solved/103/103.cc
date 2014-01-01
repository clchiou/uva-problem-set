#include <algorithm>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

const int MAX_NUM_BOXES = 30;
const int MAX_DIMENSION = 10;

class Box {
 public:
  static int Dimension;

  friend std::istream& operator>>(std::istream& in, Box& box);
  friend std::ostream& operator<<(std::ostream& out, const Box& box);

  Box();

  void Sort();
  bool IsNestIn(const Box& other) const;

 private:
  int measurements_[MAX_DIMENSION];
};

template <typename E>
class Matrix {
 public:
  Matrix(const E& init_value) {
    for (int i = 0; i < MAX_NUM_BOXES; i++)
      std::fill(elements_[i], elements_[i] + MAX_NUM_BOXES, init_value);
  }

  const E& at(int i, int j) const {
    return const_cast<Matrix*>(this)->at(i, j);
  }

  E& at(int i, int j) {
    assert(0 <= i && i < MAX_NUM_BOXES && 0 <= j && j < MAX_NUM_BOXES);
    return elements_[i][j];
  }

 private:
  E elements_[MAX_NUM_BOXES][MAX_NUM_BOXES];
};

int box_id(int index) { return index + 1; }

void PrintPath(const Matrix<int>& intermediates, int from, int to);

//// main

int main() {
  Box boxes[MAX_NUM_BOXES];
  int num_boxes;

  while (std::cin >> num_boxes >> Box::Dimension) {
    assert(0 < num_boxes && num_boxes <= MAX_NUM_BOXES);
    for (int i = 0; i < num_boxes; i++) {
      std::cin >> boxes[i];
      boxes[i].Sort();
    }

    Matrix<int> distances(0);
    Matrix<int> intermediates(-1);
    for (int i = 0; i < num_boxes; i++)
      for (int j = 0; j < num_boxes; j++)
        if (boxes[i].IsNestIn(boxes[j])) {
          distances.at(i, j) = 1;
          intermediates.at(i, j) = j;
        }

    // Because the graph constructed from the box-nesting relation is an
    // acyclic directed graph, we may use shortest path algorithm for it.
    for (int k = 0; k < num_boxes; k++) {
      for (int i = 0; i < num_boxes; i++) {
        for (int j = 0; j < num_boxes; j++) {
          if (distances.at(i, k) == 0 || distances.at(k, j) == 0)
            continue;
          int new_distance = distances.at(i, k) + distances.at(k, j);
          if (new_distance > distances.at(i, j)) {
            distances.at(i, j) = new_distance;
            intermediates.at(i, j) = k;
          }
        }
      }
    }

    int longest_distance = -1, from = -1, to = -1;
    for (int i = 0; i < num_boxes; i++) {
      for (int j = 0; j < num_boxes; j++) {
        int distance = distances.at(i, j);
        if (distance > longest_distance) {
          longest_distance = distance;
          from = i;
          to = j;
        }
      }
    }
    if (longest_distance > 0) {
      assert(from != -1 && to != -1);
      std::cout << longest_distance + 1 << std::endl;
      std::cout << box_id(from);
      PrintPath(intermediates, from, to);
      std::cout << ' ' << box_id(to) << std::endl;
    } else {
      std::cout << 1 << std::endl;
      std::cout << 1 << std::endl;
    }
  }

  return 0;
}

void PrintPath(const Matrix<int>& intermediates, int from, int to) {
  int node = intermediates.at(from, to);
  if (node != -1 && node != to) {
    PrintPath(intermediates, from, node);
    std::cout << ' ' << box_id(node);
    PrintPath(intermediates, node, to);
  }
}

//// class Box

int Box::Dimension;

std::istream& operator>>(std::istream& in, Box& box) {
  assert(0 < Box::Dimension && Box::Dimension <= MAX_DIMENSION);
  for (int dim = 0; dim < Box::Dimension; dim++)
    in >> box.measurements_[dim];
  return in;
}

std::ostream& operator<<(std::ostream& out, const Box& box) {
  assert(0 < Box::Dimension && Box::Dimension <= MAX_DIMENSION);
  for (int dim = 0; dim < Box::Dimension; dim++) {
    if (dim > 0)
      out << ' ';
    out << box.measurements_[dim];
  }
  return out;
}

Box::Box() { std::fill(measurements_, measurements_ + MAX_DIMENSION, 0); }

void Box::Sort() {
  assert(0 < Box::Dimension && Box::Dimension <= MAX_DIMENSION);
  std::sort(measurements_, measurements_ + Box::Dimension);
}

bool Box::IsNestIn(const Box& other) const {
  assert(0 < Box::Dimension && Box::Dimension <= MAX_DIMENSION);
  for (int dim = 0; dim < Box::Dimension; dim++) {
    if (measurements_[dim] >= other.measurements_[dim])
      return false;
  }
  return true;
}
