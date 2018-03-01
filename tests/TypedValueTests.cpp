#include "SketchUpAPITests.hpp"
#include "gtest/gtest.h"

#include <vector>

#include <SketchUpAPI/sketchup.h>

#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/AttributeDictionary.hpp"


TEST(TypedValue, bool_value_true)
{
  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  SU(SUTypedValueSetBool(su_typed_value, true));

  CW::TypedValue typed_value(su_typed_value);
  auto value = typed_value.bool_value();

  ASSERT_TRUE(value);
}

TEST(TypedValue, bool_value_false)
{
  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  SU(SUTypedValueSetBool(su_typed_value, false));

  CW::TypedValue typed_value(su_typed_value);
  auto value = typed_value.bool_value();

  ASSERT_FALSE(value);
}

TEST(DISABLED_TypedValue, bool_value_wrong_type)
{
  SUColor color{ 255, 128, 64, 32 };

  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  SU(SUTypedValueSetColor(su_typed_value, &color));

  CW::TypedValue typed_value(su_typed_value);
  // TODO(thomthom): Should be a custom exception type:
  ASSERT_THROW(typed_value.bool_value(), std::exception);
}


TEST(TypedValue, typed_value_array_GetInts)
{
  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  std::vector<SUTypedValueRef> su_values;
  for (int32_t i = 0; i < 3; ++i)
  {
    SUTypedValueRef su_value = SU_INVALID;
    SU(SUTypedValueCreate(&su_value));
    SU(SUTypedValueSetInt32(su_value, i));
    su_values.emplace_back(su_value);
  }
  SU(SUTypedValueSetArrayItems(su_typed_value, su_values.size(), su_values.data()));

  CW::TypedValue typed_value(su_typed_value);
  auto values = typed_value.typed_value_array();

  ASSERT_EQ(3, values.size());

  ASSERT_EQ(0, values.at(0).int32_value());
  ASSERT_EQ(1, values.at(1).int32_value());
  ASSERT_EQ(2, values.at(2).int32_value());
}

TEST(TypedValue, get_mixed_array_items)
{
  const char filepath[] = "C:/Users/Jim/test-model.skp";
  CW::initialize();
  auto model = CW::Model(filepath);
  CW::TypedValue typed_value_array = model.get_attribute("TypedValues", "Array<mixed>", typed_value_array);
  size_t count = 0;
  SUResult res = SUTypedValueGetNumArrayItems(typed_value_array, &count);
  ASSERT_EQ(3, count);
}
