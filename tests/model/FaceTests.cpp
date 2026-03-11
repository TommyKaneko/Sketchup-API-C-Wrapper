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
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"

namespace CW::Tests {

/**********************
 * Face Tests
 **********************/

TEST(FaceTest, DefaultConstructor) {
  CW::Face face;
  EXPECT_TRUE(!face);
}

TEST(FaceTest, CreateFromPoints) {
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);
  EXPECT_FALSE(!face);
  EXPECT_GT(face.area(), 0.0);
}

TEST(FaceTest, Vertices) {
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);
  ASSERT_FALSE(!face);

  std::vector<CW::Vertex> verts = face.vertices();
  EXPECT_EQ(verts.size(), 4u);
}

TEST(FaceTest, Normal) {
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);
  ASSERT_FALSE(!face);

  CW::Vector3D normal = face.normal();
  // For a face in the XY plane, normal should be along Z
  EXPECT_NEAR(std::abs(normal.z), 1.0, 0.001);
}

TEST(FaceTest, OuterLoop) {
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);
  ASSERT_FALSE(!face);

  CW::Loop loop = face.outer_loop();
  EXPECT_FALSE(!loop);
}

TEST(FaceTest, Copy) {
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);
  ASSERT_FALSE(!face);

  CW::Face face_copy = face.copy();
  EXPECT_FALSE(!face_copy);
  EXPECT_FALSE(face_copy.attached());
  EXPECT_NE(face_copy.ref().ptr, face.ref().ptr);
  EXPECT_NEAR(face_copy.area(), face.area(), 0.001);

  // Normals should match
  CW::Vector3D n1 = face.normal();
  CW::Vector3D n2 = face_copy.normal();
  EXPECT_NEAR(n1.x, n2.x, 0.001);
  EXPECT_NEAR(n1.y, n2.y, 0.001);
  EXPECT_NEAR(n1.z, n2.z, 0.001);
}

TEST(FaceTest, CopyNullThrows) {
  CW::Face face;
  EXPECT_THROW(face.copy(), std::logic_error);
}

TEST(FaceTest, NullThrows) {
  CW::Face face;
  EXPECT_THROW(face.area(), std::logic_error);
  EXPECT_THROW(face.normal(), std::logic_error);
  EXPECT_THROW(face.outer_loop(), std::logic_error);
  EXPECT_THROW(face.vertices(), std::logic_error);
}


/**********************
 * Face Model Copy Tests
 **********************/

class FaceModelTest : public ModelLoad {};

TEST_F(FaceModelTest, CopyFaces) {
  using namespace CW;
  Entities src_entities = m_model->entities();
  std::vector<Face> source_faces = src_entities.faces();
  ASSERT_GT(source_faces.size(), 0u);

  // Create detached copies of each face
  std::vector<Face> new_faces;
  for (const auto& src : source_faces) {
    new_faces.push_back(src.copy());
  }

  Entities dest_entities = m_model_copy->entities();
  dest_entities.add_faces(new_faces);

  // Read back from destination and verify count matches
  std::vector<Face> copied_faces = dest_entities.faces();
  ASSERT_EQ(copied_faces.size(), source_faces.size());

  // Verify each copied face has positive area
  for (size_t i = 0; i < copied_faces.size(); ++i) {
    EXPECT_GT(copied_faces[i].area(), 0.0);
    // Note: inner loops are not copied, so areas may differ for faces with holes.
    // Copied area should be >= source area (no holes means larger or equal area).
    EXPECT_GE(copied_faces[i].area(), source_faces[i].area() - 0.001);
  }

  SaveModel("Faces");
}

} // namespace CW::Tests
