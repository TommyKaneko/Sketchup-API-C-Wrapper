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

#include "SUAPI-CppWrapper/model/InstancePath.hpp"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/DimensionRadial.hpp"
#include "SUAPI-CppWrapper/model/DimensionLinear.hpp"
#include "SUAPI-CppWrapper/model/Dimension.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/DimensionStyle.hpp"
#include "SUAPI-CppWrapper/model/Font.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/Color.hpp"

namespace CW::Tests {

/***********************************
 * DimensionLinear Standalone Tests
 ***********************************/

TEST(DimensionLinearTest, CreateDefault) {
  CW::DimensionLinear dim;
  EXPECT_TRUE(!dim);
}

TEST(DimensionLinearTest, CreateWithPoints) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  EXPECT_FALSE(!dim);

  // Check start and end points
  CW::Point3D s = dim.start_point();
  CW::Point3D e = dim.end_point();
  EXPECT_NEAR(s.x, 0.0, 0.001);
  EXPECT_NEAR(e.x, 10.0, 0.001);

  // Check type via base class
  EXPECT_EQ(dim.type(), SUDimensionType_Linear);
}

TEST(DimensionLinearTest, SetStartEndPoints) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  // Change start point
  dim.start_point(CW::Point3D(1, 1, 0));
  CW::Point3D new_start = dim.start_point();
  EXPECT_NEAR(new_start.x, 1.0, 0.001);
  EXPECT_NEAR(new_start.y, 1.0, 0.001);

  // Change end point
  dim.end_point(CW::Point3D(20, 0, 0));
  CW::Point3D new_end = dim.end_point();
  EXPECT_NEAR(new_end.x, 20.0, 0.001);
}

TEST(DimensionLinearTest, TextProperty) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  // Set custom text
  dim.text(std::string("Custom Text"));
  CW::String txt = dim.text();
  EXPECT_EQ(txt.std_string(), "Custom Text");
}

TEST(DimensionLinearTest, NormalAndXAxis) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  CW::Vector3D axis = dim.x_axis();
  CW::Vector3D norm = dim.normal();
  // The axis should be along the measured direction
  // The normal should be perpendicular to plane
  EXPECT_FALSE(axis == CW::Vector3D(0, 0, 0));
  EXPECT_FALSE(norm == CW::Vector3D(0, 0, 0));
}

TEST(DimensionLinearTest, Position2D) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  auto pos = dim.position();
  // Just verify we can get the position without error
  // Set a new position
  dim.position(5.0, 3.0);
  auto new_pos = dim.position();
  EXPECT_NEAR(new_pos.first, 5.0, 0.001);
  EXPECT_NEAR(new_pos.second, 3.0, 0.001);
}

TEST(DimensionLinearTest, VerticalAlignment) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  dim.vertical_alignment(SUVerticalTextPositionAbove);
  EXPECT_EQ(dim.vertical_alignment(), SUVerticalTextPositionAbove);

  dim.vertical_alignment(SUVerticalTextPositionBelow);
  EXPECT_EQ(dim.vertical_alignment(), SUVerticalTextPositionBelow);
}

TEST(DimensionLinearTest, HorizontalAlignment) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  dim.horizontal_alignment(SUHorizontalTextPositionOutsideStart);
  EXPECT_EQ(dim.horizontal_alignment(), SUHorizontalTextPositionOutsideStart);

  dim.horizontal_alignment(SUHorizontalTextPositionCenter);
  EXPECT_EQ(dim.horizontal_alignment(), SUHorizontalTextPositionCenter);
}

TEST(DimensionLinearTest, Text3D) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  dim.text_3d(true);
  EXPECT_TRUE(dim.text_3d());

  dim.text_3d(false);
  EXPECT_FALSE(dim.text_3d());
}

TEST(DimensionLinearTest, ArrowType) {
  CW::Point3D start(0, 0, 0);
  CW::Point3D end(10, 0, 0);
  CW::DimensionLinear dim(start, end, 2.0);
  ASSERT_FALSE(!dim);

  dim.arrow_type(SUArrowSlash);
  EXPECT_EQ(dim.arrow_type(), SUArrowSlash);
}

TEST(DimensionLinearTest, NullThrows) {
  CW::DimensionLinear dim;
  EXPECT_THROW(dim.start_point(), std::logic_error);
  EXPECT_THROW(dim.end_point(), std::logic_error);
  EXPECT_THROW(dim.text(), std::logic_error);
  EXPECT_THROW(dim.type(), std::logic_error);
}

/****************************
 * DimensionRadial Tests
 ****************************/

TEST(DimensionRadialTest, CreateDefault) {
  CW::DimensionRadial dim;
  EXPECT_TRUE(!dim);
}

TEST(DimensionRadialTest, NullThrows) {
  CW::DimensionRadial dim;
  EXPECT_THROW(dim.is_diameter(), std::logic_error);
  EXPECT_THROW(dim.leader_break_point(), std::logic_error);
  EXPECT_THROW(dim.leader_points(), std::logic_error);
  EXPECT_THROW(dim.curve_instance_path(), std::logic_error);
}

/****************************
 * DimensionStyle Tests
 ****************************/

TEST(DimensionStyleTest, CreateDefault) {
  CW::DimensionStyle style;
  EXPECT_TRUE(!style);
}

TEST(DimensionStyleTest, NullThrows) {
  CW::DimensionStyle style;
  EXPECT_THROW(style.font(), std::logic_error);
  EXPECT_THROW(style.has_3d(), std::logic_error);
  EXPECT_THROW(style.arrow_type(), std::logic_error);
  EXPECT_THROW(style.color(), std::logic_error);
}

TEST_F(ModelLoad, DimensionStyleFromModel) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  DimensionStyle style = m_model->dimension_style();
  // DimensionStyle should always be available from a model
  EXPECT_FALSE(!style);
  // Read some properties
  Font f = style.font();
  EXPECT_FALSE(!f);
  size_t arrow_sz = style.arrow_size();
  EXPECT_GT(arrow_sz, 0u);
  Color c = style.color();
  // Just verify no crash
  size_t line_wt = style.line_weight();
  EXPECT_GT(line_wt, 0u);
}

/****************************
 * Model Integration Tests
 ****************************/

#if SketchUpAPI_VERSION_MAJOR >= 2026
// NOTE: "box and box.skp" may not contain dimension entities.
// If this test reports 0 dimensions, the user should add dimension entities
// to the test model for more comprehensive testing.
TEST_F(ModelLoad, CopyDimensions) {
  using namespace CW;
  ASSERT_FALSE(!m_model);
  Entities entities = m_model->entities();
  std::vector<Dimension> dims = entities.dimensions();
  Entities dest_entities = m_model_copy->entities(); // Just get entities to add to - could be same or different model
  // Just verify the call succeeds - the model may or may not have dimensions
  for (const Dimension& dim : dims) {
    ASSERT_FALSE(!dim);
    // Check type is valid
    SUDimensionType t = dim.type();
    EXPECT_TRUE(t == SUDimensionType_Linear || t == SUDimensionType_Radial);
  }
  dest_entities.add_dimensions(dims);
}
#endif

} // namespace CW::Tests
