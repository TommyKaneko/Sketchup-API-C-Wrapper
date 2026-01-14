#ifndef ModelTestUtility_hpp
#define ModelTestUtility_hpp

#include "SketchUpAPITests.hpp"
#include "gtest/gtest.h"

#include "SUAPI-CppWrapper/model/Model.hpp"

// forward declaration
namespace CW {
class Material;
class Texture;
class ImageRep;
class Layer;
class AttributeDictionary;
class TypedValue;
class Entity;
class DrawingElement;
class Edge;
class Face;
class Loop;
class Group;
class Model;
class ComponentInstance;
class ComponentDefinition;
class Point3D;
}

namespace CW {
namespace Tests {

// Declare helper functions
CW::Point3D SumPoints(const std::vector<CW::Point3D>& points);
void SortFacesByProperties(std::vector<CW::Face>& faces);

class ModelLoad : public testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ModelLoad();
     // You can do set-up work for each test here.

  ~ModelLoad() override;
     // You can do clean-up work that doesn't throw exceptions here.

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override;
     // Code here will be called immediately after the constructor (right
     // before each test).

  void TearDown() override;
     // Code here will be called immediately after each test (right
     // before the destructor).

  // Save the model_copy to the test model output folder, with the name appended to original model name.
  void SaveModel(std::string suffix);

  // Class members declared here can be used by all tests in the test suite
  // for Model.
  CW::Model* m_model = nullptr;

  // An empty model for testing adding entities
  CW::Model* m_model_copy = nullptr;


  // Helper Methods

  void ModelEntitiesOwnership(const CW::Entities& entities, const CW::Model* model);
  // Tests that the given element and all of its children belongs to the given model
  void ModelElementOwnership(const CW::DrawingElement& element, const CW::Model* model);
  // Tests that the given entity and all of its children belongs to the given model
  void ModelEntityOwnership(const CW::Entity& entity, const CW::Model* model);

  void MaterialsAreEqual(const CW::Material& mat1, const CW::Material& mat2);
  // Tests that two textures have the same properties
  void TexturesAreEqual(const CW::Texture& tex1, const CW::Texture& tex2);

  void ImageRepsAreEqual(const CW::ImageRep& image1, const CW::ImageRep& image2);

  void LayersAreEqual(const CW::Layer& layer1, const CW::Layer& layer2);

  void EntitysAreEqual(const CW::Entity& entity1, const CW::Entity& entity2);

  void DrawingElementsAreEqual(const CW::DrawingElement& element1, const CW::DrawingElement& element2);

  void EdgesAreEqual(const CW::Edge& edge1, const CW::Edge& edge2);

  void FacesAreEqual(const CW::Face& face1, const CW::Face& face2);

  void LoopsAreEqual(const CW::Loop& loop1, const CW::Loop& loop2);

  void AttributeDictionariesAreEqual(const CW::AttributeDictionary& dict1, const CW::AttributeDictionary& dict2);

  void GroupsAreEqual(const CW::Group& group1, const CW::Group& group2);

  void ComponentDefinitionsAreEqual(const CW::ComponentDefinition& def1, const CW::ComponentDefinition& def2);
  void ComponentsAreEqual(const CW::ComponentInstance& inst1, const CW::ComponentInstance& inst2);

};

} // namespace Tests
} // namespace CW

#endif /* ModelTestUtility_hpp */
