#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif  // ONLINE_JUDGE
};

class Point {
 public:
  Point();
  Point(int x, int y);

  friend std::istream& operator>>(std::istream& in, Point& point);

  int x() const;
  int y() const;

 private:
  int x_, y_;
};

std::ostream& operator<<(std::ostream& out, const Point& point);

typedef std::vector<Point> Polygon;

std::istream& operator>>(std::istream& in, Polygon& polygon);
std::ostream& operator<<(std::ostream& out, const Polygon& polygon);

void ComputeConvexHull(const Polygon& polygon, Polygon* convex_hull);

double ComputeArea(const Polygon& polygon);

bool IsInside(const Polygon& polygon, const Point& point);

int CrossProduct(const Point& p1, const Point& p2, const Point& p3);

int main() {
  std::vector<Polygon> kingdoms;
  for (Polygon kingdom; std::cin >> kingdom && kingdom.size() > 0;) {
    kingdoms.push_back(kingdom);
  }
  if (DEBUG) {
    std::cerr << "Kingdoms" << std::endl;
    for (const auto& kingdom : kingdoms) {
      std::cerr << kingdom;
    }
    std::cerr << "---" << std::endl;
  }

  std::vector<Polygon> convex_hulls;
  for (const auto& kingdom : kingdoms) {
    Polygon convex_hull;
    ComputeConvexHull(kingdom, &convex_hull);
    if (ComputeArea(convex_hull) > 0) {
      convex_hulls.push_back(convex_hull);
    }
  }
  if (DEBUG) {
    std::cerr << "Convex hulls" << std::endl;
    for (const auto& convex_hull : convex_hulls) {
      std::cerr << convex_hull;
      std::cerr << "area=" << ComputeArea(convex_hull) << std::endl;
    }
    std::cerr << "---" << std::endl;
  }

  double area = 0;
  for (Point missile; std::cin >> missile;) {
    if (DEBUG) {
      std::cerr << "missile=" << missile << std::endl;
    }
    for (std::vector<Polygon>::iterator i = convex_hulls.begin();
         i != convex_hulls.end();) {
      if (IsInside(*i, missile)) {
        if (DEBUG) {
          std::cerr << "Hit!" << std::endl;
          std::cerr << *i;
          std::cerr << "---" << std::endl;
        }
        area += ComputeArea(*i);
        i = convex_hulls.erase(i);
      } else {
        ++i;
      }
    }
    if (convex_hulls.empty()) {
      break;
    }
  }
  if (DEBUG) {
    std::cerr << "area=" << area << std::endl;
  }
  printf("%.2f\n", area);

  return 0;
}

// Andrew's monotone chain 2D convex hull algorithm.
// Courtesy of wikibooks.org.
void ComputeConvexHull(const Polygon& polygon, Polygon* convex_hull) {
  auto lexicography_order = [](const Point& p0, const Point& p1) {
    return p0.x() < p1.x() || (p0.x() == p1.x() && p0.y() < p1.y());
  };

  Polygon polygon_(polygon);
  std::sort(polygon_.begin(), polygon_.end(), lexicography_order);

  convex_hull->resize(2 * polygon_.size());
  Polygon::iterator zero = convex_hull->begin(), k = zero;

  // Build lower hull.
  for (Polygon::iterator i = polygon_.begin(); i != polygon_.end(); ++i) {
    while (k >= zero + 2 && CrossProduct(*(k - 2), *(k - 1), *i) <= 0) {
      --k;
    }
    *k++ = *i;
  }

  // Build upper hull.
  Polygon::iterator t = k + 1;
  for (Polygon::reverse_iterator i = polygon_.rbegin() + 1;
       i != polygon_.rend();
       ++i) {
    while (k >= t && CrossProduct(*(k - 2), *(k - 1), *i) <= 0) {
      --k;
    }
    *k++ = *i;
  }

  // NOTE: Last point is the same as the first one.
  convex_hull->resize(k - zero);
}

double ComputeArea(const Polygon& polygon) {
  double area = 0;
  for (Polygon::const_iterator i = polygon.cbegin() + 1; i != polygon.cend();
       ++i) {
    const Point& p0 = *(i - 1);
    const Point& p1 = *i;
    area += p0.x() * p1.y() - p1.x() * p0.y();
  }
  return std::fabs(area / 2);
}

bool IsInside(const Polygon& polygon, const Point& point) {
  enum { LEFT, RIGHT, NONE, } prev_side = NONE, side = NONE;
  for (Polygon::const_iterator i = polygon.cbegin() + 1; i != polygon.cend();
       ++i) {
    int product = CrossProduct(*(i - 1), *i, point);
    if (product == 0) {
      return false;
    }
    side = product > 0 ? RIGHT : LEFT;
    if (prev_side == NONE) {
      prev_side = side;
    } else if (prev_side != side) {
      return false;
    }
  }
  return true;
}

int CrossProduct(const Point& p1, const Point& p2, const Point& p3) {
  return (p2.x() - p1.x()) * (p3.y() - p1.y()) -
         (p2.y() - p1.y()) * (p3.x() - p1.x());
}

Point::Point() : x_(0), y_(0) {}
Point::Point(int x, int y) : x_(x), y_(y) {}

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_;
  return in;
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
  return out << point.x() << ' ' << point.y();
}

int Point::x() const { return x_; }
int Point::y() const { return y_; }

std::istream& operator>>(std::istream& in, Polygon& polygon) {
  int num_points;
  in >> num_points;
  if (num_points <= 0) {
    polygon.resize(0);
    return in;
  }
  polygon.resize(num_points);
  for (Polygon::iterator i = polygon.begin(); i != polygon.end(); ++i) {
    in >> *i;
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Polygon& polygon) {
  out << polygon.size() << std::endl;
  for (Polygon::const_iterator i = polygon.begin(); i != polygon.end(); ++i) {
    out << *i << std::endl;
  }
  return out;
}
