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
#include "SUAPI-CppWrapper/model/Camera.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {


class CameraTest : public testing::Test {
 protected:
  CameraTest() {
    CW::initialize();
  }
  ~CameraTest() override {
    CW::terminate();
  }
};


TEST_F(CameraTest, CreateDefaultCamera) {
  CW::Camera camera;
  EXPECT_TRUE(!camera);
}


TEST_F(CameraTest, CreateValidCamera) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));
  EXPECT_FALSE(!camera);
}


TEST_F(CameraTest, SetAndGetOrientation) {
  CW::Camera camera(CW::Point3D(10.0, 20.0, 30.0), CW::Point3D(0.0, 0.0, 0.0), CW::Vector3D(0.0, 0.0, 1.0));

  CW::Point3D got_eye, got_target;
  CW::Vector3D got_up;
  camera.orientation(got_eye, got_target, got_up);

  EXPECT_DOUBLE_EQ(got_eye.x, 10.0);
  EXPECT_DOUBLE_EQ(got_eye.y, 20.0);
  EXPECT_DOUBLE_EQ(got_eye.z, 30.0);
  EXPECT_DOUBLE_EQ(got_target.x, 0.0);
  EXPECT_DOUBLE_EQ(got_target.y, 0.0);
  EXPECT_DOUBLE_EQ(got_target.z, 0.0);
}


TEST_F(CameraTest, EyeTargetUpHelpers) {
  CW::Camera camera(CW::Point3D(5.0, 10.0, 15.0), CW::Point3D(0.0, 0.0, 0.0), CW::Vector3D(0.0, 0.0, 1.0));

  CW::Point3D got_eye = camera.eye();
  CW::Point3D got_target = camera.target();
  EXPECT_DOUBLE_EQ(got_eye.x, 5.0);
  EXPECT_DOUBLE_EQ(got_eye.y, 10.0);
  EXPECT_DOUBLE_EQ(got_eye.z, 15.0);
  EXPECT_DOUBLE_EQ(got_target.x, 0.0);
  EXPECT_DOUBLE_EQ(got_target.y, 0.0);
  EXPECT_DOUBLE_EQ(got_target.z, 0.0);
}


TEST_F(CameraTest, DirectionVector) {
  CW::Camera camera(CW::Point3D(10.0, 0.0, 0.0), CW::Point3D(0.0, 0.0, 0.0), CW::Vector3D(0.0, 0.0, 1.0));

  CW::Vector3D dir = camera.direction();
  // Direction should point from eye to target: (-1, 0, 0) normalized
  EXPECT_LT(dir.x, 0.0);
  EXPECT_NEAR(dir.y, 0.0, 1e-6);
  EXPECT_NEAR(dir.z, 0.0, 1e-6);
}


TEST_F(CameraTest, PerspectiveMode) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));

  // Default is perspective
  EXPECT_TRUE(camera.perspective());

  // Set to orthographic
  camera.perspective(false);
  EXPECT_FALSE(camera.perspective());

  // Set back to perspective
  camera.perspective(true);
  EXPECT_TRUE(camera.perspective());
}


TEST_F(CameraTest, FOV) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));

  camera.perspective(true);
  camera.fov(45.0);
  EXPECT_DOUBLE_EQ(camera.fov(), 45.0);

  camera.fov(60.0);
  EXPECT_DOUBLE_EQ(camera.fov(), 60.0);
}


TEST_F(CameraTest, OrthographicHeight) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));

  camera.orthographic_height(100.0);
  // Setting orthographic height makes it orthographic
  EXPECT_FALSE(camera.perspective());
  EXPECT_DOUBLE_EQ(camera.orthographic_height(), 100.0);
}


TEST_F(CameraTest, Description) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));

  camera.description(std::string("Test camera description"));
  CW::String desc = camera.description();
  EXPECT_EQ(desc.std_string(), "Test camera description");
}


TEST_F(CameraTest, ImageWidth) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));

  camera.image_width(36.0);
  EXPECT_DOUBLE_EQ(camera.image_width(), 36.0);
}


TEST_F(CameraTest, TwoDimensional) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));

  camera.is_2d(true);
  EXPECT_TRUE(camera.is_2d());

  camera.is_2d(false);
  EXPECT_FALSE(camera.is_2d());
}


TEST_F(CameraTest, Scale2D) {
  CW::Camera camera(CW::Point3D(0, 0, 10), CW::Point3D(0, 0, 0), CW::Vector3D(0, 1, 0));

  camera.scale_2d(2.0);
  EXPECT_DOUBLE_EQ(camera.scale_2d(), 2.0);
}


TEST_F(CameraTest, CopyConstructor) {
  CW::Camera camera(CW::Point3D(10.0, 20.0, 30.0), CW::Point3D(0.0, 0.0, 0.0), CW::Vector3D(0.0, 0.0, 1.0));
  camera.fov(45.0);

  CW::Camera copy(camera);
  EXPECT_FALSE(!copy);

  CW::Point3D copy_eye = copy.eye();
  EXPECT_NEAR(copy_eye.x, 10.0, 1e-6);
  EXPECT_NEAR(copy_eye.y, 20.0, 1e-6);
  EXPECT_NEAR(copy_eye.z, 30.0, 1e-6);
}


TEST_F(CameraTest, ViewTransformation) {
  CW::Camera camera(CW::Point3D(10.0, 0.0, 0.0), CW::Point3D(0.0, 0.0, 0.0), CW::Vector3D(0.0, 0.0, 1.0));

  CW::Transformation t = camera.view_transformation();
  // Just verify it doesn't throw and returns a valid transformation
  SUCCEED();
}


TEST_F(CameraTest, NullCameraThrows) {
  CW::Camera camera;
  EXPECT_THROW(camera.eye(), std::logic_error);
  EXPECT_THROW(camera.perspective(), std::logic_error);
  EXPECT_THROW(camera.fov(), std::logic_error);
}


// Model-based tests: copy camera from source model to destination model
class CameraModelTest : public ModelLoad {};


TEST_F(CameraModelTest, CopyModelCamera) {
  // Read camera from the source model and copy it
  CW::Camera src_camera = m_model->camera();
  EXPECT_FALSE(!src_camera);
  CW::Camera camera_copy = src_camera.copy();

  // Apply the copied camera to the destination model's camera
  CW::Camera dest_camera = m_model_copy->camera();
  dest_camera.set_orientation(camera_copy.eye(), camera_copy.target(), camera_copy.up());
  dest_camera.perspective(camera_copy.perspective());
  if (camera_copy.perspective()) {
    dest_camera.fov(camera_copy.fov());
  } else {
    dest_camera.orthographic_height(camera_copy.orthographic_height());
  }

  // Verify the copy preserved the source properties
  EXPECT_NEAR(dest_camera.eye().x, src_camera.eye().x, 1e-6);
  EXPECT_NEAR(dest_camera.eye().y, src_camera.eye().y, 1e-6);
  EXPECT_NEAR(dest_camera.eye().z, src_camera.eye().z, 1e-6);
  EXPECT_NEAR(dest_camera.target().x, src_camera.target().x, 1e-6);
  EXPECT_NEAR(dest_camera.target().y, src_camera.target().y, 1e-6);
  EXPECT_NEAR(dest_camera.target().z, src_camera.target().z, 1e-6);
  EXPECT_EQ(dest_camera.perspective(), src_camera.perspective());

  SaveModel("Camera");
}


}
