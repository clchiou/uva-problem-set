#define main _main
#include "109.cc"

#include "gtest/gtest.h"

static void ComparePolygons(const Polygon& poly0, const Polygon& poly1) {
  ASSERT_EQ(poly0.size(), poly1.size());
  for (int i = 0; i < (int)poly0.size(); i++) {
    ASSERT_EQ(poly0[i].x(), poly1[i].x());
    ASSERT_EQ(poly0[i].y(), poly1[i].y());
  }
}

TEST(Test109, TestConvexHull) {
  const Polygon input_1 = {
    Point(0, 0),
    Point(1, 1),
    Point(2, 2),
    Point(3, 3),
    Point(4, 4),
    Point(5, 5),
    Point(6, 6),
    Point(7, 7),
    Point(8, 8),
    Point(0, 8),
  };
  const Polygon output_1 = {
    Point(0, 0),
    Point(8, 8),
    Point(0, 8),
    Point(0, 0),
  };

  const Polygon input_2 = {
    Point(0, 0),
    Point(1, 1),
    Point(2, 2),
  };
  const Polygon output_2 = {
    Point(0, 0),
    Point(2, 2),
    Point(0, 0),
  };

  const Polygon input_3 = {
    Point(0, 0),
    Point(2, 0),
    Point(2, 2),
    Point(0, 2),
    Point(0, 0),
  };
  const Polygon output_3 = {
    Point(0, 0),
    Point(2, 0),
    Point(2, 2),
    Point(0, 2),
    Point(0, 0),
  };

  Polygon input, convex_hull, output;

  ComputeConvexHull(input_1, &convex_hull);
  ComparePolygons(output_1, convex_hull);
  ASSERT_DOUBLE_EQ(32, ComputeArea(convex_hull));
  ASSERT_TRUE(IsInside(convex_hull, Point(1, 2)));
  ASSERT_FALSE(IsInside(convex_hull, Point(3, 3)));
  ASSERT_FALSE(IsInside(convex_hull, Point(5, 10)));
  ASSERT_FALSE(IsInside(convex_hull, Point(0, 4)));
  ASSERT_FALSE(IsInside(convex_hull, Point(-1, 4)));

  ComputeConvexHull(input_2, &convex_hull);
  ComparePolygons(output_2, convex_hull);
  ASSERT_DOUBLE_EQ(0, ComputeArea(convex_hull));
  ASSERT_FALSE(IsInside(convex_hull, Point(0, 0)));
  ASSERT_FALSE(IsInside(convex_hull, Point(1, 1)));
  ASSERT_FALSE(IsInside(convex_hull, Point(2, 2)));
  ASSERT_FALSE(IsInside(convex_hull, Point(3, 3)));

  ComputeConvexHull(input_3, &convex_hull);
  ComparePolygons(output_3, convex_hull);
  ASSERT_DOUBLE_EQ(4, ComputeArea(convex_hull));
}
