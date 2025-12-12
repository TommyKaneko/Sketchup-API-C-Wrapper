#include "gtest/gtest.h"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/GeometryInput.hpp"

namespace CW::Tests {

/**********************
 * Edge Tests
 * Tests Edge class
 **********************/

// Edge Referencing Tests that references of an edge point to the same object
TEST_F(ModelLoad, EdgeReferencing)
{
  using namespace CW;
  ASSERT_FALSE(!m_model);

  // Get all edges from the loaded model (include non-stray edges)
  std::vector<Edge> edges = m_model->entities().edges(false);
  // The above operation should refer to the same edges
  size_t i = 0;
  for (const Edge& edge : edges) {
    if (i > 0) break; // only check one edge
    i++;
    CW::Edge edge_ref = edge;
    EXPECT_EQ(edge_ref.attached(), true);
    EXPECT_EQ(edge.attached(), true);
    EXPECT_EQ(edge_ref.ref().ptr, edge.ref().ptr);
  }

  // Now check copies and detached copies
  i = 0;
  for (const Edge& edge : edges) {
    if (i > 0) break; // only check one edge
    i++;
    // Create an explicit detached copy using the endpoints
    CW::Edge detached_copy(edge.copy());
    EXPECT_NE(detached_copy.ref().ptr, edge.ref().ptr); // must be a different native object
    EXPECT_EQ(edge.attached(), true);
    EXPECT_EQ(detached_copy.attached(), false);
    std::cerr << "\033[33m[     INFO ] " << "NOTE: Materials and Layers cannot be assigned to Edges that are not attached to the model. EdgeReferencing Test is expected to fail with equality of material and layer input and output. \033[0m" << std::endl;
    // std::string copied_layer_name = "undefined";
    // if (detached_copy.layer().is_valid()) {
    //   copied_layer_name = detached_copy.layer().name().std_string();
    // }
    // std::cerr << "[     INFO ] " << "original layer name: " << edge.layer().name().std_string() << " copied layer name: " << copied_layer_name << std::endl;
    // CW::Layer edge_layer = edge.layer();
    // std::cerr << "[     INFO ] " << "original layer name: " << edge_layer.name().std_string() << std::endl;
    // SUResult result = SUDrawingElementSetLayer(SUEdgeToDrawingElement(detached_copy.ref()), edge_layer.ref());
    // if (detached_copy.layer().is_valid()) {
    //   copied_layer_name = detached_copy.layer().name().std_string();
    // }
    // std::cerr << "[     INFO ] " << " copied layer name: " << copied_layer_name << std::endl;
    // Properties should match
    ASSERT_NO_FATAL_FAILURE(EdgesAreEqual(detached_copy, edge));

    // Test copy of the detached copy produces another independent object
    CW::Edge detached_copy_copy = detached_copy;
    EXPECT_NE(detached_copy_copy.ref().ptr, detached_copy.ref().ptr);
    EXPECT_EQ(detached_copy_copy.attached(), false);
    ASSERT_NO_FATAL_FAILURE(EdgesAreEqual(detached_copy_copy, detached_copy));
  }
}


// Edge Implicit Copying - adding edges from one model into a new model
TEST_F(ModelLoad, EdgeImplicitCopying)
{
  using namespace CW;
  ASSERT_FALSE(!m_model);

  std::vector<Edge> edges = m_model->entities().edges(false);
  // Add edges into the empty model copy via its Entities object
  CW::Entities dest_entities = m_model_copy->entities();
  dest_entities.add_edges(edges);

  std::vector<Edge> copied_edges = dest_entities.edges(false);
  EXPECT_EQ(edges.size(), copied_edges.size());
  std::cerr << "\033[33m[     INFO ] " << "NOTE: Materials and Layers cannot be assigned to Edges that are not attached to the model. EdgeImplicitCopying Test is expected to fail for equality of material and layer input and output. \033[0m" << std::endl;
  for (size_t i = 0; i < edges.size(); ++i) {
    if (i > 0) break; // only check one edge
    const Edge& original = edges[i];
    const Edge& copied = copied_edges[i];
    // Should be different native references
    EXPECT_NE(original.ref().ptr, copied.ref().ptr);
    EXPECT_TRUE(copied.attached());
    // Properties should be the same
    ASSERT_NO_FATAL_FAILURE(EdgesAreEqual(original, copied));
  }
}


// Edge Explicit Copying - construct detached edges and add them to a new model
TEST_F(ModelLoad, EdgeExplicitCopying)
{
  using namespace CW;
  ASSERT_FALSE(!m_model);

  std::vector<Edge> edges = m_model->entities().edges(false);

  // Make explicit detached copies
  size_t i = 0;
  std::vector<Edge> edges_copy; edges_copy.reserve(edges.size());
  std::cerr << "\033[33m[     INFO ] " << "NOTE: Materials and Layers cannot be assigned to Edges that are not attached to the model. EdgeExplicitCopying Test is expected to fail for equality of material and layer input and output. \033[0m" << std::endl;
  for (const Edge& edge : edges) {
    // if (i > 0) break; // only check one edge
    // i++;
    CW::Edge copy = edge.copy();
    // copy properties
    copy.color(edge.color());
    copy.smooth(edge.smooth());
    copy.soft(edge.soft());

    ASSERT_FALSE(copy.attached());
    EXPECT_NE(copy.ref().ptr, edge.ref().ptr);
    ASSERT_NO_FATAL_FAILURE(EdgesAreEqual(copy, edge));
    edges_copy.push_back(copy);
  }

  // Add detached copies into new model
  CW::Entities dest_entities = m_model_copy->entities();
  dest_entities.add_edges(edges_copy);
  i = 0;
  std::vector<Edge> copied_edges = dest_entities.edges(false);
  EXPECT_EQ(edges.size(), copied_edges.size());
  for (const Edge& original : edges) {
    // if (i > 0) break; // only check one edge
    // i++;
    // find matching copied edge by start/end coords
    auto found_it = std::find_if(copied_edges.begin(), copied_edges.end(),
      [&original](const Edge& value) {
        return (original.start().position() == value.start().position())
            && (original.end().position() == value.end().position());
      });
    if (found_it != copied_edges.end()) {
      const Edge& copied = *found_it;
      EXPECT_NE(original.ref().ptr, copied.ref().ptr);
      EXPECT_TRUE(copied.attached());
      ASSERT_NO_FATAL_FAILURE(EdgesAreEqual(original, copied));
    }
  }
}



} // namespace CW::Tests
