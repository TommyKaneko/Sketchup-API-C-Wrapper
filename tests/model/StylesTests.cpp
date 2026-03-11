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
#include "SUAPI-CppWrapper/model/Styles.hpp"
#include "SUAPI-CppWrapper/model/Style.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW::Tests {

/****************
 * Styles Tests *
 ****************/

TEST(Styles, DefaultConstructor) {
  CW::Styles styles;
  EXPECT_TRUE(!styles);
}

TEST(Styles, NullThrows) {
  CW::Styles styles;
  EXPECT_THROW(styles.size(), std::logic_error);
}

// Test Styles from model
TEST_F(ModelLoad, StylesFromModel) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  Styles model_styles = m_model->styles();
  EXPECT_FALSE(!model_styles);
  size_t count = model_styles.size();
  // Every model has at least one style
  EXPECT_GT(count, 0u);

  // Verify we can get all styles
  std::vector<Style> all_styles = model_styles.styles();
  EXPECT_EQ(all_styles.size(), count);

  // Verify active style exists
  Style active = model_styles.active_style();
  EXPECT_FALSE(!active);

  // Verify selected style exists
  Style selected = model_styles.selected_style();
  EXPECT_FALSE(!selected);

  // Verify each style has a name and guid
  for (const auto& s : all_styles) {
    EXPECT_FALSE(!s);
    String name = s.name();
    EXPECT_GT(std::string(name).size(), 0u);
    String guid = s.guid();
    EXPECT_GT(std::string(guid).size(), 0u);
  }

  // Verify lookup by guid
  if (!all_styles.empty()) {
    std::string guid = std::string(all_styles[0].guid());
    Style found = model_styles.style_by_guid(guid);
    EXPECT_FALSE(!found);
    EXPECT_EQ(std::string(found.guid()), guid);
  }

  // active_style_changed should be queryable
  bool changed = model_styles.active_style_changed();
  // Just verify it doesn't throw
  (void)changed;
}

} /* namespace CW::Tests */
