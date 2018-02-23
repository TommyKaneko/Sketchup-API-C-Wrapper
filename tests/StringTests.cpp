#include "SketchUpAPITests.hpp"
#include "gtest/gtest.h"

#include <SketchUpAPI/sketchup.h>

#include <string>

#include "SUAPI-CppWrapper/String.hpp"


TEST(String, std_string)
{
  std::string expected = "Hello World";

  SUStringRef su_string = SU_INVALID;
  SU(SUStringCreateFromUTF8(&su_string, expected.c_str()));
  size_t su_string_size = 0;
  SU(SUStringGetUTF8Length(su_string, &su_string_size));

  CW::String string(su_string);
  auto std_string = string.std_string();

  ASSERT_EQ(expected.size(), su_string_size);
  ASSERT_EQ(expected.size(), string.size());
  ASSERT_EQ(expected.size(), std_string.size());

  ASSERT_STREQ(expected.c_str(), std_string.c_str());
}
