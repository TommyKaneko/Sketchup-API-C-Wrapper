#include "ModelTestUtility.hpp"
#include "ModelPath.h"

#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Texture.hpp"
#include "SUAPI-CppWrapper/model/ImageRep.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"
#include "SUAPI-CppWrapper/model/AttributeDictionary.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/Group.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"

namespace CW::Tests {


CW::Point3D SumPoints(const std::vector<CW::Point3D>& points) {
  double sum_x = 0.0;
  double sum_y = 0.0;
  double sum_z = 0.0;
  for (const auto& point : points) {
    sum_x += point.x;
    sum_y += point.y;
    sum_z += point.z;
  }
  return CW::Point3D(sum_x, sum_y, sum_z);
}

// Helper function to try to sort the order of a vector of faces for comparison
void SortFacesByProperties(std::vector<CW::Face>& faces) {
  std::sort(faces.begin(), faces.end(),
    [](const CW::Face& a, const CW::Face& b) {
      CW::Point3D sum_a = SumPoints(a.outer_loop().points());
      CW::Point3D sum_b = SumPoints(b.outer_loop().points());
      if (std::abs(sum_a.x - sum_b.x) > 1e-6) {
        return sum_a.x < sum_b.x;
      }
      if (std::abs(sum_a.y - sum_b.y) > 1e-6) {
        return sum_a.y < sum_b.y;
      }
      return sum_a.z < sum_b.z;
    });
}


ModelLoad::ModelLoad():
  m_model(nullptr) {
  // You can do set-up work for each test here.
  // Load models
  std::string model_path = TEST_MODEL_PATH  + "/box and box.skp";
  CW::initialize();
  m_model = new CW::Model(model_path);
  // An empty model for testing adding entities
  m_model_copy = new CW::Model();
    // FIXME: the model must not be released before we are done with it.  The destructor in Model() will release on an object being destroyed.
}

ModelLoad::~ModelLoad() {
  // You can do clean-up work that doesn't throw exceptions here.
  delete m_model;
  delete m_model_copy;
  CW::terminate();
}

// If the constructor and destructor are not enough for setting up
// and cleaning up each test, you can define the following methods:

void ModelLoad::SetUp() {
  // Code here will be called immediately after the constructor (right
  // before each test).
}

void ModelLoad::TearDown() {
  // Code here will be called immediately after each test (right
  // before the destructor).

  // Test all elements in the model belong to the model
  this->ModelEntitiesOwnership(m_model->entities(), m_model);
  this->ModelEntitiesOwnership(m_model_copy->entities(), m_model_copy);
}

void ModelLoad::SaveModel(std::string suffix) {
  std::string save_path = TEST_MODEL_OUTPUT_PATH + "/" + "box and box_" + suffix + ".skp";
  m_model_copy->save(save_path);
}


void ModelLoad::ModelEntitiesOwnership(const CW::Entities& entities, const CW::Model* model) {
  std::vector<CW::Face> faces = entities.faces();
  for (const CW::Face& face : faces) {
    this->ModelElementOwnership(face, model);
  }
  std::vector<CW::Edge> edges = entities.edges();
  for (const CW::Edge& edge : edges) {
    this->ModelElementOwnership(edge, model);
  }
  std::vector<CW::ComponentInstance> instances = entities.instances();
  for (const CW::ComponentInstance& instance : instances) {
    this->ModelElementOwnership(instance, model);
  }
  std::vector<CW::Group> groups = entities.groups();
  for (const CW::Group& group : groups) {
    this->ModelElementOwnership(group, model);
  }
}


void ModelLoad::ModelElementOwnership(const CW::DrawingElement& element, const CW::Model* model) {
  ASSERT_TRUE(element.is_valid());
  this->ModelEntityOwnership(element, model);
  // Recursively check child entities
  switch (element.entity_type()) {
    case SURefType_Group: {
      SUGroupRef group_ref = SUGroupFromDrawingElement(element.ref());
      CW::Group group(group_ref);
      CW::Entities group_entities = group.entities();
      ModelEntitiesOwnership(group_entities, model);
      break;
    }
    case SURefType_ComponentInstance: {
      SUComponentInstanceRef instance_ref = SUComponentInstanceFromDrawingElement(element.ref());
      CW::ComponentInstance instance(instance_ref);
      this->ModelElementOwnership(instance.definition(), model);
      break;
    }
    case SURefType_ComponentDefinition: {
      SUComponentDefinitionRef definition_ref = SUComponentDefinitionFromDrawingElement(element.ref());
      CW::ComponentDefinition definition(definition_ref);
      CW::Entities definition_entities = definition.entities();
      ModelEntitiesOwnership(definition_entities, model);
      break;
    }
    default:
      break;
  }
}


void ModelLoad::ModelEntityOwnership(const CW::Entity& entity, const CW::Model* model) {
  ASSERT_TRUE(entity.is_valid());
  CW::Model entity_model = entity.model();
  EXPECT_EQ(entity_model.ref().ptr, model->ref().ptr);
  // Recursively check child atttributes
  std::vector<CW::AttributeDictionary> dicts = entity.attribute_dictionaries();
  for (const CW::AttributeDictionary& dict : dicts) {
    this->ModelEntityOwnership(dict, model);
  }
}


void ModelLoad::MaterialsAreEqual(const CW::Material& material1, const CW::Material& material2){
  if (!material1.is_valid() || !material2.is_valid()) {
    EXPECT_EQ(material1.is_valid(), material2.is_valid());
    return;
  }
  this->EntitysAreEqual(material1, material2);
  bool names_equal = material1.name().std_string() == material2.name().std_string();
  EXPECT_EQ(material1.name().std_string(), material2.name().std_string()) << "\033[33mMaterials cannot be added to unattached DrawingElements - review how material was applied\033[0m";
  if (!names_equal) {
    //Check attached or not
    std::cerr << "Material1 attached:" << material1.attached() << ", Material2 attached: " << material2.attached() << std::endl;
    return;
  }
  EXPECT_EQ(material1.opacity(), material2.opacity());
  EXPECT_EQ(material1.type(), material2.type());
  EXPECT_EQ(material1.use_alpha(), material2.use_alpha());
  TexturesAreEqual(material1.texture(), material2.texture());
}


void ModelLoad::TexturesAreEqual(const CW::Texture& tex1, const CW::Texture& tex2){
  if (!tex1.is_valid() || !tex2.is_valid()) {
    EXPECT_EQ(tex1.is_valid(), tex2.is_valid());
    return;
  }
  EXPECT_EQ(tex1.width(), tex2.width());
  EXPECT_EQ(tex1.height(), tex2.height());
  EXPECT_EQ(tex1.file_name(), tex2.file_name());
  EXPECT_EQ(tex1.alpha_used(), tex2.alpha_used());
  EXPECT_EQ(tex1.s_scale(), tex2.s_scale());
  EXPECT_EQ(tex1.t_scale(), tex2.t_scale());
  ImageRepsAreEqual(tex1.image_rep(), tex2.image_rep());
}

void ModelLoad::ImageRepsAreEqual(const CW::ImageRep& image1, const CW::ImageRep& image2){
  if (!image1 || !image1) {
    EXPECT_EQ(!image1, !image2);
    return;
  }
  EXPECT_EQ(image1.width(), image2.width());
  EXPECT_EQ(image1.height(), image2.height());
  EXPECT_EQ(image1.row_padding(), image2.row_padding());
  EXPECT_EQ(image1.data_size(), image2.data_size());
  EXPECT_EQ(image1.bits_per_pixel(), image2.bits_per_pixel());
  std::vector<SUByte> image1_data = image1.pixel_data();
  std::vector<SUByte> image2_data = image2.pixel_data();
  for (size_t i; i < image1_data.size(); i++) {
    EXPECT_EQ(image1_data[i], image2_data[i]);
  }
}


void ModelLoad::LayersAreEqual(const CW::Layer& layer1, const CW::Layer& layer2) {
  if (!layer1 || !layer2) {
    EXPECT_EQ(layer1.is_valid(), layer2.is_valid()); // Current version failing: SUDrawingElementSetLayer followed by SUDrawingElementGetLayer gives inconsistent results.
    return;
  }
  EXPECT_EQ(layer1.name().std_string(), layer2.name().std_string());
  this->EntitysAreEqual(layer1, layer2);
  // TODO: add more properties as they are added to the API.
}


void ModelLoad::AttributeDictionariesAreEqual(const CW::AttributeDictionary& dict1, const CW::AttributeDictionary& dict2) {
  if (!dict1.is_valid() || !dict2.is_valid()) {
    EXPECT_EQ(dict1.is_valid(), dict2.is_valid());
    return;
  }
  EXPECT_EQ(dict1.name(), dict2.name());
  std::vector<std::string> keys1 = dict1.get_keys();
  std::vector<std::string> keys2 = dict2.get_keys();
  EXPECT_EQ(keys1.size(), keys2.size());
  for (size_t i = 0; i < keys1.size(); ++i) {
    EXPECT_EQ(keys1[i], keys2[i]);
    EXPECT_EQ(dict1.get_value(keys1[i]), dict2.get_value(keys2[i]));
  }
}


void ModelLoad::EntitysAreEqual(const CW::Entity& entity1, const CW::Entity& entity2) {
  if (!entity1.is_valid() || !entity1.is_valid()) {
    EXPECT_EQ(entity1.is_valid(), entity2.is_valid());
    return;
  }
  EXPECT_EQ(entity1.entity_type(), entity2.entity_type());
  // Check Attribute Dictionaries
  std::vector<CW::AttributeDictionary> dicts1 = entity1.attribute_dictionaries();
  std::vector<CW::AttributeDictionary> dicts2 = entity2.attribute_dictionaries();
  EXPECT_EQ(dicts1.size(), dicts2.size());
  for (size_t i = 0; i < dicts1.size(); ++i) {
    this->AttributeDictionariesAreEqual(dicts1[i], dicts2[i]);
  }
}


void ModelLoad::DrawingElementsAreEqual(const CW::DrawingElement& element1, const CW::DrawingElement& element2) {
  if (!element1.is_valid() || !element2.is_valid()) {
    EXPECT_EQ(!element1.is_valid(), !element2.is_valid());
    return;
  }
  this->EntitysAreEqual(element1, element2);
  EXPECT_EQ(element1.casts_shadows(), element2.casts_shadows());
  EXPECT_EQ(element1.hidden(), element2.hidden());
  if (!!element1.layer()) {
    EXPECT_EQ(element1.layer().attached(), element2.layer().attached()); // Only attached layers should be assigned to a DrawingElement
    this->LayersAreEqual(element1.layer(), element2.layer());
  }
  if (!!element1.material()) {
    EXPECT_EQ(element1.material().attached(), element2.material().attached()); // Only attached layers should be assigned to a DrawingElement
    this->MaterialsAreEqual(element1.material(), element2.material());
  }
  EXPECT_EQ(element1.receive_shadows(), element2.receive_shadows());
}


void ModelLoad::EdgesAreEqual(const CW::Edge& edge1, const CW::Edge& edge2) {
  if (!edge1.is_valid() || !edge2.is_valid()) {
    EXPECT_EQ(edge1.is_valid(), edge2.is_valid());
    return;
  }
  DrawingElementsAreEqual(edge1, edge2);

  // Compare start and end positions
  // Two edges with swapped start and end should be considered equal
  if (edge1.start().position() == edge2.start().position()) {
    EXPECT_EQ(edge1.start().position(), edge2.start().position());
    EXPECT_EQ(edge1.end().position(), edge2.end().position());
  } else{
    EXPECT_EQ(edge1.start().position(), edge2.end().position());
    EXPECT_EQ(edge1.end().position(), edge2.start().position());
  }

  // Compare drawing properties
  EXPECT_EQ(edge1.smooth(), edge2.smooth());
  EXPECT_EQ(edge1.soft(), edge2.soft());

  // Compare color
  EXPECT_EQ(edge1.color(), edge2.color());
}


void ModelLoad::FacesAreEqual(const CW::Face& face1, const CW::Face& face2) {
  if (!face1.is_valid() || !face2.is_valid()) {
    EXPECT_EQ(face1.is_valid(), face2.is_valid());
    return;
  }
  DrawingElementsAreEqual(face1, face2);

  // Compare loops
  LoopsAreEqual(face1.outer_loop(), face2.outer_loop());
  std::vector<Loop> face1_inner_loops = face1.inner_loops();
  std::vector<Loop> face2_inner_loops = face2.inner_loops();
  ASSERT_EQ(face1_inner_loops.size(), face2_inner_loops.size());
  for (size_t i=0; i < face1_inner_loops.size(); i++) {
    LoopsAreEqual(face1_inner_loops[i], face2_inner_loops[i]);
  }
  EXPECT_EQ(face1.normal(), face2.normal());
  EXPECT_EQ(face1.plane(), face2.plane());
  MaterialsAreEqual(face1.back_material(), face2.back_material());
  EXPECT_EQ(face1.area(), face2.area());
}


void ModelLoad::LoopsAreEqual(const CW::Loop& loop1, const CW::Loop& loop2) {
  if (!loop1.is_valid() || !loop2.is_valid()) {
    EXPECT_EQ(loop1.is_valid(), loop2.is_valid());
    return;
  }
  EXPECT_EQ(loop1.num_vertices(), loop2.num_vertices());
  EXPECT_EQ(loop1.points(), loop2.points());
  std::vector<Edge> loop1_edges = loop1.edges();
  std::vector<Edge> loop2_edges = loop2.edges();
  ASSERT_EQ(loop1_edges.size(), loop2_edges.size());
  for (size_t i=0; i < loop1_edges.size(); i++) {
    EdgesAreEqual(loop1_edges[i], loop2_edges[i]);
  }
}


void ModelLoad::GroupsAreEqual(const CW::Group& group1, const CW::Group& group2) {
  if (!group1.is_valid() || !group2.is_valid()) {
    EXPECT_EQ(group1.is_valid(), group2.is_valid());
    return;
  }
  // Compare basic entity properties
  this->EntitysAreEqual(group1, group2);

  // Compare name and transformation
  EXPECT_EQ(group1.name().std_string(), group2.name().std_string());
  EXPECT_EQ(group1.transformation(), group2.transformation());

  // Compare nested entities (faces)
  std::vector<Face> faces1 = group1.entities().faces();
  std::vector<Face> faces2 = group2.entities().faces();
  EXPECT_EQ(faces1.size(), faces2.size());
  for (size_t i = 0; i < faces1.size(); ++i) {
    ASSERT_NO_FATAL_FAILURE(FacesAreEqual(faces1[i], faces2[i]));
  }

  // Compare stray edges
  std::vector<Edge> edges1 = group1.entities().edges(true);
  std::vector<Edge> edges2 = group2.entities().edges(true);
  EXPECT_EQ(edges1.size(), edges2.size());
  for (size_t i = 0; i < edges1.size(); ++i) {
    ASSERT_NO_FATAL_FAILURE(EdgesAreEqual(edges1[i], edges2[i]));
  }
}


// New utility: compare two component definitions by name, group flag and nested entities
void ModelLoad::ComponentDefinitionsAreEqual(const CW::ComponentDefinition& def1, const CW::ComponentDefinition& def2) {
  if (!def1.is_valid() || !def2.is_valid()) {
    EXPECT_EQ(def1.is_valid(), def2.is_valid());
    return;
  }
  EXPECT_EQ(def1.name().std_string(), def2.name().std_string());
  EXPECT_EQ(def1.is_group(), def2.is_group());
  if (def1.is_group()) {
    std::cerr << "Definitions is a group - not expect at this stage." << std::endl;
  }
  // Compare faces
  std::vector<CW::Face> faces1 = def1.entities().faces();
  std::vector<CW::Face> faces2 = def2.entities().faces();
  ASSERT_EQ(faces1.size(), faces2.size());
  for (size_t i = 0; i < faces1.size(); ++i) {
    ASSERT_NO_FATAL_FAILURE(FacesAreEqual(faces1[i], faces2[i]));
  }

  // Compare stray edges
  std::vector<CW::Edge> edges1 = def1.entities().edges(true);
  std::vector<CW::Edge> edges2 = def2.entities().edges(true);
  EXPECT_EQ(edges1.size(), edges2.size());
  // TODO: deeper check for edges that may not be in the same order....
  //for (size_t i = 0; i < edges1.size(); ++i) {
  //  ASSERT_NO_FATAL_FAILURE(EdgesAreEqual(edges1[i], edges2[i]));
  //}

  // Compare nested groups (if any)
  std::vector<CW::Group> groups1 = def1.entities().groups();
  std::vector<CW::Group> groups2 = def2.entities().groups();
  EXPECT_EQ(groups1.size(), groups2.size());
  for (size_t i = 0; i < groups1.size(); ++i) {
    ASSERT_NO_FATAL_FAILURE(GroupsAreEqual(groups1[i], groups2[i]));
  }
}

// New utility: compare two component instances by name, transform, and definition content
void ModelLoad::ComponentsAreEqual(const CW::ComponentInstance& inst1, const CW::ComponentInstance& inst2) {
  if (!inst1.is_valid() || !inst2.is_valid()) {
    EXPECT_EQ(inst1.is_valid(), inst2.is_valid());
    return;
  }
  // Compare simple properties
  EXPECT_EQ(inst1.name().std_string(), inst2.name().std_string());
  EXPECT_EQ(inst1.transformation(), inst2.transformation());

  // Compare definitions
  ComponentDefinition def1 = inst1.definition();
  ComponentDefinition def2 = inst2.definition();
  ASSERT_NO_FATAL_FAILURE(ComponentDefinitionsAreEqual(def1, def2));
}


// Code here will be called immediately after the constructor (right
  // before each test).

} // namespace CW::Tests

