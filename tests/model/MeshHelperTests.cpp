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
#include "SUAPI-CppWrapper/model/MeshHelper.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"

namespace CW::Tests {

/**********************
 * MeshHelper Tests
 **********************/

// Test default constructor creates a null mesh helper
TEST(MeshHelperTest, DefaultConstructor)
{
  CW::MeshHelper mesh;
  EXPECT_TRUE(!mesh);
}

// Test null throws
TEST(MeshHelperTest, NullThrows)
{
  CW::MeshHelper mesh;
  EXPECT_THROW(mesh.num_triangles(), std::logic_error);
  EXPECT_THROW(mesh.num_vertices(), std::logic_error);
  EXPECT_THROW(mesh.vertex_indices(), std::logic_error);
  EXPECT_THROW(mesh.vertices(), std::logic_error);
  EXPECT_THROW(mesh.front_stq_coords(), std::logic_error);
  EXPECT_THROW(mesh.back_stq_coords(), std::logic_error);
  EXPECT_THROW(mesh.normals(), std::logic_error);
}

// Test MeshHelper from a face in a loaded model
TEST_F(ModelLoad, MeshHelperFromFace)
{
  ASSERT_FALSE(!(*m_model));
  CW::Entities entities = m_model->entities();
  std::vector<CW::Face> faces = entities.faces();
  ASSERT_GT(faces.size(), 0u);

  CW::MeshHelper mesh(faces[0]);
  EXPECT_FALSE(!mesh);

  // A face should tessellate into at least 1 triangle
  size_t num_tris = mesh.num_triangles();
  EXPECT_GT(num_tris, 0u);

  size_t num_verts = mesh.num_vertices();
  EXPECT_GT(num_verts, 0u);

  // Vertex indices: should be 3 * num_triangles
  std::vector<size_t> indices = mesh.vertex_indices();
  EXPECT_EQ(indices.size(), num_tris * 3);

  // Vertices
  std::vector<CW::Point3D> verts = mesh.vertices();
  EXPECT_EQ(verts.size(), num_verts);

  // Normals
  std::vector<CW::Vector3D> norms = mesh.normals();
  EXPECT_EQ(norms.size(), num_verts);

  // STQ coordinates
  std::vector<CW::Point3D> front_stq = mesh.front_stq_coords();
  EXPECT_EQ(front_stq.size(), num_verts);

  std::vector<CW::Point3D> back_stq = mesh.back_stq_coords();
  EXPECT_EQ(back_stq.size(), num_verts);
}

// Test MeshHelper with a simple rectangular face
TEST(MeshHelperTest, SimpleRectangleFace)
{
  // Create a simple rectangular face: 4 corners
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);
  ASSERT_FALSE(!face);

  CW::MeshHelper mesh(face);
  EXPECT_FALSE(!mesh);

  // A rectangle should tessellate into 2 triangles
  EXPECT_EQ(mesh.num_triangles(), 2u);
  EXPECT_GE(mesh.num_vertices(), 4u);

  // All vertices should lie on z=0 plane
  std::vector<CW::Point3D> verts = mesh.vertices();
  for (const auto& v : verts) {
    EXPECT_NEAR(v.z, 0.0, 0.001);
  }

  // All normals should point in Z direction
  std::vector<CW::Vector3D> norms = mesh.normals();
  for (const auto& n : norms) {
    EXPECT_NEAR(std::abs(n.z), 1.0, 0.001);
  }
}

// Test MeshHelper move semantics
TEST(MeshHelperTest, MoveConstructor)
{
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);

  CW::MeshHelper mesh1(face);
  ASSERT_FALSE(!mesh1);
  size_t num_tris = mesh1.num_triangles();

  CW::MeshHelper mesh2(std::move(mesh1));
  EXPECT_TRUE(!mesh1);  // moved-from should be null
  EXPECT_FALSE(!mesh2);
  EXPECT_EQ(mesh2.num_triangles(), num_tris);
}

// Test MeshHelper move assignment
TEST(MeshHelperTest, MoveAssignment)
{
  std::vector<CW::Point3D> points = {
    CW::Point3D(0, 0, 0),
    CW::Point3D(10, 0, 0),
    CW::Point3D(10, 10, 0),
    CW::Point3D(0, 10, 0)
  };
  CW::Face face(points);

  CW::MeshHelper mesh1(face);
  ASSERT_FALSE(!mesh1);
  size_t num_tris = mesh1.num_triangles();

  CW::MeshHelper mesh2;
  EXPECT_TRUE(!mesh2);

  mesh2 = std::move(mesh1);
  EXPECT_TRUE(!mesh1);  // moved-from should be null
  EXPECT_FALSE(!mesh2);
  EXPECT_EQ(mesh2.num_triangles(), num_tris);
}

} // namespace CW::Tests
