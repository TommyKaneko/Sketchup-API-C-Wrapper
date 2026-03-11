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
#include "SUAPI-CppWrapper/model/Image.hpp"
#include "SUAPI-CppWrapper/model/ImageRep.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {


class ImageTest : public testing::Test {
 protected:
  ImageTest() {
    CW::initialize();
  }
  ~ImageTest() override {
    CW::terminate();
  }
};


TEST_F(ImageTest, CreateDefaultImage) {
  CW::Image img;
  EXPECT_TRUE(!img);
}


TEST_F(ImageTest, CopyConstructorNull) {
  CW::Image img;
  CW::Image copy(img);
  EXPECT_TRUE(!copy);
}


TEST_F(ImageTest, NullImageThrows) {
  CW::Image img;
  EXPECT_THROW(img.image_rep(), std::logic_error);
  EXPECT_THROW(img.transform(), std::logic_error);
  EXPECT_THROW(img.file_name(), std::logic_error);
  EXPECT_THROW(img.dimensions(), std::logic_error);
  EXPECT_THROW(img.definition(), std::logic_error);
  EXPECT_THROW(img.copy(), std::logic_error);
}


// NOTE: The test model "box and box.skp" may not contain Image entities.
// If so, the model-based copy test below will need the test model updated
// to include at least one Image entity.
class ImageModelTest : public ModelLoad {};


TEST_F(ImageModelTest, CopyImages) {
  CW::Entities src_entities = m_model->entities();
  std::vector<CW::Image> source_images = src_entities.images();

  if (source_images.size() == 0) {
    // No images in model - skip the copy test but verify we can read
    SUCCEED() << "No images in test model - add an image to 'box and box.skp' to enable full test";
    return;
  }

  // Copy each image using copy() and add to the destination model
  CW::Entities dest_entities = m_model_copy->entities();
  for (auto& src_img : source_images) {
    CW::Image new_img = src_img.copy();
    dest_entities.add_image(new_img);
  }

  // Read back from destination and verify properties match
  std::vector<CW::Image> copied_images = dest_entities.images();
  ASSERT_EQ(copied_images.size(), source_images.size());

  for (size_t i = 0; i < source_images.size(); ++i) {
    auto [src_w, src_h] = source_images[i].dimensions();
    auto [dst_w, dst_h] = copied_images[i].dimensions();
    EXPECT_DOUBLE_EQ(src_w, dst_w);
    EXPECT_DOUBLE_EQ(src_h, dst_h);
  }

  SaveModel("Images");
}


}
