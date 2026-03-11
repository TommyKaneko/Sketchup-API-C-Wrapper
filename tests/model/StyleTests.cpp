// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "gtest/gtest.h"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/Style.hpp"
#include "SUAPI-CppWrapper/model/Styles.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Color.hpp"

namespace CW::Tests {

/***************
 * Style Tests *
 ***************/

TEST(Style, DefaultConstructor) {
  CW::Style style;
  EXPECT_TRUE(!style);
}

TEST(Style, CreateWithName) {
  CW::Style style("My Style");
  EXPECT_FALSE(!style);
  EXPECT_EQ(std::string(style.name()), "My Style");
}

TEST(Style, SetNameAndDescription) {
  CW::Style style("Original Style");
  style.name("New Style");
  EXPECT_EQ(std::string(style.name()), "New Style");
  style.description("A test style description");
  EXPECT_EQ(std::string(style.description()), "A test style description");
}

TEST(Style, NullThrows) {
  CW::Style style;
  EXPECT_THROW(style.name(), std::logic_error);
  EXPECT_THROW(style.description(), std::logic_error);
  EXPECT_THROW(style.guid(), std::logic_error);
  EXPECT_THROW(style.displays_watermark(), std::logic_error);
}

TEST(Style, CopyConstructor) {
  CW::Style style("Copy Style");
  style.description("Copy desc");
  CW::Style copy(style);
  EXPECT_FALSE(!copy);
  EXPECT_EQ(std::string(copy.name()), "Copy Style");
  EXPECT_EQ(std::string(copy.description()), "Copy desc");
  // Different ref pointers for detached copies
  EXPECT_NE(style.ref().ptr, copy.ref().ptr);
}

TEST(Style, CopyAssignment) {
  CW::Style style("Assign Style");
  CW::Style copy;
  copy = style;
  EXPECT_FALSE(!copy);
  EXPECT_EQ(std::string(copy.name()), "Assign Style");
}

TEST(Style, PropertyGetSet) {
  CW::Style style("Prop Style");
  // Set background color
  SUColor su_color = {255, 0, 0, 255};
  CW::Color color_obj(su_color);
  CW::TypedValue bg_color(color_obj);
  style.set_property(SUStyleBackgroundColor, bg_color);
  CW::TypedValue retrieved = style.get_property(SUStyleBackgroundColor);
  CW::Color color = retrieved.color_value();
  SUColor raw = color.ref();
  EXPECT_EQ(raw.red, 255);
}


/************************************
 * Style Model Copy Tests           *
 ************************************/

 // TODO: Currently, the only way to copy styles from one model to another is to save the Style to file, then load it into the other model.
// class StyleModelTest : public ModelLoad {};

// TEST_F(StyleModelTest, CopyStyle) {
//   using namespace CW;
//   ASSERT_FALSE(!m_model);
//   ASSERT_FALSE(!m_model_copy);

//   // Get styles from source model
//   Styles src_styles = m_model->styles();
//   std::vector<Style> all_src_styles = src_styles.styles();
//   ASSERT_GT(all_src_styles.size(), 0u);
//   // Copy all styles to dest model
//   Styles dest_styles = m_model_copy->styles();
//   for (const auto& src : all_src_styles) {
//     dest_styles.add_style(src.copy());
//   }

//   std::vector<Style> dest_all_styles = dest_styles.styles();
//   ASSERT_EQ(dest_all_styles.size(), all_src_styles.size());
//   for (size_t i = 0; i < all_src_styles.size(); ++i) {
//     const Style& original = all_src_styles[i];
//     const Style& copied = dest_all_styles[i];
//     EXPECT_EQ(std::string(original.name()), std::string(copied.name()));
//     EXPECT_EQ(std::string(original.description()), std::string(copied.description()));
//     EXPECT_EQ(original.displays_watermark(), copied.displays_watermark());
//   }
//   SaveModel("Style");
// }

} /* namespace CW::Tests */
