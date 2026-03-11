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
#include "SUAPI-CppWrapper/model/Location.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"

namespace CW::Tests {

TEST(LocationTest, DefaultConstructor) {
  CW::Location loc;
  EXPECT_TRUE(!loc);
}

TEST(LocationTest, NullThrows) {
  CW::Location loc;
  EXPECT_THROW(loc.lat_long(), std::logic_error);
  EXPECT_THROW(loc.set_lat_long(0.0, 0.0), std::logic_error);
  EXPECT_THROW(loc.set_north_angle(0.0), std::logic_error);
}

TEST_F(ModelLoad, LocationFromModel) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  bool geo = m_model->georeferenced();
  // Just verify the call succeeds without throwing
  (void)geo;
}


/**********************
 * Location Model Copy Tests
 **********************/

class LocationModelTest : public ModelLoad {};

TEST_F(LocationModelTest, CopyLocation) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  ASSERT_FALSE(!m_model_copy);

  // Read location from source model
  Location src_loc = m_model->location();
  ASSERT_FALSE(!src_loc);

  auto [src_lat, src_long] = src_loc.lat_long();

  // Set location on destination model
  Location dest_loc = m_model_copy->location();
  ASSERT_FALSE(!dest_loc);
  dest_loc.set_lat_long(src_lat, src_long);

  // Verify the location was set correctly
  auto [dest_lat, dest_long] = dest_loc.lat_long();
  EXPECT_DOUBLE_EQ(src_lat, dest_lat);
  EXPECT_DOUBLE_EQ(src_long, dest_long);

  SaveModel("Location");
}

} // namespace CW::Tests
