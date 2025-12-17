#include "gtest/gtest.h"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
// #include "SUAPI-CppWrapper/model/Vertex.hpp"
// #include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
// #include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/GeometryInput.hpp"

namespace CW::Tests {

// GeometryInput test - use GeometryInput to add edges into a model
TEST_F(ModelLoad, GeometryInputEdge)
{
  using namespace CW;
  ASSERT_FALSE(!m_model);

  std::vector<Edge> edges = m_model->entities().edges(false);
  std::vector<Material> edge_materials; edge_materials.reserve(edges.size());
  std::vector<Layer> edge_layers; edge_layers.reserve(edges.size());
  for (Edge& edge : edges) {
    edge_materials.push_back(edge.material());
    edge_layers.push_back(edge.layer());
  }

  // Create GeometryInput targeting the copy model
  CW::GeometryInput geom_input(m_model_copy);
  geom_input.load_materials(m_model->materials());
  geom_input.load_layers(m_model->layers());
  geom_input.add_edges(edges, edge_materials, edge_layers);

  // Fill the entities of the copy model with the geometry input
  CW::Entities dest_entities = m_model_copy->entities();
  SUResult res = dest_entities.fill(geom_input);
  EXPECT_EQ(res, SU_ERROR_NONE);

  // Verify edges were added
  std::vector<Edge> added_edges = dest_entities.edges(false);
  EXPECT_EQ(edges.size(), added_edges.size());
  for (size_t i = 0; i < edges.size(); ++i) {
    EdgesAreEqual(edges[i], added_edges[i]);
  }
  // TODO: test edges nested in groups and components.
}


// GeometryInputEdgeCopy - use GeometryInput to add copies of edges (detached) into a model
TEST_F(ModelLoad, GeometryInputEdgeCopy)
{
  using namespace CW;
  ASSERT_FALSE(!m_model);

  std::vector<Edge> edges = m_model->entities().edges(false);
  std::vector<Edge> copied_edges; copied_edges.reserve(edges.size());
  std::vector<Material> edge_materials; edge_materials.reserve(edges.size());
  std::vector<Layer> edge_layers; edge_layers.reserve(edges.size());
  for (Edge& edge : edges) {
    copied_edges.push_back(edge.copy());
    edge_materials.push_back(edge.material());
    edge_layers.push_back(edge.layer());
  }

  // Create GeometryInput targeting the copy model
  CW::GeometryInput geom_input(m_model_copy);
  geom_input.load_materials(m_model->materials());
  geom_input.load_layers(m_model->layers());
  geom_input.add_edges(copied_edges, edge_materials, edge_layers);

  // Fill the entities of the copy model with the geometry input
  CW::Entities dest_entities = m_model_copy->entities();
  SUResult res = dest_entities.fill(geom_input);
  EXPECT_EQ(res, SU_ERROR_NONE);

  // Verify edges were added
  std::vector<Edge> added_edges = dest_entities.edges(false);
  EXPECT_EQ(edges.size(), added_edges.size());
  for (size_t i = 0; i < edges.size(); ++i) {
    EdgesAreEqual(edges[i], added_edges[i]);
  }
  // TODO: test edges nested in groups and components.
}


// GeometryInputMaterials test - test that materials added with GeometryInput to a new model are valid
TEST_F(ModelLoad, GeometryInputMaterials)
{
  using namespace CW;
  ASSERT_FALSE(!m_model);

  std::vector<Face> faces = m_model->entities().faces();
  std::vector<Material> face_materials; face_materials.reserve(faces.size());
  std::vector<Layer> face_layers; face_layers.reserve(faces.size());
  for (Face& face : faces) {
    face_materials.push_back(face.material());
    face_layers.push_back(face.layer());
  }

  // Create GeometryInput targeting the copy model
  CW::GeometryInput geom_input(m_model_copy);
  geom_input.load_materials(m_model->materials());
  geom_input.load_layers(m_model->layers());
  // Check that target model has layers and materials loaded correctly:
  std::vector<Material> original_model_materials = m_model->materials();
  std::vector<Material> target_model_materials = m_model_copy->materials();
  for (size_t i=0; i < original_model_materials.size(); i++) {
    //std::cerr << original_model_materials[i].name().std_string() << std::endl;
    //std::cerr << target_model_materials[i].name().std_string() << std::endl;
    MaterialsAreEqual(original_model_materials[i], target_model_materials[i]);
  }
  // TODO: The tests above perhaps should return false
  // std::cerr << "Material Equality check done" << std::endl;
  // std::cerr << "Original Model's Material Names:" << std::endl;
  // for (auto mat : original_model_materials){
  //   std::cerr << mat.name().std_string() << std::endl;
  // }
  // std::cerr << "Target Model's Material Names:" << std::endl;
  // for (auto mat : target_model_materials){
  //   std::cerr << mat.name().std_string() << std::endl;
  // }

  // Fill the entities of the copy model with the geometry input
  CW::Entities dest_entities = m_model_copy->entities();
  SUResult res = dest_entities.fill(geom_input);
  EXPECT_EQ(res, SU_ERROR_NONE);
  // TODO: test edges nested in groups and components.
}



// GeometryInputFace test - use GeometryInput to add faces into a model
TEST_F(ModelLoad, GeometryInputFace)
{
  using namespace CW;
  ASSERT_FALSE(!m_model);
  std::vector<Face> faces = m_model->entities().faces();
  std::vector<Material> face_materials; face_materials.reserve(faces.size());
  std::vector<Layer> face_layers; face_layers.reserve(faces.size());
  for (Face& face : faces) {
    face_materials.push_back(face.material());
    face_layers.push_back(face.layer());
  }

  // Create GeometryInput targeting the copy model
  CW::GeometryInput geom_input(m_model_copy);
  geom_input.load_materials(m_model->materials());
  geom_input.load_layers(m_model->layers());
  // Check that target model has layers and materials loaded correctly:
  std::vector<Material> original_model_materials = m_model->materials();
  std::vector<Material> target_model_materials = m_model_copy->materials();
  for (size_t i=0; i < original_model_materials.size(); i++) {
    MaterialsAreEqual(original_model_materials[i], target_model_materials[i]);
  }
  geom_input.add_faces(faces, true);

  // Fill the entities of the copy model with the geometry input
  CW::Entities dest_entities = m_model_copy->entities();
  SUResult res = dest_entities.fill(geom_input);
  EXPECT_EQ(res, SU_ERROR_NONE);

  // Verify faces were added
  std::vector<Face> added_faces = dest_entities.faces();
  EXPECT_EQ(faces.size(), added_faces.size());
  std::vector<Material> copied_materials = m_model_copy->materials();
  for (size_t i = 0; i < faces.size(); ++i) {
    FacesAreEqual(faces[i], added_faces[i]);
  }
  // TODO: test edges nested in groups and components.
}


} // namespace CW::Tests
