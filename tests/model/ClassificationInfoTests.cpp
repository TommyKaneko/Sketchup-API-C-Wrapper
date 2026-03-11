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

#include "SUAPI-CppWrapper/model/ClassificationInfo.hpp"
#include "SUAPI-CppWrapper/model/ClassificationAttribute.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW::Tests {

TEST(ClassificationInfoTest, DefaultConstructor) {
  CW::ClassificationInfo info;
  EXPECT_TRUE(!info);
}

TEST(ClassificationInfoTest, NullThrows) {
  CW::ClassificationInfo info;
  EXPECT_THROW(info.num_schemas(), std::logic_error);
  EXPECT_THROW(info.schema_name(0), std::logic_error);
  EXPECT_THROW(info.schema_type(0), std::logic_error);
  EXPECT_THROW(info.schema_attribute(0), std::logic_error);
}

TEST(ClassificationInfoTest, MoveConstructor) {
  // Move from null - resulting object should also be null
  CW::ClassificationInfo info;
  CW::ClassificationInfo moved(std::move(info));
  EXPECT_TRUE(!moved);
}

TEST(ClassificationInfoTest, MoveAssignment) {
  CW::ClassificationInfo info;
  CW::ClassificationInfo other;
  other = std::move(info);
  EXPECT_TRUE(!other);
}

} // namespace CW::Tests
