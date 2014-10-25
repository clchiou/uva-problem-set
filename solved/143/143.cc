#include <cmath>
#include <iomanip>
#include <iostream>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

#define LOG  \
  if (DEBUG) \
  std::cerr

struct Point {
  Point(double x_, double y_) : x(x_), y(y_) {}
  Point() : Point(0, 0) {}
  double x, y;

  static const Point ZERO;
};

const Point Point::ZERO;

std::istream& operator>>(std::istream& in, Point& point) {
  return in >> point.x >> point.y;
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
  return out << point.x << ' ' << point.y;
}

bool operator==(const Point& p, const Point& q) {
  return p.x == q.x && p.y == q.y;
}

bool operator!=(const Point& p, const Point& q) { return !(p == q); }

bool IsApprox(double x, double y) { return ::fabs(x - y) < 1e-8; }

Point operator-(const Point& p, const Point& q) {
  return Point(p.x - q.x, p.y - q.y);
}

double DotProduct(const Point& p, const Point& q) {
  return p.x * q.x + p.y * q.y;
}

double CrossProduct(const Point& p, const Point& q) {
  return p.x * q.y - p.y * q.x;
}

bool IsInsideOrOnBorder(const Point triangle[], const Point& point) {
  // Test if it is on the border.
  for (auto i = triangle, j = i + 1; j != triangle + 4; ++i, ++j) {
    Point segment = *j - *i;
    Point vector = point - *i;
    // Special case where triangle is not a proper triangle.
    if (segment == Point::ZERO) {
      if (vector == Point::ZERO) {
        return true;
      } else {
        continue;
      }
    }
    double product = CrossProduct(segment, vector);
    if (IsApprox(product, 0)) {
      double dot = DotProduct(segment, vector);
      if (0 <= dot && dot <= DotProduct(segment, segment)) {
        return true;
      }
    }
  }
  // Test if it is inside.
  enum { NONE, POS, NEG, } prev = NONE, curr = NONE;
  for (auto i = triangle, j = i + 1; j != triangle + 4; ++i, ++j) {
    Point segment = *j - *i;
    Point vector = point - *i;
    double product = CrossProduct(segment, vector);
    if (IsApprox(product, 0)) {
      return false;
    }
    curr = product > 0 ? POS : NEG;
    if (prev != NONE && prev != curr) {
      return false;
    }
    prev = curr;
  }
  return true;
}

int main() {
  for (Point triangle[4];
       std::cin >> triangle[0] >> triangle[1] >> triangle[2] &&
           (triangle[0] != Point::ZERO || triangle[1] != Point::ZERO ||
            triangle[2] != Point::ZERO);) {
    triangle[3] = triangle[0];
    LOG << "triangle: " << triangle[0] << ' ' << triangle[1] << ' '
        << triangle[2] << std::endl;
    int num_trees = 0;
    for (double x = 1; x <= 99; x += 1) {
      for (double y = 1; y <= 99; y += 1) {
        Point point(x, y);
        if (IsInsideOrOnBorder(triangle, point)) {
          LOG << "point: " << point << std::endl;
          ++num_trees;
        }
      }
    }
    std::cout << std::setw(4) << num_trees << std::endl;
  }
  return 0;
}
