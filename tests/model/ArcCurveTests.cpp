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
#include "SUAPI-CppWrapper/model/ArcCurve.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"

namespace CW::Tests {

/**********************
 * ArcCurve Tests
 **********************/

// Test default constructor creates a null arc curve
TEST(ArcCurveTest, DefaultConstructor)
{
  CW::ArcCurve arc;
  EXPECT_TRUE(!arc);
}

// Test creating an arc (semicircle)
TEST(ArcCurveTest, CreateArc)
{
  CW::Point3D center(0, 0, 0);
  CW::Point3D start(10, 0, 0);
  CW::Point3D end(-10, 0, 0);
  CW::Vector3D normal(0, 0, 1);
  size_t num_edges = 12;

  CW::ArcCurve arc(center, start, end, normal, num_edges);
  EXPECT_FALSE(!arc);

  EXPECT_NEAR(arc.radius(), 10.0, 0.001);
  EXPECT_FALSE(arc.is_full_circle());

  CW::Point3D arc_center = arc.center();
  EXPECT_NEAR(arc_center.x, 0.0, 0.001);
  EXPECT_NEAR(arc_center.y, 0.0, 0.001);
  EXPECT_NEAR(arc_center.z, 0.0, 0.001);

  CW::Point3D arc_start = arc.start_point();
  EXPECT_NEAR(arc_start.x, 10.0, 0.001);
  EXPECT_NEAR(arc_start.y, 0.0, 0.001);
  EXPECT_NEAR(arc_start.z, 0.0, 0.001);

  CW::Point3D arc_end = arc.end_point();
  EXPECT_NEAR(arc_end.x, -10.0, 0.001);
  EXPECT_NEAR(arc_end.y, 0.0, 0.001);
  EXPECT_NEAR(arc_end.z, 0.0, 0.001);

  CW::Vector3D arc_normal = arc.normal();
  EXPECT_NEAR(arc_normal.x, 0.0, 0.001);
  EXPECT_NEAR(arc_normal.y, 0.0, 0.001);
  EXPECT_NEAR(arc_normal.z, 1.0, 0.001);
}

// Test creating a full circle (start == end)
TEST(ArcCurveTest, CreateFullCircle)
{
  CW::Point3D center(5, 5, 0);
  CW::Point3D start(15, 5, 0);
  CW::Point3D end(15, 5, 0); // same as start = full circle
  CW::Vector3D normal(0, 0, 1);
  size_t num_edges = 24;

  CW::ArcCurve arc(center, start, end, normal, num_edges);
  EXPECT_FALSE(!arc);
  EXPECT_TRUE(arc.is_full_circle());
  EXPECT_NEAR(arc.radius(), 10.0, 0.001);
}

// Test x_axis and y_axis
TEST(ArcCurveTest, Axes)
{
  CW::Point3D center(0, 0, 0);
  CW::Point3D start(10, 0, 0);
  CW::Point3D end(-10, 0, 0);
  CW::Vector3D normal(0, 0, 1);

  CW::ArcCurve arc(center, start, end, normal, 12);
  EXPECT_FALSE(!arc);

  CW::Vector3D x = arc.x_axis();
  CW::Vector3D y = arc.y_axis();
  // x_axis and y_axis should be perpendicular
  double dot = x.x * y.x + x.y * y.y + x.z * y.z;
  EXPECT_NEAR(dot, 0.0, 0.001);
}

// Test start and end angles
TEST(ArcCurveTest, Angles)
{
  CW::Point3D center(0, 0, 0);
  CW::Point3D start(10, 0, 0);
  CW::Point3D end(-10, 0, 0);
  CW::Vector3D normal(0, 0, 1);

  CW::ArcCurve arc(center, start, end, normal, 12);
  EXPECT_FALSE(!arc);

  double start_angle = arc.start_angle();
  double end_angle = arc.end_angle();
  // Angles should differ for a non-full-circle arc
  EXPECT_NE(start_angle, end_angle);
}

// Test null throws
TEST(ArcCurveTest, NullThrows)
{
  CW::ArcCurve arc;
  EXPECT_THROW(arc.radius(), std::logic_error);
  EXPECT_THROW(arc.start_point(), std::logic_error);
  EXPECT_THROW(arc.end_point(), std::logic_error);
  EXPECT_THROW(arc.center(), std::logic_error);
  EXPECT_THROW(arc.normal(), std::logic_error);
  EXPECT_THROW(arc.x_axis(), std::logic_error);
  EXPECT_THROW(arc.y_axis(), std::logic_error);
  EXPECT_THROW(arc.start_angle(), std::logic_error);
  EXPECT_THROW(arc.end_angle(), std::logic_error);
  EXPECT_THROW(arc.is_full_circle(), std::logic_error);
}

// Test adding arc curves to entities and retrieving them
TEST(ArcCurveTest, AddToEntitiesAndRetrieve)
{
  CW::Model model;
  CW::Entities entities = model.entities();

  // Initially no arc curves
  std::vector<CW::ArcCurve> arcs = entities.arc_curves();
  EXPECT_EQ(arcs.size(), 0u);

  // Create arc curves directly in the vector
  CW::Point3D center(0, 0, 0);
  CW::Point3D start(10, 0, 0);
  CW::Point3D end(0, 10, 0);
  CW::Vector3D normal(0, 0, 1);

  std::vector<CW::ArcCurve> arcs_to_add;
  arcs_to_add.emplace_back(center, start, end, normal, 8);
  entities.add_arc_curves(arcs_to_add);

  // Now there should be arc curves
  std::vector<CW::ArcCurve> retrieved = entities.arc_curves();
  EXPECT_GE(retrieved.size(), 1u);
}

// Test reading arc curves from a loaded model
TEST_F(ModelLoad, ArcCurvesFromModel)
{
  ASSERT_FALSE(!(*m_model));
  CW::Entities entities = m_model->entities();
  std::vector<CW::ArcCurve> arcs = entities.arc_curves();
  // The test model may or may not contain arc curves.
  // If it does, verify properties.
  if (arcs.size() > 0) {
    CW::ArcCurve& first = arcs[0];
    EXPECT_GT(first.radius(), 0.0);
    CW::Point3D c = first.center();
    CW::Vector3D n = first.normal();
    // Normal should be non-zero
    double len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    EXPECT_GT(len, 0.0);
  }
}


// Test copy() creates a detached copy with the same geometry
TEST(ArcCurveTest, Copy)
{
  CW::Point3D center(0, 0, 0);
  CW::Point3D start(10, 0, 0);
  CW::Point3D end(-10, 0, 0);
  CW::Vector3D normal(0, 0, 1);
  size_t num_edges = 16;

  CW::ArcCurve arc(center, start, end, normal, num_edges);
  CW::ArcCurve arc_copy = arc.copy();

  EXPECT_FALSE(!arc_copy);
  EXPECT_NEAR(arc_copy.radius(), arc.radius(), 0.001);
  EXPECT_NEAR(arc_copy.center().x, center.x, 0.001);
  EXPECT_NEAR(arc_copy.center().y, center.y, 0.001);
  EXPECT_NEAR(arc_copy.center().z, center.z, 0.001);
  EXPECT_NEAR(arc_copy.start_point().x, start.x, 0.001);
  EXPECT_NEAR(arc_copy.start_point().y, start.y, 0.001);
  EXPECT_NEAR(arc_copy.end_point().x, end.x, 0.001);
  EXPECT_NEAR(arc_copy.end_point().y, end.y, 0.001);
  EXPECT_NEAR(arc_copy.normal().z, normal.z, 0.001);
  EXPECT_EQ(arc_copy.num_edges(), num_edges);
}

// Test num_edges()
TEST(ArcCurveTest, NumEdges)
{
  CW::Point3D center(0, 0, 0);
  CW::Point3D start(10, 0, 0);
  CW::Point3D end(-10, 0, 0);
  CW::Vector3D normal(0, 0, 1);

  CW::ArcCurve arc8(center, start, end, normal, 8);
  EXPECT_EQ(arc8.num_edges(), 8u);

  CW::ArcCurve arc24(center, start, end, normal, 24);
  EXPECT_EQ(arc24.num_edges(), 24u);
}

// Test copy() on null throws
TEST(ArcCurveTest, CopyNullThrows)
{
  CW::ArcCurve arc;
  EXPECT_THROW(arc.copy(), std::logic_error);
  EXPECT_THROW(arc.num_edges(), std::logic_error);
}


/**********************
 * ArcCurve Model Copy Tests
 **********************/

class ArcCurveModelTest : public ModelLoad {};

TEST_F(ArcCurveModelTest, CopyArcCurves) {
  using namespace CW;
  Entities src_entities = m_model->entities();
  std::vector<ArcCurve> source_arcs = src_entities.arc_curves();

  if (source_arcs.size() == 0) {
    FAIL() << "No arc curves in test model - add arc curves to 'box and box.skp' to enable full test";
    return;
  }

  // Use copy() to create detached copies for the destination model
  std::vector<ArcCurve> new_arcs;
  for (const auto& src : source_arcs) {
    new_arcs.push_back(src.copy());
  }

  CW::Entities dest_entities = m_model_copy->entities();
  dest_entities.add_arc_curves(new_arcs);

  // Read back from destination and verify count matches
  std::vector<ArcCurve> copied_arcs = dest_entities.arc_curves();
  ASSERT_EQ(copied_arcs.size(), source_arcs.size());

  SaveModel("ArcCurves");
}

} // namespace CW::Tests
