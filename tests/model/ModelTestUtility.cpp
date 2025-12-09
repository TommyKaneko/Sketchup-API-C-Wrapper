#include "ModelTestUtility.hpp"
#include "ModelPath.h"

#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Texture.hpp"
#include "SUAPI-CppWrapper/model/ImageRep.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
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

void ModelLoad::MaterialsAreEqual(const CW::Material& mat1, const CW::Material& mat2){
  if (!mat1.is_valid()) {
      if (!mat2.is_valid()) {
        return;
      }
      throw std::invalid_argument("One Material is invalid and the other is not");
  }
  EXPECT_EQ(mat1.name(), mat2.name());
  EXPECT_EQ(mat1.opacity(), mat2.opacity());
  EXPECT_EQ(mat1.type(), mat2.type());
  EXPECT_EQ(mat1.use_alpha(), mat2.use_alpha());
  TexturesAreEqual(mat1.texture(), mat2.texture());
}


void ModelLoad::TexturesAreEqual(const CW::Texture& tex1, const CW::Texture& tex2){
  if (!tex1.is_valid()) {
      if (!tex2.is_valid()) {
        return;
      }
      throw std::invalid_argument("One Texture is invalid and the other is not");
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
  if (!image1) {
      if (!image2) {
        return;
      }
      throw std::invalid_argument("One ImageRep is invalid and the other is not");
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
  if (!layer1.is_valid()) {
      if (!layer2.is_valid()) {
        return;
      }
      throw std::invalid_argument("One Layer is invalid and the other is not");
  }
  EXPECT_EQ(layer1.name(), layer2.name());
  // TODO: add more properties as they are added to the API.
}

  // Code here will be called immediately after the constructor (right
  // before each test).

} // namespace CW::Tests
