//
//  ComponentDefinition.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 17/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//
#include <cassert>

#include "ComponentDefinition.hpp"
#include "Entities.hpp"

namespace CW {


ComponentDefinition::ComponentDefinition():
  ComponentDefinition(create_definition(), true)
{
}


ComponentDefinition::ComponentDefinition(SUComponentDefinitionRef definition, bool release_on_destroy):
  DrawingElement(SUComponentDefinitionToDrawingElement(definition)),
  m_definition(definition),
  m_release_on_destroy(release_on_destroy)
{
}
  

ComponentDefinition::~ComponentDefinition() {
  if (m_release_on_destroy) {
    SUComponentDefinitionRelease(&m_definition);
  }
}


/**
* Static Function
*/
SUComponentDefinitionRef ComponentDefinition::create_definition() {
  SUComponentDefinitionRef definition = SU_INVALID;
  return definition;
}


Entities ComponentDefinition::entities() const {
  SUEntitiesRef entities = SU_INVALID;
  SU_RESULT res = SUComponentDefinitionGetEntities( m_definition, &entities);
  assert(res == SU_ERROR_NONE);
  return Entities(entities);
}


String ComponentDefinition::name() const {
  SUStringRef name_string;
  SU_RESULT res = SUComponentDefinitionGetName(m_definition, &name_string);
  assert(res == SU_ERROR_NONE);
  return String(name_string);
}


bool ComponentDefinition::name(String name) {
  SU_RESULT res = SUComponentDefinitionSetName(m_definition, std::string(name).c_str());
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}
  

SUComponentDefinitionRef ComponentDefinition::ref() const {
  return m_definition;
}


bool ComponentDefinition::is_group() const {
  SUComponentType type;
  SU_RESULT res = SUComponentDefinitionGetType(m_definition, &type);
  assert(res == SU_ERROR_NONE);
  if (type == SUComponentType_Group) {
    return true;
  }
  else {
    return false;
  }
}


Behavior ComponentDefinition::behavior() const {
  SUComponentBehavior behavior;
  SU_RESULT res = SUComponentDefinitionGetBehavior(m_definition, &behavior);
  assert(res == SU_ERROR_NONE);
  return Behavior(behavior);
}

  
/****************
* Behavior class
*****************/

Behavior::Behavior(SUComponentBehavior behavior):
m_behavior(behavior)
{
}

} /* namespace CW */