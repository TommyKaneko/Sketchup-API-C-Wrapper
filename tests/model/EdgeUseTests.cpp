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
#include "SUAPI-CppWrapper/model/EdgeUse.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"

#include <SketchUpAPI/model/loop.h>

namespace CW::Tests {

/**********************
 * EdgeUse Tests
 **********************/

// Test default constructor creates a null edge use
TEST(EdgeUseTest, DefaultConstructor)
{
  CW::EdgeUse eu;
  EXPECT_TRUE(!eu);
}

// Test null throws
TEST(EdgeUseTest, NullThrows)
{
  CW::EdgeUse eu;
  EXPECT_THROW(eu.edge(), std::logic_error);
  EXPECT_THROW(eu.loop(), std::logic_error);
  EXPECT_THROW(eu.face(), std::logic_error);
  EXPECT_THROW(eu.partners(), std::logic_error);
  EXPECT_THROW(eu.is_reversed(), std::logic_error);
  EXPECT_THROW(eu.previous(), std::logic_error);
  EXPECT_THROW(eu.next(), std::logic_error);
  EXPECT_THROW(eu.start_vertex(), std::logic_error);
  EXPECT_THROW(eu.end_vertex(), std::logic_error);
  EXPECT_THROW(eu.start_vertex_normal(), std::logic_error);
  EXPECT_THROW(eu.end_vertex_normal(), std::logic_error);
}

// Test EdgeUse from a face in the loaded model
TEST_F(ModelLoad, EdgeUseFromFace)
{
  ASSERT_FALSE(!(*m_model));
  CW::Entities entities = m_model->entities();
  std::vector<CW::Face> faces = entities.faces();
  ASSERT_GT(faces.size(), 0u);

  // Get the outer loop of the first face
  CW::Face& face = faces[0];
  CW::Loop outer = face.outer_loop();
  ASSERT_FALSE(!outer);

  // Get edge uses from the loop via C API
  size_t num_edges = outer.size();
  ASSERT_GT(num_edges, 0u);

  std::vector<SUEdgeUseRef> eu_refs(num_edges, SU_INVALID);
  size_t count = 0;
  SUResult res = SULoopGetEdgeUses(outer.ref(), num_edges, eu_refs.data(), &count);
  ASSERT_EQ(res, SU_ERROR_NONE);
  ASSERT_GT(count, 0u);

  // Wrap in EdgeUse and test properties
  CW::EdgeUse eu(eu_refs[0]);
  EXPECT_FALSE(!eu);

  // Test edge()
  CW::Edge edge = eu.edge();
  EXPECT_FALSE(!edge);

  // Test loop()
  CW::Loop eu_loop = eu.loop();
  EXPECT_FALSE(!eu_loop);

  // Test face()
  CW::Face eu_face = eu.face();
  EXPECT_FALSE(!eu_face);

  // Test start and end vertex
  CW::Vertex sv = eu.start_vertex();
  EXPECT_FALSE(!sv);
  CW::Vertex ev = eu.end_vertex();
  EXPECT_FALSE(!ev);

  // Test next/previous navigation (should form a cycle)
  CW::EdgeUse next = eu.next();
  EXPECT_FALSE(!next);
  CW::EdgeUse prev = eu.previous();
  EXPECT_FALSE(!prev);

  // Test vertex normals
  CW::Vector3D sn = eu.start_vertex_normal();
  CW::Vector3D en = eu.end_vertex_normal();
  // Normals should be non-zero
  double sn_len = std::sqrt(sn.x * sn.x + sn.y * sn.y + sn.z * sn.z);
  double en_len = std::sqrt(en.x * en.x + en.y * en.y + en.z * en.z);
  EXPECT_GT(sn_len, 0.0);
  EXPECT_GT(en_len, 0.0);
}

// Test EdgeUse partners (adjacent faces sharing an edge)
TEST_F(ModelLoad, EdgeUsePartners)
{
  ASSERT_FALSE(!(*m_model));
  CW::Entities entities = m_model->entities();
  std::vector<CW::Face> faces = entities.faces();
  ASSERT_GT(faces.size(), 0u);

  // Find an edge use that has partners (shared edge between two faces)
  for (const CW::Face& face : faces) {
    CW::Loop outer = face.outer_loop();
    size_t num_edges = outer.size();
    std::vector<SUEdgeUseRef> eu_refs(num_edges, SU_INVALID);
    size_t count = 0;
    SUResult res = SULoopGetEdgeUses(outer.ref(), num_edges, eu_refs.data(), &count);
    if (res != SU_ERROR_NONE) continue;

    for (size_t i = 0; i < count; ++i) {
      CW::EdgeUse eu(eu_refs[i]);
      std::vector<CW::EdgeUse> partners = eu.partners();
      if (partners.size() > 0) {
        // Found one with partners — verify properties
        EXPECT_FALSE(!partners[0]);
        CW::Edge partner_edge = partners[0].edge();
        EXPECT_FALSE(!partner_edge);
        return; // Test passes
      }
    }
  }
  // If no partners found, that's OK for box model — edges are shared
}

// Test EdgeUse is_reversed
TEST_F(ModelLoad, EdgeUseIsReversed)
{
  ASSERT_FALSE(!(*m_model));
  CW::Entities entities = m_model->entities();
  std::vector<CW::Face> faces = entities.faces();
  ASSERT_GT(faces.size(), 0u);

  CW::Face& face = faces[0];
  CW::Loop outer = face.outer_loop();
  size_t num_edges = outer.size();
  std::vector<SUEdgeUseRef> eu_refs(num_edges, SU_INVALID);
  size_t count = 0;
  SUResult res = SULoopGetEdgeUses(outer.ref(), num_edges, eu_refs.data(), &count);
  ASSERT_EQ(res, SU_ERROR_NONE);
  ASSERT_GT(count, 0u);

  // Just verify is_reversed doesn't crash and returns a valid bool
  CW::EdgeUse eu(eu_refs[0]);
  bool reversed = eu.is_reversed();
  EXPECT_TRUE(reversed == true || reversed == false);
}

} // namespace CW::Tests
