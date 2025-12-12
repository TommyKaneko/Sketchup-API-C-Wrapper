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
#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/String.hpp"


namespace CW::Tests {

ModelLoad::ModelLoad():
  m_model(nullptr) {
  // You can do set-up work for each test here.
  // Load models
  std::string model_path = TEST_MODEL_PATH  + "/box and box.skp";
  CW::initialize();
  m_model = new CW::Model(model_path);
  // An empty model for testing adding entities
  m_model_copy = new CW::Model();
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
}

void ModelLoad::MaterialsAreEqual(const CW::Material& material1, const CW::Material& material2){
  if (!material1.is_valid() || !material2.is_valid()) {
    EXPECT_EQ(material1.is_valid(), material2.is_valid());
    return;
  }
  this->EntitysAreEqual(material1, material2);
  ASSERT_EQ(material1.name().std_string(), material2.name().std_string()) << "\033[33mMaterials cannot be added to unattached DrawingElements - review how material was applied\033[0m";
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
  // Check Atribute Dictionaries
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
  EXPECT_EQ(edge1.start().position(), edge2.start().position());
  EXPECT_EQ(edge1.end().position(), edge2.end().position());

  // Compare geometric vector
  EXPECT_EQ(edge1.vector(), edge2.vector());

  // Compare drawing properties
  EXPECT_EQ(edge1.smooth(), edge2.smooth());
  EXPECT_EQ(edge1.soft(), edge2.soft());

  // Compare color
  EXPECT_EQ(edge1.color(), edge2.color());
}

  // Code here will be called immediately after the constructor (right
  // before each test).

} // namespace CW::Tests

