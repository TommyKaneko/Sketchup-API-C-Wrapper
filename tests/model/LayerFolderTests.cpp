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

#include <stdexcept>

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/LayerFolder.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {

/*********************
 * LayerFolder Tests *
 *********************/

TEST(LayerFolder, DefaultConstructor) {
  CW::LayerFolder folder;
  EXPECT_TRUE(!folder);
}

TEST(LayerFolder, CreateWithName) {
  CW::LayerFolder folder("Test Folder");
  EXPECT_FALSE(!folder);
  EXPECT_EQ(std::string(folder.name()), "Test Folder");
}

TEST(LayerFolder, SetName) {
  CW::LayerFolder folder("Original");
  folder.name("Renamed");
  EXPECT_EQ(std::string(folder.name()), "Renamed");
}

TEST(LayerFolder, Visibility) {
  CW::LayerFolder folder("Vis Test");
  // Default should be visible
  EXPECT_TRUE(folder.visible());
  folder.visible(false);
  EXPECT_FALSE(folder.visible());
  folder.visible(true);
  EXPECT_TRUE(folder.visible());
}

TEST(LayerFolder, NullThrows) {
  CW::LayerFolder folder;
  EXPECT_THROW(folder.name(), std::logic_error);
  EXPECT_THROW(folder.visible(), std::logic_error);
}

TEST(LayerFolder, CopyConstructor) {
  CW::LayerFolder folder("Copy Test");
  CW::LayerFolder copy(folder);
  EXPECT_FALSE(!copy);
  EXPECT_EQ(std::string(copy.name()), "Copy Test");
  // Different ref pointers for detached copies
  EXPECT_NE(folder.ref().ptr, copy.ref().ptr);
}

TEST(LayerFolder, CopyAssignment) {
  CW::LayerFolder folder("Assign Test");
  CW::LayerFolder copy;
  copy = folder;
  EXPECT_FALSE(!copy);
  EXPECT_EQ(std::string(copy.name()), "Assign Test");
}

// Test adding LayerFolder to a model
TEST_F(ModelLoad, LayerFolderAddToModel) {
  using namespace CW;
  ASSERT_FALSE(!m_model_copy);
  LayerFolder folder("Test Folder");
  m_model_copy->add_layer_folder(folder);
  std::vector<LayerFolder> folders = m_model_copy->layer_folders();
  EXPECT_GE(folders.size(), 1u);
  bool found = false;
  for (const auto& f : folders) {
    if (std::string(f.name()) == "Test Folder") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);
}


/************************************
 * LayerFolder Model Copy Tests     *
 ************************************/

class LayerFolderModelTest : public ModelLoad {};

Layer find_layer_in_model_by_name(Model& model, const std::string& layer_name) {
  std::vector<Layer> model_layers = model.layers();
  for (Layer& model_layer : model_layers) {
    if (std::string(model_layer.name()) == layer_name) {
      return model_layer;
    }
  }
  throw std::runtime_error("Copied layer not found in target model: " + layer_name);
}

void copy_layer_to_folder(const Layer& source_layer, LayerFolder& target_folder, Model& target_model) {
  const std::string layer_name = std::string(source_layer.name());
  std::vector<Layer> layers_to_add = {source_layer.copy()};
  target_model.add_layers(layers_to_add);
  Layer attached_layer = find_layer_in_model_by_name(target_model, layer_name);
  target_folder.add_layer(attached_layer);
}


void add_layer_folders_nested(LayerFolder& parent_folder, const std::vector<LayerFolder>& layer_folder_list) {
  using namespace CW;
  Model target_model = parent_folder.model();
  std::string parent_name = std::string(parent_folder.name());
  for (const LayerFolder& folder : layer_folder_list) {
    if (!!folder.parent_folder() && std::string(folder.parent_folder().name()) == parent_name) {
      LayerFolder folder_copy = folder.copy();
      parent_folder.model().add_layer_folder(folder_copy);
      assert(folder_copy.attached());
      parent_folder.add_layer_folder(folder_copy);
      add_layer_folders_nested(folder_copy, folder.sub_folders());
      // Also copy layers in the folder
      std::vector<Layer> folder_layers = folder.layers();
      for (const auto& layer : folder_layers) {
        copy_layer_to_folder(layer, folder_copy, target_model);
      }
    }
  }
}


TEST_F(LayerFolderModelTest, CopyLayerFolderWithLayers) {
  using namespace CW;
  ASSERT_FALSE(!m_model_copy);

  std::vector<LayerFolder> folders = m_model->layer_folders();
  for (LayerFolder& folder: folders) {
    if (!folder.parent_folder()) {
      LayerFolder folder_copy = folder.copy();
      m_model_copy->add_layer_folder(folder_copy);
      EXPECT_TRUE(folder_copy.attached());
      add_layer_folders_nested(folder_copy, folder.sub_folders());
      // Also copy layers in the folder
      std::vector<Layer> folder_layers = folder.layers();
      for (const auto& layer : folder_layers) {
        copy_layer_to_folder(layer, folder_copy, *m_model_copy);
      }
    }
  }
  std::vector<LayerFolder> copied_folders = m_model_copy->layer_folders();
  EXPECT_EQ(copied_folders.size(), folders.size());
  for (LayerFolder& source_folder : folders) {
    // Find the corresponding source folder layer by name
    bool found = false;
    for (LayerFolder& copied_folder : copied_folders) {
      if (std::string(source_folder.name()) == std::string(copied_folder.name())) {
        found = true;
        // Check equality of parent folder names (if any)
        if (!!source_folder.parent_folder() && !!copied_folder.parent_folder()) {
          EXPECT_EQ(std::string(source_folder.parent_folder().name()), std::string(copied_folder.parent_folder().name()));
        } else {
          EXPECT_TRUE(!source_folder.parent_folder());
          EXPECT_TRUE(!copied_folder.parent_folder());
        }
        // Check equality of layer names in the folder
        std::vector<Layer> source_layers = source_folder.layers();
        std::vector<Layer> copied_layers = copied_folder.layers();
        EXPECT_EQ(source_layers.size(), copied_layers.size()) << "Layer count mismatch in folder '" << std::string(source_folder.name()) << "'";
        for (const Layer& source_layer : source_layers) {
          // Layers may not be returned in the same order, so find the corresponding layer by name
          bool layer_found = false;
          for (const auto& cl : copied_layers) {
            if (std::string(cl.name()) == std::string(source_layer.name())) {
              layer_found = true;
              break;
            }
          }
          EXPECT_TRUE(layer_found) << "Layer '" << std::string(source_layer.name()) << "' not found in copied folder '" << std::string(copied_folder.name()) << "'";
        }
      }
    }
    EXPECT_TRUE(found);
  }
  SaveModel("LayerFolderWithLayers");
}

} /* namespace CW::Tests */
