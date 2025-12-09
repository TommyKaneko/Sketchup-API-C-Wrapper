#include "gtest/gtest.h"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW::Tests {

// Layer Referencing Tests that references of a layer point to the same object
TEST_F(ModelLoad, LayerReferencing)
{
	using namespace CW;
  ASSERT_FALSE(!m_model);
  std::vector<Layer> layers = m_model->layers();
  // The above operation should refer to the same layers
  for (const Layer& lay : layers) {
    CW::Layer layer_ref = lay;
    // We should have the same reference
    EXPECT_EQ(layer_ref.attached(), true);
    EXPECT_EQ(lay.attached(), true);
    EXPECT_EQ(layer_ref.ref().ptr, lay.ref().ptr);
  }

  // Now check that copies of copies also have the same properties
  for (const Layer& lay : layers) {
    // Test a copy
    CW::Layer layer_copy = lay.copy();
    EXPECT_NE(layer_copy.ref().ptr, lay.ref().ptr);
    EXPECT_EQ(lay.attached(), true);
    EXPECT_EQ(layer_copy.attached(), false);
    // We should have the same properties
    LayersAreEqual(lay, layer_copy);

    // Test copy of a copy:
    CW::Layer layer_copy_copy = layer_copy.copy();
    EXPECT_NE(layer_copy_copy.ref().ptr, layer_copy.ref().ptr);
    EXPECT_EQ(layer_copy_copy.attached(), false);
    LayersAreEqual(layer_copy_copy, layer_copy);
  }
}


// Layer Referencing Tests that references of a layer point to the same object
TEST_F(ModelLoad, LayerImplicitCopying)
{
	using namespace CW;
  ASSERT_FALSE(!m_model);
  std::vector<Layer> layers = m_model->layers();
  // The above operation should refer to the same layers
	m_model_copy->add_layers(layers);
  std::vector<Layer> copied_layers = m_model_copy->layers();
  EXPECT_EQ(layers.size(), copied_layers.size());
  // TODO: we should match layer by name, as the order may not be the same
  for (size_t i = 0; i < layers.size(); ++i) {
    const Layer& original_lay = layers[i];
    const Layer& copied_lay = copied_layers[i];
    // We should have different references
    EXPECT_NE(original_lay.ref().ptr, copied_lay.ref().ptr);
    EXPECT_TRUE(copied_lay.attached());
    // But the properties should be the same
    LayersAreEqual(original_lay, copied_lay);
  }
}


// Layer Referencing Tests that references of a layer point to the same object
TEST_F(ModelLoad, LayerExplicitCopying)
{
	using namespace CW;
  ASSERT_FALSE(!m_model);
  std::vector<Layer> layers = m_model->layers();
  // Check copies are new independent objects
  std::vector<Layer> layers_copy; layers_copy.reserve(layers.size());
  for (const Layer& lay : layers) {
    if (lay.name() == CW::String("Untagged")) {
      // Skip default layer as it cannot be added to a new model
      continue;
    }
    CW::Layer layer_copy = lay.copy();
    ASSERT_FALSE(layer_copy.attached());
    // We should have different references
    EXPECT_NE(layer_copy.ref().ptr, lay.ref().ptr);
    // But the properties should be the same
    LayersAreEqual(layer_copy, lay);
    layers_copy.push_back(layer_copy);
  }

  // The above operation should refer to the same layers
	m_model_copy->add_layers(layers_copy);
  std::vector<Layer> copied_layers = m_model_copy->layers();
  EXPECT_EQ(layers.size(), copied_layers.size());
  for (size_t i = 0; i < layers.size(); ++i) {
    const Layer& original_lay = layers[i];
    const Layer& copied_lay = copied_layers[i];
    // We should have different references
    EXPECT_NE(original_lay.ref().ptr, copied_lay.ref().ptr);
    // But the properties should be the same
    LayersAreEqual(original_lay, copied_lay);
    EXPECT_TRUE(copied_lay.attached());
  }
}


}
