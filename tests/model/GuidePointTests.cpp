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
#include "SUAPI-CppWrapper/model/GuidePoint.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {


class GuidePointTest : public testing::Test {
 protected:
  GuidePointTest() {
    CW::initialize();
  }
  ~GuidePointTest() override {
    CW::terminate();
  }
};


TEST_F(GuidePointTest, CreateDefaultGuidePoint) {
  CW::GuidePoint gp;
  EXPECT_TRUE(!gp);
}


TEST_F(GuidePointTest, CreateFromPosition) {
  CW::Point3D pos(10.0, 20.0, 30.0);
  CW::GuidePoint gp(pos);
  EXPECT_FALSE(!gp);
  EXPECT_FALSE(gp.attached());
}


TEST_F(GuidePointTest, GetPosition) {
  CW::Point3D pos(10.0, 20.0, 30.0);
  CW::GuidePoint gp(pos);

  CW::Point3D got_pos = gp.position();
  EXPECT_DOUBLE_EQ(got_pos.x, 10.0);
  EXPECT_DOUBLE_EQ(got_pos.y, 20.0);
  EXPECT_DOUBLE_EQ(got_pos.z, 30.0);
}


TEST_F(GuidePointTest, GetFromPosition) {
  CW::Point3D pos(10.0, 20.0, 30.0);
  CW::GuidePoint gp(pos);

  // For guide points created with SUGuidePointCreate, the anchor is the origin
  CW::Point3D from_pos = gp.from_position();
  EXPECT_DOUBLE_EQ(from_pos.x, 0.0);
  EXPECT_DOUBLE_EQ(from_pos.y, 0.0);
  EXPECT_DOUBLE_EQ(from_pos.z, 0.0);
}


TEST_F(GuidePointTest, DisplayAsLine) {
  CW::Point3D pos(10.0, 20.0, 30.0);
  CW::GuidePoint gp(pos);

  // Newly created guide points should have a display_as_line value
  bool as_line = gp.display_as_line();
  // Just check that the call succeeds without error
  (void)as_line;
  SUCCEED();
}


TEST_F(GuidePointTest, CopyConstructor) {
  CW::Point3D pos(5.0, 10.0, 15.0);
  CW::GuidePoint gp(pos);

  CW::GuidePoint copy(gp);
  EXPECT_FALSE(!copy);

  CW::Point3D copy_pos = copy.position();
  EXPECT_DOUBLE_EQ(copy_pos.x, 5.0);
  EXPECT_DOUBLE_EQ(copy_pos.y, 10.0);
  EXPECT_DOUBLE_EQ(copy_pos.z, 15.0);

  // Should be different references
  EXPECT_NE(gp.ref().ptr, copy.ref().ptr);
}


TEST_F(GuidePointTest, CopyAssignment) {
  CW::Point3D pos1(1.0, 2.0, 3.0);
  CW::GuidePoint gp1(pos1);

  CW::Point3D pos2(4.0, 5.0, 6.0);
  CW::GuidePoint gp2(pos2);

  gp2 = gp1;
  CW::Point3D got_pos = gp2.position();
  EXPECT_DOUBLE_EQ(got_pos.x, 1.0);
  EXPECT_DOUBLE_EQ(got_pos.y, 2.0);
  EXPECT_DOUBLE_EQ(got_pos.z, 3.0);
}


TEST_F(GuidePointTest, NullGuidePointThrows) {
  CW::GuidePoint gp;
  EXPECT_THROW(gp.position(), std::logic_error);
  EXPECT_THROW(gp.from_position(), std::logic_error);
  EXPECT_THROW(gp.display_as_line(), std::logic_error);
}


TEST_F(GuidePointTest, OriginGuidePoint) {
  CW::Point3D pos(0.0, 0.0, 0.0);
  CW::GuidePoint gp(pos);
  EXPECT_FALSE(!gp);

  CW::Point3D got_pos = gp.position();
  EXPECT_DOUBLE_EQ(got_pos.x, 0.0);
  EXPECT_DOUBLE_EQ(got_pos.y, 0.0);
  EXPECT_DOUBLE_EQ(got_pos.z, 0.0);
}


// Model-based tests: copy guide points from source model to destination model
class GuidePointModelTest : public ModelLoad {};


TEST_F(GuidePointModelTest, CopyGuidePoints) {
  // Read guide points from the source model
  CW::Entities src_entities = m_model->entities();
  std::vector<CW::GuidePoint> source_points = src_entities.guide_points();
  ASSERT_GT(source_points.size(), 0u);

  // Copy each guide point using copy() and add to the destination model
  std::vector<CW::GuidePoint> new_points;
  for (const auto& sp : source_points) {
    new_points.push_back(sp.copy());
  }
  CW::Entities dest_entities = m_model_copy->entities();
  dest_entities.add_guide_points(new_points);

  // Read back from destination and verify positions match
  std::vector<CW::GuidePoint> copied_points = dest_entities.guide_points();
  ASSERT_EQ(copied_points.size(), source_points.size());

  for (size_t i = 0; i < source_points.size(); ++i) {
    CW::Point3D src_pos = source_points[i].position();
    CW::Point3D dest_pos = copied_points[i].position();
    EXPECT_DOUBLE_EQ(src_pos.x, dest_pos.x);
    EXPECT_DOUBLE_EQ(src_pos.y, dest_pos.y);
    EXPECT_DOUBLE_EQ(src_pos.z, dest_pos.z);
  }

  SaveModel("GuidePoints");
}


}
