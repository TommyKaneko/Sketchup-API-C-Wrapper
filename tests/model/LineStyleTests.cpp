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
#include "SUAPI-CppWrapper/model/LineStyle.hpp"
#include "SUAPI-CppWrapper/model/LineStyles.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW::Tests {

/*******************
 * LineStyle Tests *
 *******************/

TEST(LineStyle, DefaultConstructor) {
  CW::LineStyle style;
  EXPECT_TRUE(!style);
}

TEST(LineStyle, NullThrows) {
  CW::LineStyle style;
  EXPECT_THROW(style.name(), std::logic_error);
}

// Test LineStyles from model (requires model to have line styles)
TEST_F(ModelLoad, LineStylesFromModel) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  LineStyles ls = m_model->line_styles();
  EXPECT_FALSE(!ls);
  size_t count = ls.size();
  // Every model should have at least one built-in line style
  EXPECT_GT(count, 0u);

  // Get names and verify non-empty
  std::vector<String> names = ls.names();
  EXPECT_EQ(names.size(), count);

  // Retrieve a line style by name
  if (!names.empty()) {
    LineStyle first = ls.line_style_by_name(std::string(names[0]));
    EXPECT_FALSE(!first);
    EXPECT_EQ(std::string(first.name()), std::string(names[0]));
  }
}


/************************************
 * LineStyle Model Copy Tests       *
 ************************************/

class LineStyleModelTest : public ModelLoad {};

TEST_F(LineStyleModelTest, CopyLineStylesBetweenModels) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  ASSERT_FALSE(!m_model_copy);

  // Get line styles from source model
  LineStyles src_ls = m_model->line_styles();
  ASSERT_FALSE(!src_ls);
  size_t count = src_ls.size();
  ASSERT_GT(count, 0u);

  std::vector<String> names = src_ls.names();

  // For each line style, read its properties
  for (const auto& style_name : names) {
    LineStyle src_style = src_ls.line_style_by_name(std::string(style_name));
    ASSERT_FALSE(!src_style);

    // Verify properties are accessible
    EXPECT_GT(std::string(src_style.name()).size(), 0u);
    double width = src_style.width_pixels();
    EXPECT_GT(width, 0.0);
    double multiplier = src_style.length_multiplier();
    EXPECT_GT(multiplier, 0.0);
  }

  // Verify destination model also has line styles (built-in)
  LineStyles dest_ls = m_model_copy->line_styles();
  ASSERT_FALSE(!dest_ls);
  EXPECT_GT(dest_ls.size(), 0u);

  // Verify source and dest have same built-in line styles
  std::vector<String> dest_names = dest_ls.names();
  EXPECT_EQ(names.size(), dest_names.size());
}


TEST_F(LineStyleModelTest, CreateLineStyleCopy) {
  using namespace CW;
  ASSERT_FALSE(!m_model);

  LineStyles ls = m_model->line_styles();
  std::vector<String> names = ls.names();
  ASSERT_GT(names.size(), 0u);

  // Get first line style and create a copy
  LineStyle original = ls.line_style_by_name(std::string(names[0]));
  ASSERT_FALSE(!original);

  LineStyle copy = original.create_copy("Custom Copy Style");
  EXPECT_FALSE(!copy);
  EXPECT_EQ(std::string(copy.name()), "Custom Copy Style");

  // Copied style should have same width and multiplier
  EXPECT_DOUBLE_EQ(original.width_pixels(), copy.width_pixels());
  EXPECT_DOUBLE_EQ(original.length_multiplier(), copy.length_multiplier());
}

} /* namespace CW::Tests */
