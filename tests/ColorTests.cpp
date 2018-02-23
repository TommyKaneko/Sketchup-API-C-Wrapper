#include "SketchUpAPITests.hpp"
#include "gtest/gtest.h"

#include <SketchUpAPI/sketchup.h>

#include "SUAPI-CppWrapper/Color.hpp"


TEST(SUColor, Initialize)
{
  SUColor su_color{ 255, 128, 64, 32 };
  CW::Color color(su_color);

  ASSERT_EQ(255, color.red);
  ASSERT_EQ(128, color.green);
  ASSERT_EQ( 64, color.blue);
  ASSERT_EQ( 32, color.alpha);
}

TEST(SUColor, ImplicitCastToSUColor)
{
  CW::Color color({ 255, 128, 64, 32 });
  SUColor su_color = color;

  ASSERT_EQ(255, su_color.red);
  ASSERT_EQ(128, su_color.green);
  ASSERT_EQ( 64, su_color.blue);
  ASSERT_EQ( 32, su_color.alpha);
}

TEST(SUColor, ImplicitCastFromSUColor)
{
  SUColor su_color{ 255, 128, 64, 32 };
  CW::Color color = su_color;

  ASSERT_EQ(255, color.red);
  ASSERT_EQ(128, color.green);
  ASSERT_EQ( 64, color.blue);
  ASSERT_EQ( 32, color.alpha);
}

TEST(SUColor, EqualComparison)
{
  CW::Color color1({ 255, 128, 64, 32 });
  CW::Color color2({ 255, 128, 64, 32 });

  ASSERT_TRUE(color1 == color2);
}
