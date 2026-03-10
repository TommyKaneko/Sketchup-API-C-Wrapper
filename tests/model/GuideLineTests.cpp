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
#include "SUAPI-CppWrapper/model/GuideLine.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {


class GuideLineTest : public testing::Test {
 protected:
  GuideLineTest() {
    CW::initialize();
  }
  ~GuideLineTest() override {
    CW::terminate();
  }
};


TEST_F(GuideLineTest, CreateDefaultGuideLine) {
  CW::GuideLine gl;
  EXPECT_TRUE(!gl);
}


TEST_F(GuideLineTest, CreateFiniteGuideLine) {
  CW::Point3D start(0.0, 0.0, 0.0);
  CW::Point3D end(10.0, 0.0, 0.0);
  CW::GuideLine gl = CW::GuideLine::create_finite(start, end);
  EXPECT_FALSE(!gl);
  EXPECT_FALSE(gl.attached());
}


TEST_F(GuideLineTest, CreateInfiniteGuideLine) {
  CW::Point3D point(0.0, 0.0, 0.0);
  CW::Vector3D dir(1.0, 0.0, 0.0);
  CW::GuideLine gl = CW::GuideLine::create_infinite(point, dir);
  EXPECT_FALSE(!gl);
  EXPECT_TRUE(gl.is_infinite());
}


TEST_F(GuideLineTest, FiniteIsNotInfinite) {
  CW::Point3D start(0.0, 0.0, 0.0);
  CW::Point3D end(10.0, 0.0, 0.0);
  CW::GuideLine gl = CW::GuideLine::create_finite(start, end);
  EXPECT_FALSE(gl.is_infinite());
}


TEST_F(GuideLineTest, FiniteStartAndEnd) {
  CW::Point3D start(1.0, 2.0, 3.0);
  CW::Point3D end(4.0, 5.0, 6.0);
  CW::GuideLine gl = CW::GuideLine::create_finite(start, end);

  CW::Point3D got_start = gl.start();
  EXPECT_DOUBLE_EQ(got_start.x, 1.0);
  EXPECT_DOUBLE_EQ(got_start.y, 2.0);
  EXPECT_DOUBLE_EQ(got_start.z, 3.0);

  CW::Point3D got_end = gl.end();
  EXPECT_DOUBLE_EQ(got_end.x, 4.0);
  EXPECT_DOUBLE_EQ(got_end.y, 5.0);
  EXPECT_DOUBLE_EQ(got_end.z, 6.0);
}


TEST_F(GuideLineTest, FiniteDirection) {
  CW::Point3D start(0.0, 0.0, 0.0);
  CW::Point3D end(10.0, 0.0, 0.0);
  CW::GuideLine gl = CW::GuideLine::create_finite(start, end);

  CW::Vector3D dir = gl.direction();
  // Direction should be along X axis, length 10
  EXPECT_DOUBLE_EQ(dir.x, 10.0);
  EXPECT_DOUBLE_EQ(dir.y, 0.0);
  EXPECT_DOUBLE_EQ(dir.z, 0.0);
}


TEST_F(GuideLineTest, InfiniteStartAndDirection) {
  CW::Point3D point(5.0, 5.0, 5.0);
  CW::Vector3D dir(0.0, 0.0, 1.0);
  CW::GuideLine gl = CW::GuideLine::create_infinite(point, dir);

  CW::Point3D got_start = gl.start();
  EXPECT_DOUBLE_EQ(got_start.x, 5.0);
  EXPECT_DOUBLE_EQ(got_start.y, 5.0);
  EXPECT_DOUBLE_EQ(got_start.z, 5.0);

  CW::Vector3D got_dir = gl.direction();
  EXPECT_DOUBLE_EQ(got_dir.x, 0.0);
  EXPECT_DOUBLE_EQ(got_dir.y, 0.0);
  EXPECT_DOUBLE_EQ(got_dir.z, 1.0);
}


TEST_F(GuideLineTest, InfiniteEndThrows) {
  CW::Point3D point(0.0, 0.0, 0.0);
  CW::Vector3D dir(1.0, 0.0, 0.0);
  CW::GuideLine gl = CW::GuideLine::create_infinite(point, dir);
  EXPECT_THROW(gl.end(), std::logic_error);
}


TEST_F(GuideLineTest, GetData) {
  CW::Point3D start(1.0, 2.0, 3.0);
  CW::Point3D end(4.0, 5.0, 6.0);
  CW::GuideLine gl = CW::GuideLine::create_finite(start, end);

  CW::Point3D out_start;
  CW::Vector3D out_dir;
  bool is_inf = gl.get_data(out_start, out_dir);
  EXPECT_FALSE(is_inf);
  EXPECT_DOUBLE_EQ(out_start.x, 1.0);
  EXPECT_DOUBLE_EQ(out_start.y, 2.0);
  EXPECT_DOUBLE_EQ(out_start.z, 3.0);
}


TEST_F(GuideLineTest, CopyConstructor) {
  CW::Point3D start(0.0, 0.0, 0.0);
  CW::Point3D end(10.0, 0.0, 0.0);
  CW::GuideLine gl = CW::GuideLine::create_finite(start, end);

  CW::GuideLine copy(gl);
  EXPECT_FALSE(!copy);
  EXPECT_FALSE(copy.is_infinite());

  CW::Point3D copy_start = copy.start();
  EXPECT_DOUBLE_EQ(copy_start.x, 0.0);
}


TEST_F(GuideLineTest, ConstructFromTwoPoints) {
  CW::Point3D start(0.0, 0.0, 0.0);
  CW::Point3D end(5.0, 5.0, 0.0);
  CW::GuideLine gl(start, end);
  EXPECT_FALSE(!gl);
  EXPECT_FALSE(gl.is_infinite());
}


TEST_F(GuideLineTest, NullGuideLineThrows) {
  CW::GuideLine gl;
  EXPECT_THROW(gl.start(), std::logic_error);
  EXPECT_THROW(gl.direction(), std::logic_error);
  EXPECT_THROW(gl.is_infinite(), std::logic_error);
}


// Model-based tests: copy guide lines from source model to destination model
class GuideLineModelTest : public ModelLoad {};


TEST_F(GuideLineModelTest, CopyGuideLines) {
  // Read guide lines from the source model
  CW::Entities src_entities = m_model->entities();
  std::vector<CW::GuideLine> source_lines = src_entities.guide_lines();
  ASSERT_GT(source_lines.size(), 0u);

  // Copy each guide line using copy() and add to the destination model
  std::vector<CW::GuideLine> new_lines;
  for (const auto& sl : source_lines) {
    new_lines.push_back(sl.copy());
  }
  CW::Entities dest_entities = m_model_copy->entities();
  dest_entities.add_guide_lines(new_lines);

  // Read back from destination and verify properties match
  std::vector<CW::GuideLine> copied_lines = dest_entities.guide_lines();
  ASSERT_EQ(copied_lines.size(), source_lines.size());

  for (size_t i = 0; i < source_lines.size(); ++i) {
    EXPECT_EQ(source_lines[i].is_infinite(), copied_lines[i].is_infinite());

    CW::Point3D src_start = source_lines[i].start();
    CW::Point3D dest_start = copied_lines[i].start();
    EXPECT_DOUBLE_EQ(src_start.x, dest_start.x);
    EXPECT_DOUBLE_EQ(src_start.y, dest_start.y);
    EXPECT_DOUBLE_EQ(src_start.z, dest_start.z);

    CW::Vector3D src_dir = source_lines[i].direction();
    CW::Vector3D dest_dir = copied_lines[i].direction();
    EXPECT_NEAR(src_dir.x, dest_dir.x, 1e-6);
    EXPECT_NEAR(src_dir.y, dest_dir.y, 1e-6);
    EXPECT_NEAR(src_dir.z, dest_dir.z, 1e-6);

    if (!source_lines[i].is_infinite()) {
      CW::Point3D src_end = source_lines[i].end();
      CW::Point3D dest_end = copied_lines[i].end();
      EXPECT_DOUBLE_EQ(src_end.x, dest_end.x);
      EXPECT_DOUBLE_EQ(src_end.y, dest_end.y);
      EXPECT_DOUBLE_EQ(src_end.z, dest_end.z);
    }
  }

  SaveModel("GuideLines");
}


}
