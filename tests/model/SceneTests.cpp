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
#include "SUAPI-CppWrapper/model/Scene.hpp"
#include "SUAPI-CppWrapper/model/Camera.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {


class SceneTest : public testing::Test {
 protected:
  SceneTest() {
    CW::initialize();
  }
  ~SceneTest() override {
    CW::terminate();
  }
};


TEST_F(SceneTest, CreateDefaultScene) {
  CW::Scene scene;
  EXPECT_TRUE(!scene);
}


TEST_F(SceneTest, NullSceneThrows) {
  CW::Scene scene;
  EXPECT_THROW(scene.name(), std::logic_error);
  EXPECT_THROW(scene.description(), std::logic_error);
  EXPECT_THROW(scene.camera(), std::logic_error);
  EXPECT_THROW(scene.flags(), std::logic_error);
}


// Model-based tests: copy scenes from source model to destination model
class SceneModelTest : public ModelLoad {};


TEST_F(SceneModelTest, CopyScenes) {
  // Read scenes from the source model
  std::vector<CW::Scene> source_scenes = m_model->scenes();
  ASSERT_GT(source_scenes.size(), 0u);

  // Copy each scene using copy() and add to the destination model
  for (const auto& src : source_scenes) {
    CW::Scene dest_scene = src.copy();
    m_model_copy->add_scene(dest_scene);
    // Camera must be set after adding scene to model
    dest_scene.camera(src.camera());
  }

  // Read back from destination and verify properties match
  std::vector<CW::Scene> copied_scenes = m_model_copy->scenes();
  ASSERT_EQ(copied_scenes.size(), source_scenes.size());

  for (size_t i = 0; i < source_scenes.size(); ++i) {
    EXPECT_EQ(std::string(source_scenes[i].name()), std::string(copied_scenes[i].name()));
    EXPECT_EQ(std::string(source_scenes[i].description()), std::string(copied_scenes[i].description()));
    EXPECT_EQ(source_scenes[i].use_camera(), copied_scenes[i].use_camera());
    EXPECT_EQ(source_scenes[i].include_in_animation(), copied_scenes[i].include_in_animation());

    CW::Camera src_cam = source_scenes[i].camera();
    CW::Camera dest_cam = copied_scenes[i].camera();
    EXPECT_NEAR(src_cam.eye().x, dest_cam.eye().x, 1e-6);
    EXPECT_NEAR(src_cam.eye().y, dest_cam.eye().y, 1e-6);
    EXPECT_NEAR(src_cam.eye().z, dest_cam.eye().z, 1e-6);
    EXPECT_NEAR(src_cam.target().x, dest_cam.target().x, 1e-6);
    EXPECT_NEAR(src_cam.target().y, dest_cam.target().y, 1e-6);
    EXPECT_NEAR(src_cam.target().z, dest_cam.target().z, 1e-6);
  }

  SaveModel("Scenes");
}


}
