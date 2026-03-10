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
#include "SUAPI-CppWrapper/model/SectionPlane.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {


class SectionPlaneTest : public testing::Test {
 protected:
  SectionPlaneTest() {
    CW::initialize();
  }
  ~SectionPlaneTest() override {
    CW::terminate();
  }
};


TEST_F(SectionPlaneTest, CreateDefaultSectionPlane) {
  CW::SectionPlane sp;
  EXPECT_TRUE(!sp);
}


TEST_F(SectionPlaneTest, CreateNewSectionPlane) {
  CW::SectionPlane sp;
  EXPECT_TRUE(!sp);
}


TEST_F(SectionPlaneTest, CopyConstructor) {
  CW::SectionPlane sp;
  CW::SectionPlane copy(sp);
  EXPECT_TRUE(!copy);
}


TEST_F(SectionPlaneTest, NullSectionPlaneThrows) {
  CW::SectionPlane sp;
  EXPECT_THROW(sp.plane(), std::logic_error);
  EXPECT_THROW(sp.active(), std::logic_error);
  EXPECT_THROW(sp.name(), std::logic_error);
  EXPECT_THROW(sp.symbol(), std::logic_error);
}


// Model-based tests: copy section planes from source model to destination model
class SectionPlaneModelTest : public ModelLoad {};


TEST_F(SectionPlaneModelTest, CopySectionPlanes) {
  // Read section planes from the source model
  CW::Entities src_entities = m_model->entities();
  std::vector<CW::SectionPlane> source_planes = src_entities.section_planes();
  ASSERT_GT(source_planes.size(), 0u);

  // Copy each section plane using copy() and add to the destination model
  std::vector<CW::SectionPlane> new_planes;
  for (const auto& sp : source_planes) {
    new_planes.push_back(sp.copy());
  }
  CW::Entities dest_entities = m_model_copy->entities();
  dest_entities.add_section_planes(new_planes);

  // Name and symbol must be set after adding to entities
  for (size_t i = 0; i < source_planes.size(); ++i) {
    new_planes[i].name(source_planes[i].name());
    new_planes[i].symbol(source_planes[i].symbol());
  }

  // Read back from destination and verify properties match
  std::vector<CW::SectionPlane> copied_planes = dest_entities.section_planes();
  ASSERT_EQ(copied_planes.size(), source_planes.size());

  for (size_t i = 0; i < source_planes.size(); ++i) {
    CW::Plane3D src_plane = source_planes[i].plane();
    CW::Plane3D dest_plane = copied_planes[i].plane();
    EXPECT_DOUBLE_EQ(src_plane.a, dest_plane.a);
    EXPECT_DOUBLE_EQ(src_plane.b, dest_plane.b);
    EXPECT_DOUBLE_EQ(src_plane.c, dest_plane.c);
    EXPECT_DOUBLE_EQ(src_plane.d, dest_plane.d);

    EXPECT_EQ(std::string(source_planes[i].name()), std::string(copied_planes[i].name()));
    EXPECT_EQ(std::string(source_planes[i].symbol()), std::string(copied_planes[i].symbol()));
  }

  SaveModel("SectionPlanes");
}


}
