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
#include "SUAPI-CppWrapper/model/Classifications.hpp"
#include "SUAPI-CppWrapper/model/Schema.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"

namespace CW::Tests {

TEST(ClassificationsTest, DefaultConstructor) {
  CW::Classifications cls;
  EXPECT_TRUE(!cls);
}

TEST(ClassificationsTest, NullThrows) {
  CW::Classifications cls;
  EXPECT_THROW(cls.load_schema("test.xsd"), std::logic_error);
  EXPECT_THROW(cls.num_schemas(), std::logic_error);
  EXPECT_THROW(cls.schemas(), std::logic_error);
}

TEST_F(ModelLoad, ClassificationsFromModel) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  Classifications cls = m_model->classifications();
  // Classifications should be retrievable from any model
  EXPECT_FALSE(!cls);
  // The default box model may not have any schemas loaded
  size_t count = cls.num_schemas();
  (void)count; // Just verify it doesn't throw
}

} // namespace CW::Tests
