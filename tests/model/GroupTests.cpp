#include "gtest/gtest.h"

#include "ModelTestUtility.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/Group.hpp"

namespace CW::Tests {

// Implicit copying - add source entities into an empty model's entities
TEST_F(ModelLoad, GroupCopying)
{
  using namespace CW;

  Entities src_entities = m_model->entities();
  std::vector<Group> groups = src_entities.groups();
  ASSERT_GT(groups.size(), (size_t)0);
  // Add all entities (including groups) from source into the empty copy
  Entities dest_entities = m_model_copy->entities();
  for (CW::Group &group : groups) {
    Group new_group = dest_entities.add_group(group.definition(), group.transformation());
    new_group.name(group.name());
    // TODO: apply materials and layers to new group
  }
  std::vector<Group> copied_groups = m_model_copy->entities().groups();

  SaveModel("GroupImplicitCopying");
  EXPECT_EQ(copied_groups.size(), groups.size());

  for (size_t i = 0; i < groups.size(); ++i) {
    // Must be different native objects
    EXPECT_NE(groups[i].ref().ptr, copied_groups[i].ref().ptr);
    ASSERT_NO_FATAL_FAILURE(GroupsAreEqual(groups[i], copied_groups[i]));
  }
}


} // namespace CW::Tests
