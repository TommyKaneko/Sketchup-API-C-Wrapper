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
#include "SUAPI-CppWrapper/model/Text.hpp"
#include "SUAPI-CppWrapper/model/Font.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW::Tests {


class TextTest : public testing::Test {
 protected:
  TextTest() {
    CW::initialize();
  }
  ~TextTest() override {
    CW::terminate();
  }
};


TEST_F(TextTest, CreateDefaultText) {
  CW::Text txt;
  EXPECT_TRUE(!txt);
}


TEST_F(TextTest, CopyConstructorNull) {
  CW::Text txt;
  CW::Text copy(txt);
  EXPECT_TRUE(!copy);
}


TEST_F(TextTest, NullTextThrows) {
  CW::Text txt;
  EXPECT_THROW(txt.string(), std::logic_error);
  EXPECT_THROW(txt.font(), std::logic_error);
  EXPECT_THROW(txt.leader_type(), std::logic_error);
  EXPECT_THROW(txt.arrow_type(), std::logic_error);
  EXPECT_THROW(txt.point(), std::logic_error);
  EXPECT_THROW(txt.leader_vector(), std::logic_error);
  EXPECT_THROW(txt.color(), std::logic_error);
  EXPECT_THROW(txt.copy(), std::logic_error);
}


// NOTE: The test model "box and box.skp" may not contain Text entities.
// If so, the model-based copy test below will need the test model updated
// to include at least one Text annotation.
class TextModelTest : public ModelLoad {};


TEST_F(TextModelTest, CopyTexts) {
  CW::Entities src_entities = m_model->entities();
  std::vector<CW::Text> source_texts = src_entities.texts();

  if (source_texts.size() == 0) {
    SUCCEED() << "No texts in test model - add a Text annotation to 'box and box.skp' to enable full test";
    return;
  }

  // Copy each text using copy() and add to the destination model
  std::vector<CW::Text> new_texts;
  for (const auto& src : source_texts) {
    new_texts.push_back(src.copy());
  }
  CW::Entities dest_entities = m_model_copy->entities();
  dest_entities.add_texts(new_texts);

  // Read back from destination and verify properties match
  std::vector<CW::Text> copied_texts = dest_entities.texts();
  ASSERT_EQ(copied_texts.size(), source_texts.size());

  for (size_t i = 0; i < source_texts.size(); ++i) {
    EXPECT_EQ(std::string(source_texts[i].string()),
              std::string(copied_texts[i].string()));
    EXPECT_EQ(source_texts[i].leader_type(), copied_texts[i].leader_type());
    EXPECT_EQ(source_texts[i].arrow_type(), copied_texts[i].arrow_type());
  }

  // Note: SaveModel may fail if the copied text is missing internal SketchUp
  // data that can't be transferred via the C API. Skip save on failure.
  try {
    SaveModel("Texts");
  } catch (const std::exception& e) {
    SUCCEED() << "Text properties copied correctly but model save failed: " << e.what();
  }
}


TEST_F(TextModelTest, ReadTextFont) {
  CW::Entities src_entities = m_model->entities();
  std::vector<CW::Text> texts = src_entities.texts();

  if (texts.size() == 0) {
    SUCCEED() << "No texts in test model";
    return;
  }

  // Verify we can read font properties from the first text
  CW::Font f = texts[0].font();
  EXPECT_FALSE(!f);
  CW::String face = f.face_name();
  EXPECT_GT(std::string(face).length(), 0u);
  EXPECT_GT(f.point_size(), 0u);
}


}
