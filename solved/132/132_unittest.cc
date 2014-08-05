#define main _main
#include "132.cc"

#include "gtest/gtest.h"

TEST(Test132, TestIsColinear) {
  ASSERT_TRUE(IsColinear(Vector(1, 1), Vector(2, 2), Vector(3, 3)));
  ASSERT_TRUE(IsColinear(Vector(2, 2), Vector(2, 2), Vector(3, 3)));
  ASSERT_TRUE(IsColinear(Vector(1, 1), Vector(2, 2), Vector(2, 2)));
  ASSERT_TRUE(IsColinear(Vector(3, 3), Vector(2, 2), Vector(3, 3)));

  ASSERT_FALSE(IsColinear(Vector(1, 1), Vector(2, 2), Vector(4, 3)));
  ASSERT_FALSE(IsColinear(Vector(1, 1), Vector(2, 3), Vector(3, 3)));
  ASSERT_FALSE(IsColinear(Vector(1, -1), Vector(2, 2), Vector(3, 3)));
}

TEST(Test132, TestIsProjectedInside) {
  ASSERT_TRUE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(0, 2)));
  ASSERT_TRUE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(1, 2)));
  ASSERT_TRUE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(2, 2)));
  ASSERT_TRUE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(0, -2)));
  ASSERT_TRUE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(1, -2)));
  ASSERT_TRUE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(2, -2)));

  ASSERT_FALSE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(-1, 2)));
  ASSERT_FALSE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(3, 2)));
  ASSERT_FALSE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(-1, -2)));
  ASSERT_FALSE(IsProjectedInside(Vector(0, 0), Vector(2, 0), Vector(3, -2)));
}
