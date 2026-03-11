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

#include "SUAPI-CppWrapper/model/DynamicComponentInfo.hpp"
#include "SUAPI-CppWrapper/model/DynamicComponentAttribute.hpp"

namespace CW::Tests {

TEST(DynamicComponentInfoTest, DefaultConstructor) {
  CW::DynamicComponentInfo info;
  EXPECT_TRUE(!info);
}

TEST(DynamicComponentInfoTest, NullThrows) {
  CW::DynamicComponentInfo info;
  EXPECT_THROW(info.num_attributes(), std::logic_error);
  EXPECT_THROW(info.attributes(), std::logic_error);
}

TEST(DynamicComponentInfoTest, MoveConstructor) {
  CW::DynamicComponentInfo info1;
  CW::DynamicComponentInfo info2(std::move(info1));
  EXPECT_TRUE(!info1);
  EXPECT_TRUE(!info2);
}

TEST(DynamicComponentInfoTest, MoveAssignment) {
  CW::DynamicComponentInfo info1;
  CW::DynamicComponentInfo info2;
  info2 = std::move(info1);
  EXPECT_TRUE(!info1);
  EXPECT_TRUE(!info2);
}

} // namespace CW::Tests
