#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

struct Vector {
  static const Vector ZERO;

  explicit Vector();
  Vector(int x_, int y_);

  int x, y;
};

typedef std::vector<std::pair<int, Vector>> Polygon;

const Vector Vector::ZERO(0, 0);

std::istream& operator>>(std::istream& in, Vector& v);
std::istream& operator>>(std::istream& in, Polygon& p);

std::ostream& operator<<(std::ostream& out, const Vector& v);

bool operator==(const Vector& u, const Vector& v);
bool operator!=(const Vector& u, const Vector& v);
bool operator<(const Vector& u, const Vector& v);

Vector operator+(const Vector& u, const Vector& v);
Vector operator-(const Vector& u, const Vector& v);
Vector operator-(const Vector& u);

bool IsColinear(const Vector& u, const Vector& v, const Vector& w);
bool IsProjectedInside(const Vector& u, const Vector& v, const Vector& target);

Polygon ComputeConvexHull(Polygon polygon);

int ComputeIndex(const Polygon& polygon, const Vector& u, const Vector& v);

int InnerProduct(const Vector& u, const Vector& v);
int CrossProduct(const Vector& u, const Vector& v);

int main() {
  for (std::string name; std::cin >> name && name != "#";) {
    Vector center_of_mass;
    Polygon polygon;
    std::cin >> center_of_mass >> polygon;
    assert(polygon.size() >= 3);

    int min_index = polygon.size() + 1;
    Polygon convex_hull = ComputeConvexHull(polygon);
    for (auto i = convex_hull.cbegin(), j = i + 1; j != convex_hull.cend();
         ++i, ++j) {
      const Vector& u = i->second;
      const Vector& v = j->second;

      // NOTE: convex_hull is a subset of polygon; so pass polygon to
      // ComputeIndex.
      int index = ComputeIndex(polygon, u, v);
      if (DEBUG) {
        std::cerr << "index = " << index << "; ";
        std::cerr << "endpoints = " << u << ", " << v;
      }

      if (!IsProjectedInside(u, v, center_of_mass)) {
        if (DEBUG) {
          std::cerr << "; unstable" << std::endl;
        }
        continue;
      }

      if (DEBUG) {
        std::cerr << std::endl;
      }
      min_index = std::min(min_index, index);
    }
    assert(min_index != (int)polygon.size() + 1);

    std::cout << std::setw(19) << std::left << name << ' ' << min_index
              << std::endl;
  }
  return 0;
}

// Andrew's monotone chain 2D convex hull algorithm.
// Courtesy of wikibooks.org.
Polygon ComputeConvexHull(Polygon polygon) {
  auto lexicography_order = [](
      const std::pair<int, Vector>& i,
      const std::pair<int, Vector>& j) { return i.second < j.second; };
  std::sort(polygon.begin(), polygon.end(), lexicography_order);

  Polygon convex_hull;
  convex_hull.resize(2 * polygon.size());
  Polygon::iterator zero = convex_hull.begin(), k = zero;

  // Build lower hull.
  for (Polygon::iterator i = polygon.begin(); i != polygon.end(); ++i) {
    while (k >= zero + 2) {
      Vector u = (k - 1)->second - (k - 2)->second;
      Vector v = i->second - (k - 2)->second;
      if (CrossProduct(u, v) > 0) {
        break;
      }
      --k;
    }
    *k++ = *i;
  }

  // Build upper hull.
  Polygon::iterator t = k + 1;
  for (Polygon::reverse_iterator i = polygon.rbegin() + 1; i != polygon.rend();
       ++i) {
    while (k >= t) {
      Vector u = (k - 1)->second - (k - 2)->second;
      Vector v = i->second - (k - 2)->second;
      if (CrossProduct(u, v) > 0) {
        break;
      }
      --k;
    }
    *k++ = *i;
  }

  // NOTE: Last point is the same as the first one.
  convex_hull.resize(k - zero);
  return convex_hull;
}

int ComputeIndex(const Polygon& polygon, const Vector& u, const Vector& v) {
  int index = -1;
  for (const auto& k : polygon) {
    if (IsColinear(u, v, k.second)) {
      index = std::max(index, k.first);
    }
  }
  assert(index != -1);
  return index;
}

Vector::Vector() : Vector(0, 0) {}
Vector::Vector(int x_, int y_) : x(x_), y(y_) {}

std::istream& operator>>(std::istream& in, Vector& v) {
  return in >> v.x >> v.y;
}

std::istream& operator>>(std::istream& in, Polygon& p) {
  Vector v;
  for (int i = 1; in >> v && v != Vector::ZERO; i++) {
    p.push_back(std::make_pair(i, v));
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Vector& v) {
  return out << v.x << ' ' << v.y;
}

bool operator==(const Vector& u, const Vector& v) {
  return u.x == v.x && u.y == v.y;
}

bool operator!=(const Vector& u, const Vector& v) { return !(u == v); }

bool operator<(const Vector& u, const Vector& v) {
  return u.x != v.x ? u.x < v.x : u.y < v.y;
}

Vector operator+(const Vector& u, const Vector& v) {
  return Vector(u.x + v.x, u.y + v.y);
}

Vector operator-(const Vector& u, const Vector& v) { return u + (-v); }

Vector operator-(const Vector& u) { return Vector(-u.x, -u.y); }

bool IsColinear(const Vector& u, const Vector& v, const Vector& w) {
  return CrossProduct(w - u, v - u) == 0;
}

bool IsProjectedInside(const Vector& u, const Vector& v, const Vector& target) {
  int product = InnerProduct(target - u, v - u);
  return 0 <= product && product <= InnerProduct(v - u, v - u);
}

int InnerProduct(const Vector& u, const Vector& v) {
  return u.x * v.x + u.y * v.y;
}

int CrossProduct(const Vector& u, const Vector& v) {
  return u.x * v.y - v.x * u.y;
}
