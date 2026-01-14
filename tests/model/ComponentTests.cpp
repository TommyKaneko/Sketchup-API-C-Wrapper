#include "gtest/gtest.h"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"
#include "SUAPI-CppWrapper/model/GeometryInput.hpp"

namespace CW::Tests {

// std::vector<ComponentDefinition> get_nested_definitions(const Entities& src_entities, dest_entitiesstd::vector<ComponentDefinition> &def_map) {
//   std::vector<ComponentDefinition> nested_defs;
//   std::vector<ComponentInstance> instances = def_entities.instances();
//   for (const ComponentInstance& inst : instances) {
//     ComponentDefinition nested_def = inst.definition();
//     nested_defs.push_back(nested_def);
//     // Recursively get nested definitions
//     std::vector<ComponentDefinition> deeper_defs = get_nested_definitions(nested_def.entities());
//     nested_defs.insert(nested_defs.end(), deeper_defs.begin(), deeper_defs.end());
//   }
//   // remove any duplicates
//   std::sort(nested_defs.begin(), nested_defs.end(), [](const ComponentDefinition& a, const ComponentDefinition& b) {
//     return a.ref().ptr < b.ref().ptr;
//   });
//   nested_defs.erase(std::unique(nested_defs.begin(), nested_defs.end(), [](const ComponentDefinition& a, const ComponentDefinition& b) {
//     return a.ref().ptr == b.ref().ptr;
//   }), nested_defs.end());
//   return nested_defs;
// }

void copy_nested_components(const Entities& src_entities, Entities& dest_entities, Model* dest_model,
  std::unordered_map<ComponentDefinition, ComponentDefinition> &def_map) {
  // add simple geometry first
  GeometryInput geom_input(dest_model);
  geom_input.add_entities(src_entities);
  dest_entities.fill(geom_input);
    // add definitions and then instances
  std::vector<ComponentInstance> src_instances = src_entities.instances();
  for (const ComponentInstance& src_instance : src_instances) {
    ComponentDefinition src_def = src_instance.definition();
    // Check if definition already copied
    std::unordered_map<ComponentDefinition, ComponentDefinition>::iterator it = def_map.find(src_def);
    if (it == def_map.end()) {
      // Create a new definition in the target model
      ComponentDefinition def_copy;
      dest_model->add_definition(def_copy);
      assert(def_copy.attached() == true);
      def_copy.name(src_def.name());
      def_copy.behavior(src_def.behavior());
      Entities def_entities = def_copy.entities();
      // Recursively copy nested definitions
      copy_nested_components(src_def.entities(), def_entities, dest_model, def_map);
      def_map.emplace(src_def, def_copy);
      dest_entities.add_instance(def_copy, src_instance.transformation(), src_instance.name());
    }
    else {
      // Definition already copied, simply add instance
      dest_entities.add_instance(it->second, src_instance.transformation(), src_instance.name());
    }
  }
}


// Implicit copying - add source instances into an empty model's entities
TEST_F(ModelLoad, ComponentImplicitCopying)
{
  using namespace CW;

  Entities src_entities = m_model->entities();
  std::vector<ComponentInstance> instances = src_entities.instances();
  ASSERT_GT(instances.size(), (size_t)0);
  Entities dest_entities = m_model_copy->entities();
  std::unordered_map<ComponentDefinition, ComponentDefinition> def_map; // from source to target definitions
  copy_nested_components(src_entities, dest_entities, m_model_copy, def_map);

  std::vector<ComponentInstance> copied_instances = m_model_copy->entities().instances();
  EXPECT_EQ(copied_instances.size(), instances.size());

  // Compare instances one-by-one
  for (size_t i = 0; i < instances.size(); ++i) {
    // Must be different native objects
    EXPECT_NE(copied_instances[i].ref().ptr, instances[i].ref().ptr);
    ASSERT_NO_FATAL_FAILURE(ComponentsAreEqual(copied_instances[i], instances[i]));
  }
}

} // namespace CW::Tests
