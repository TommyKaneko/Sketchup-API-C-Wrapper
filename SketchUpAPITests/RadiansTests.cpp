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
  auto expected = 0.71681469282041377;
  ASSERT_DOUBLE_EQ(expected, radians);	
}

TEST(Radians, InitializeRollAroundNegative)
{
  // Radians always produces a positive number between 0 and 2*PI. A negative
  // number will be converted to its positive equivalent. So 5.5663706143591725
  // is the same as -0.71681469282041377
  CW::Radians radians(-7.0);
  auto expected = 5.5663706143591725;
  ASSERT_DOUBLE_EQ(expected, radians);	
}
