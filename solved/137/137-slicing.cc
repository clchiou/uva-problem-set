#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

// XXX: This "slicing" algorithm does not solve the problem 137.  It needs
// arbitrary precision of rational numbers to guarantee correctness, and it is
// also computationally inefficient.

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

// Coordinate range 1 <= x, y <= 99.  For the convenience of testing, also
// include 0.
enum { X_MIN = -1, X_MAX = 100, Y_MIN = -1, Y_MAX = 100, };

// Rational number.

class Rational {
 public:
  Rational(int numerator, int denominator);
  explicit Rational(int numerator) : Rational(numerator, 1) {}
  Rational() : Rational(0) {}

  int numerator() const { return numerator_; }
  int denominator() const { return denominator_; }

  explicit operator double() const {
    return static_cast<double>(numerator_) / denominator_;
  }

  explicit operator int() const {
    assert(numerator_ % denominator_ == 0);
    return numerator_ / denominator_;
  }

  static const Rational zero, one, two;

 private:
  int numerator_, denominator_;
};

const Rational Rational::zero(0);
const Rational Rational::one(1);
const Rational Rational::two(2);

int ComputeGcd(int a, int b) {
  assert(a >= 0 && b >= 0);
  while (b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

Rational::Rational(int numerator, int denominator)
    : numerator_(numerator), denominator_(denominator) {
  assert(denominator_ != 0);
  if (numerator_ < 0 && denominator_ < 0) {
    numerator_ = abs(numerator_);
    denominator_ = abs(denominator_);
  } else if (denominator_ < 0) {
    numerator_ = -numerator_;
    denominator_ = abs(denominator_);
  }
  int gcd = ComputeGcd(abs(numerator_), denominator_);
  numerator_ /= gcd;
  denominator_ /= gcd;
}

bool operator<(const Rational& p, const Rational& q) {
  return p.numerator() * q.denominator() < q.numerator() * p.denominator();
}

bool operator==(const Rational& p, const Rational& q) {
  return p.numerator() * q.denominator() == q.numerator() * p.denominator();
}

bool operator<=(const Rational& p, const Rational& q) {
  return p < q || p == q;
}

bool operator>(const Rational& p, const Rational& q) { return q < p; }
bool operator>=(const Rational& p, const Rational& q) { return q <= p; }
bool operator!=(const Rational& p, const Rational& q) { return !(p == q); }

Rational operator+(const Rational& p, const Rational& q) {
  int numerator =
      p.numerator() * q.denominator() + q.numerator() * p.denominator();
  int denominator = p.denominator() * q.denominator();
  return Rational(numerator, denominator);
}

Rational operator-(const Rational& p) {
  return Rational(-p.numerator(), p.denominator());
}

Rational operator-(const Rational& p, const Rational& q) { return p + (-q); }

Rational operator*(const Rational& p, const Rational& q) {
  int numerator = p.numerator() * q.numerator();
  int denominator = p.denominator() * q.denominator();
  return Rational(numerator, denominator);
}

Rational operator/(const Rational& p, const Rational& q) {
  assert(q.numerator() != 0);
  int numerator = p.numerator() * q.denominator();
  int denominator = p.denominator() * q.numerator();
  return Rational(numerator, denominator);
}

std::ostream& operator<<(std::ostream& out, const Rational& r) {
  if (r.denominator() == 1) {
    return out << r.numerator();
  } else {
    return out << r.numerator() << '/' << r.denominator();
  }
}

// Polygon.

class Point {
 public:
  Point(Rational x, Rational y) : x_(std::move(x)), y_(std::move(y)) {}
  const Rational& x() const { return x_; }
  const Rational& y() const { return y_; }

 private:
  Rational x_, y_;
};

typedef std::vector<Point> Points;

bool ReadPoints(std::istream& in, Points* points) {
  int num_points;
  if (!(in >> num_points && num_points != 0)) {
    return false;
  }
  points->clear();
  for (int i = 0, x, y; i < num_points && in >> x && in >> y; i++) {
    points->push_back(Point(Rational(x), Rational(y)));
  }
  return true;
}

std::ostream& operator<<(std::ostream& out, const Points& points) {
  for (const auto& point : points) {
    out << ' ' << point.x() << ',' << point.y();
  }
  return out;
}

// Main function.

struct Range {
  Rational lower, upper;
};

void ComputeRanges(const Points& points, Range y_ranges[X_MAX - X_MIN]) {
  Rational y_min(Y_MIN), y_max(Y_MAX);
  for (int i = 0; i < X_MAX - X_MIN; i++) {
    y_ranges[i].lower = y_max;
    y_ranges[i].upper = y_min;
  }
  for (auto i = points.cbegin(), j = i + 1; j != points.cend(); i++, j++) {
    const Point* p1 = &(*i);
    const Point* p2 = &(*j);
    if (p1->x() > p2->x()) {
      std::swap(p1, p2);
    }
    Rational x_range = p2->x() - p1->x();
    int index = static_cast<int>(p1->x()) - X_MIN - 1;
    if (x_range == Rational::zero) {
      y_ranges[index].lower =
          std::min(y_ranges[index].lower, std::min(p1->y(), p2->y()));
      y_ranges[index].upper =
          std::max(y_ranges[index].upper, std::max(p1->y(), p2->y()));
      continue;
    }
    for (Rational x_offset(0); x_offset <= x_range;
         x_offset = x_offset + Rational::one) {
      Rational ratio = x_offset / x_range;
      Rational y = (Rational::one - ratio) * p1->y() + ratio * p2->y();
      y_ranges[index].lower = std::min(y_ranges[index].lower, y);
      y_ranges[index].upper = std::max(y_ranges[index].upper, y);
      index++;
    }
  }
}

bool IsValidRange(const Range& range,
                  const Rational& min,
                  const Rational& max) {
  return range.lower != max && range.upper != min;
}

Rational ComputeArea(const Range& range_left, const Range& range_right) {
  return ((range_left.upper - range_left.lower) +
          (range_right.upper - range_right.lower)) /
         Rational::two;
}

bool Intersect(const Range& r1, const Range& r2, Range* intersection) {
  if (r1.lower > r2.upper || r2.lower > r1.upper) {
    return false;
  }
  intersection->upper = std::min(r1.upper, r2.upper);
  intersection->lower = std::max(r1.lower, r2.lower);
  return true;
}

int main() {
  // Exploit the fact that coordinates are integers.
  Rational y_min(Y_MIN), y_max(Y_MAX);
  Range y_ranges1[X_MAX - X_MIN], y_ranges2[X_MAX - X_MIN];
  Points poly1, poly2;
  while (ReadPoints(std::cin, &poly1) && ReadPoints(std::cin, &poly2)) {
    LOG << "poly1=" << poly1 << std::endl;
    LOG << "poly2=" << poly2 << std::endl;
    if (poly1.size() == 1 || poly2.size() == 1) {
      std::cout << "    0.00";
      continue;
    }

    // Add sentinels.
    poly1.push_back(poly1.front());
    poly2.push_back(poly2.front());

    ComputeRanges(poly1, y_ranges1);
    ComputeRanges(poly2, y_ranges2);
    if (DEBUG) {
      for (const Range* y_ranges : {y_ranges1, y_ranges2}) {
        LOG << "y_ranges=";
        for (int i = 0; i < X_MAX - X_MIN; i++) {
          if (IsValidRange(y_ranges[i], y_min, y_max)) {
            LOG << ' ' << i + X_MIN + 1 << ':';
            LOG << y_ranges[i].lower << '~' << y_ranges[i].upper;
          }
        }
        LOG << std::endl;
      }
    }

    Rational area;
    for (int i = 0, j = 1; i < X_MAX - X_MIN - 1; i++, j++) {
      bool valid1 = IsValidRange(y_ranges1[i], y_min, y_max) &&
                    IsValidRange(y_ranges1[j], y_min, y_max);
      bool valid2 = IsValidRange(y_ranges2[i], y_min, y_max) &&
                    IsValidRange(y_ranges2[j], y_min, y_max);
      if (valid1) {
        Rational delta = ComputeArea(y_ranges1[i], y_ranges1[j]);
        LOG << "delta1=" << delta << std::endl;
        area = area + delta;
      }
      if (valid2) {
        Rational delta = ComputeArea(y_ranges2[i], y_ranges2[j]);
        LOG << "delta2=" << delta << std::endl;
        area = area + delta;
      }
      if (valid1 && valid2) {
        Range left, right;
        if (Intersect(y_ranges1[i], y_ranges2[i], &left) &&
            Intersect(y_ranges1[j], y_ranges2[j], &right)) {
          Rational intersection = ComputeArea(left, right);
          LOG << "intersection=" << left.lower << '~' << left.upper << ' '
              << right.lower << '~' << right.upper << " -> " << intersection
              << std::endl;
          area = area - intersection - intersection;
        }
      }
      assert(area >= Rational::zero);
    }

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%8.2f", static_cast<double>(area));
    std::cout << buffer;
  }
  std::cout << std::endl;
  return 0;
}
