#include "gtest/gtest.h"

#include <cmath>

#include "SUAPI-CppWrapper/Geometry.hpp"


TEST(Radians, InitializeWithinRange)
{
  CW::Radians radians(1.0);
  ASSERT_DOUBLE_EQ(1.0, radians);	
}

TEST(Radians, InitializeRollAroundPositive)
{
  CW::Radians radians(7.0);
  auto expected = 7.0 - (CW::Radians::PI * 2);
  ASSERT_DOUBLE_EQ(expected, radians);	
}

TEST(Radians, InitializeRollAroundNegative)
{
  CW::Radians radians(-7.0);
  auto expected = -7.0 + (CW::Radians::PI * 2);
  ASSERT_DOUBLE_EQ(expected, radians);	
}
