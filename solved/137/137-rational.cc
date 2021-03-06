#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

// XXX: Although this solution gets accepted by the online judge, it does not
// pass my test cases (it should be due to integer overflow).

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

// Rational number.

class Rational {
 public:
  Rational(long long numerator, long long denominator);
  explicit Rational(long long numerator) : Rational(numerator, 1) {}
  Rational() : Rational(0) {}

  long long numerator() const { return numerator_; }
  long long denominator() const { return denominator_; }

  explicit operator double() const {
    return static_cast<double>(numerator_) / denominator_;
  }

  explicit operator int() const {
    assert(numerator_ % denominator_ == 0);
    return numerator_ / denominator_;
  }

  static const Rational zero, one, two;

 private:
  long long numerator_, denominator_;
};

const Rational Rational::zero(0);
const Rational Rational::one(1);
const Rational Rational::two(2);

long long ComputeGcd(long long a, long long b) {
  assert(a >= 0 && b >= 0);
  while (b != 0) {
    long long t = b;
    b = a % b;
    a = t;
  }
  return a;
}

Rational::Rational(long long numerator, long long denominator)
    : numerator_(numerator), denominator_(denominator) {
  assert(denominator_ != 0);
  if (numerator_ < 0 && denominator_ < 0) {
    numerator_ = ::llabs(numerator_);
    denominator_ = ::llabs(denominator_);
  } else if (denominator_ < 0) {
    numerator_ = -numerator_;
    denominator_ = ::llabs(denominator_);
  }
  long long gcd = ComputeGcd(::llabs(numerator_), denominator_);
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
  long long numerator =
      p.numerator() * q.denominator() + q.numerator() * p.denominator();
  long long denominator = p.denominator() * q.denominator();
  return Rational(numerator, denominator);
}

Rational operator-(const Rational& p) {
  return Rational(-p.numerator(), p.denominator());
}

Rational operator-(const Rational& p, const Rational& q) { return p + (-q); }

Rational operator*(const Rational& p, const Rational& q) {
  long long numerator = p.numerator() * q.numerator();
  long long denominator = p.denominator() * q.denominator();
  return Rational(numerator, denominator);
}

Rational operator/(const Rational& p, const Rational& q) {
  assert(q.numerator() != 0);
  long long numerator = p.numerator() * q.denominator();
  long long denominator = p.denominator() * q.numerator();
  return Rational(numerator, denominator);
}

std::ostream& operator<<(std::ostream& out, const Rational& r) {
  if (r.denominator() == 1) {
    return out << r.numerator();
  }
  char buffer[32];
  ::snprintf(buffer, sizeof(buffer), "%.2f", static_cast<double>(r));
  return out << buffer;
}

// Linear algebra.

class Vector {
 public:
  Vector() {}
  Vector(Rational x, Rational y) : x_(std::move(x)), y_(std::move(y)) {}
  const Rational& x() const { return x_; }
  const Rational& y() const { return y_; }

 private:
  Rational x_, y_;
};

Vector operator-(const Vector& p, const Vector& q) {
  return Vector(p.x() - q.x(), p.y() - q.y());
}

Rational DotProduct(const Vector& p, const Vector& q) {
  return p.x() * q.x() + p.y() * q.y();
}

Rational CrossProduct(const Vector& p, const Vector& q) {
  return p.x() * q.y() - p.y() * q.x();
}

std::ostream& operator<<(std::ostream& out, const Vector& v) {
  return out << v.x() << ',' << v.y();
}

typedef std::vector<Vector> Vectors;

bool ReadVectors(std::istream& in, Vectors* points) {
  int num_points;
  if (!(in >> num_points && num_points != 0)) {
    return false;
  }
  points->clear();
  for (int i = 0, x, y; i < num_points && in >> x && in >> y; i++) {
    points->push_back(Vector(Rational(x), Rational(y)));
  }
  return true;
}

std::ostream& operator<<(std::ostream& out, const Vectors& points) {
  for (const auto& point : points) {
    out << ' ' << point;
  }
  return out;
}

// | a b |
// | c d |
struct Matrix {
  Matrix(Rational a_, Rational b_, Rational c_, Rational d_)
      : a(std::move(a_)),
        b(std::move(b_)),
        c(std::move(c_)),
        d(std::move(d_)) {}
  Rational a, b, c, d;
};

Rational ComputeDeterminant(const Matrix& m) { return m.a * m.d - m.b * m.c; }

bool ComputeInverseMatrix(const Matrix& m, Matrix* out) {
  Rational determinant = ComputeDeterminant(m);
  if (determinant == Rational::zero) {
    return false;
  }
  Matrix inverse(m.d / determinant,
                 -m.b / determinant,
                 -m.c / determinant,
                 m.a / determinant);
  *out = inverse;
  return true;
}

Vector operator*(const Matrix& m, const Vector& v) {
  return Vector(m.a * v.x() + m.b * v.y(), m.c * v.x() + m.d * v.y());
}

// Construct a line passing through p and q:
//   line[0] * x + line[1] * y = line[2].
void ComputeLine(const Vector& p, const Vector& q, Rational line[3]) {
  Rational delta_x = q.x() - p.x();
  Rational delta_y = q.y() - p.y();
  line[0] = delta_y;
  line[1] = -delta_x;
  line[2] = p.x() * delta_y - p.y() * delta_x;
}

// Main function.

bool IsInside(const Vectors& poly, const Vector& point) {
  enum { NONE, POS, NEG, } prev = NONE, curr = NONE;
  for (auto i = poly.cbegin(), j = i + 1; j != poly.cend(); ++i, ++j) {
    Rational product = CrossProduct(*j - *i, point - *i);
    if (product == Rational::zero) {
      return false;
    }
    curr = product > Rational::zero ? POS : NEG;
    if (prev != NONE && prev != curr) {
      return false;
    }
    prev = curr;
  }
  return true;
}

bool IsInSegment(const Vector& seg1, const Vector& seg2, const Vector& point) {
  Vector vec = point - seg1;
  Vector seg = seg2 - seg1;
  if (CrossProduct(seg, vec) != Rational::zero) {
    return false;
  }
  Rational dot = DotProduct(seg, vec);
  return Rational::zero <= dot && dot <= DotProduct(seg, seg);
}

bool IsOnBorder(const Vectors& poly, const Vector& point) {
  for (auto i = poly.cbegin(), j = i + 1; j != poly.cend(); ++i, ++j) {
    if (IsInSegment(*i, *j, point)) {
      return true;
    }
  }
  return false;
}

// Use double (not Rational) to prevent overflow.
double ComputeArea(const Vectors& poly) {
  double area = 0.0;
  for (auto i = poly.cbegin(), j = i + 1; j != poly.cend(); ++i, ++j) {
    area = area + static_cast<double>(CrossProduct(*i, *j));
  }
  return ::fabs(area / 2.0);
}

void ComputeCrossingPoints(const Vectors& poly,
                           const Vector& p,
                           const Vector& q,
                           std::back_insert_iterator<Vectors> back_inserter) {
  Rational line1[3], line2[3];
  ComputeLine(p, q, line1);
  for (auto i = poly.cbegin(), j = i + 1; j != poly.cend(); ++i, ++j) {
    ComputeLine(*i, *j, line2);
    Matrix m(line1[0], line1[1], line2[0], line2[1]);
    if (ComputeInverseMatrix(m, &m)) {
      Vector crossing_point = m * Vector(line1[2], line2[2]);
      if (IsInSegment(*i, *j, crossing_point) &&
          IsInSegment(p, q, crossing_point)) {
        LOG << "cross=" << crossing_point << ' ';
        LOG << p << '~' << q << ' ';
        LOG << *i << '~' << *j << std::endl;
        *back_inserter++ = crossing_point;
      }
    }
  }
}

// Andrew's monotone chain 2D convex hull algorithm.
// Courtesy of wikibooks.org.
void ComputeConvexHull(Vectors poly, Vectors* convex_hull) {
  auto lexicography_order = [](const Vector& p0, const Vector& p1) {
    return p0.x() < p1.x() || (p0.x() == p1.x() && p0.y() < p1.y());
  };

  std::sort(poly.begin(), poly.end(), lexicography_order);

  convex_hull->resize(2 * poly.size());
  auto begin = convex_hull->begin(), k = begin;

  // Build lower hull.
  for (auto i = poly.begin(); i != poly.end(); ++i) {
    while (k >= begin + 2 &&
           CrossProduct(*(k - 1) - *(k - 2), *i - *(k - 2)) >= Rational::zero) {
      --k;
    }
    *k++ = *i;
  }

  // Build upper hull.
  auto t = k + 1;
  for (auto i = poly.rbegin() + 1; i != poly.rend(); ++i) {
    while (k >= t &&
           CrossProduct(*(k - 1) - *(k - 2), *i - *(k - 2)) >= Rational::zero) {
      --k;
    }
    *k++ = *i;
  }

  // NOTE: Last point is the same as the first one.
  convex_hull->resize(k - begin);
}

// Return true if all points are inside or on the border of the polygon.
bool IntersectHelper(const Vectors& poly, const Vectors& points, Vectors* out) {
  bool is_all_inside = true;
  for (auto i = points.cbegin(), j = i + 1; j != points.cend(); ++i, ++j) {
    if (IsInside(poly, *i) || IsOnBorder(poly, *i)) {
      out->push_back(*i);
    } else {
      is_all_inside = false;
    }
    ComputeCrossingPoints(poly, *i, *j, std::back_inserter(*out));
  }
  return is_all_inside;
}

void Intersect(const Vectors& poly1, const Vectors& poly2, Vectors* out) {
  Vectors points;
  if (IntersectHelper(poly1, poly2, &points)) {
    out->assign(poly2.cbegin(), poly2.cend());
    return;
  }
  if (IntersectHelper(poly2, poly1, &points)) {
    out->assign(poly1.cbegin(), poly1.cend());
    return;
  }
  LOG << "intersection_points=" << points << std::endl;
  if (points.empty()) {
    out->clear();
    return;
  }
  ComputeConvexHull(points, out);
}

int main() {
  Vectors poly1, poly2, intersection;
  while (ReadVectors(std::cin, &poly1) && ReadVectors(std::cin, &poly2)) {
    LOG << "poly1=" << poly1 << std::endl;
    LOG << "poly2=" << poly2 << std::endl;
    if (poly1.size() == 1 || poly2.size() == 1) {
      std::cout << "    0.00";
      continue;
    }
    // Add sentinels.
    poly1.push_back(poly1.front());
    poly2.push_back(poly2.front());

    LOG << "poly1_area=" << ComputeArea(poly1) << std::endl;
    LOG << "poly2_area=" << ComputeArea(poly2) << std::endl;
    double area = ComputeArea(poly1) + ComputeArea(poly2);
    Intersect(poly1, poly2, &intersection);
    if (!intersection.empty()) {
      LOG << "intersection=" << intersection << ' '
          << "area=" << ComputeArea(intersection) << std::endl;
      area = area - ComputeArea(intersection) * 2;
    }

    char buffer[16];
    ::snprintf(buffer, sizeof(buffer), "%8.2f", area);
    std::cout << buffer;
  }
  std::cout << std::endl;
  return 0;
}
