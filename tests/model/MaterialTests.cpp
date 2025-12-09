#include "gtest/gtest.h"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Texture.hpp"
#include "SUAPI-CppWrapper/model/ImageRep.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW::Tests {




// Material Referencing Tests that references of a material point to the same object
TEST_F(ModelLoad, MaterialReferencing)
{
	using namespace CW;
  ASSERT_FALSE(!m_model);
  std::vector<Material> materials = m_model->materials();
  // The above operation should refer to the same materials
  for (const Material& mat : materials) {
    CW::Material material_ref = mat;
    // We should have the same reference
    EXPECT_EQ(material_ref.attached(), true);
    EXPECT_EQ(mat.attached(), true);
    EXPECT_EQ(material_ref.ref().ptr, mat.ref().ptr);
  }

  // Now check that copies of copies also have the same properties
  for (const Material& mat : materials) {
    // Test a copy
    CW::Material material_copy = mat.copy();
    EXPECT_NE(material_copy.ref().ptr, mat.ref().ptr); // material references must differ
    EXPECT_EQ(mat.attached(), true);
    EXPECT_EQ(material_copy.attached(), false);
    if (!!mat.texture()) {
      EXPECT_NE(material_copy.texture().ref().ptr, mat.texture().ref().ptr); // texture references must differ
      EXPECT_NE(material_copy.texture().image_rep().ref().ptr, mat.texture().image_rep().ref().ptr); // image_rep references must differ
    }
    // We should have the same properties
    MaterialsAreEqual(mat, material_copy);

    // Test copy of a copy:
    CW::Material material_copy_copy = material_copy.copy();
    EXPECT_NE(material_copy_copy.ref().ptr, material_copy.ref().ptr);
    EXPECT_EQ(material_copy_copy.attached(), false);
    MaterialsAreEqual(material_copy_copy, material_copy);
  }
}


// Material Referencing Tests that references of a material point to the same object
TEST_F(ModelLoad, MaterialImplicitCopying)
{
	using namespace CW;
  ASSERT_FALSE(!m_model);
  std::vector<Material> materials = m_model->materials();
  // The above operation should refer to the same materials
	m_model_copy->add_materials(materials);
  std::vector<Material> copied_materials = m_model_copy->materials();
  EXPECT_EQ(materials.size(), copied_materials.size());
  for (size_t i = 0; i < materials.size(); ++i) {
    const Material& original_mat = materials[i];
    const Material& copied_mat = copied_materials[i];
    // We should have different references
    EXPECT_NE(original_mat.ref().ptr, copied_mat.ref().ptr);
    EXPECT_TRUE(copied_mat.attached());
    if (!!copied_mat.texture()) {
      EXPECT_NE(copied_mat.texture().ref().ptr, original_mat.texture().ref().ptr); // texture references must differ
      EXPECT_NE(copied_mat.texture().image_rep().ref().ptr, original_mat.texture().image_rep().ref().ptr); // image_rep references must differ
    }
    // But the properties should be the same
    MaterialsAreEqual(original_mat, copied_mat);
  }
}


// Material Referencing Tests that references of a material point to the same object
TEST_F(ModelLoad, MaterialExplicitCopying)
{
	using namespace CW;
  ASSERT_FALSE(!m_model);
  std::vector<Material> materials = m_model->materials();
  // Check copies are new independent objects
  std::vector<Material> materials_copy; materials_copy.reserve(materials.size());
  for (const Material& mat : materials) {
    CW::Material material_copy = mat.copy();
    ASSERT_FALSE(material_copy.attached());
    // We should have different references
    EXPECT_NE(material_copy.ref().ptr, mat.ref().ptr);
    // But the properties should be the same
    MaterialsAreEqual(material_copy, mat);
    materials_copy.push_back(material_copy);
  }

  // The above operation should refer to the same materials
	m_model_copy->add_materials(materials_copy);
  std::vector<Material> copied_materials = m_model_copy->materials();
  EXPECT_EQ(materials.size(), copied_materials.size());
  for (Material& original_mat : materials) {
    const auto found_material = std::find_if(copied_materials.begin(), copied_materials.end(),
          [original_mat](const Material& value2){ return original_mat.name() == value2.name();});
    if (found_material != copied_materials.end()) {
      const Material& copied_mat = *found_material;
      // We should have different references
      EXPECT_NE(original_mat.ref().ptr, copied_mat.ref().ptr);
      if (!!copied_mat.texture()) {
        EXPECT_NE(copied_mat.texture().ref().ptr, original_mat.texture().ref().ptr); // texture references must differ
        EXPECT_NE(copied_mat.texture().image_rep().ref().ptr, original_mat.texture().image_rep().ref().ptr); // image_rep references must differ
      }
      // But the properties should be the same
      MaterialsAreEqual(original_mat, copied_mat);
      EXPECT_TRUE(copied_mat.attached());
    }
  }
}


}
