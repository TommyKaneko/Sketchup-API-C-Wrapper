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

/******************************
** Private Static Methods **
*******************************/
SUComponentDefinitionRef ComponentDefinition::create_definition() {
  SUComponentDefinitionRef definition = SU_INVALID;
	SU_RESULT res = SUComponentDefinitionCreate(&definition);
  assert(res == SU_ERROR_NONE);
  return definition;
}

SUComponentDefinitionRef ComponentDefinition::copy_reference(const ComponentDefinition& other) {
	if (other.m_attached) {
  	return other.m_definition;
  }
  // The other definition has not been attached to the model, so copy its properties to a new object
  SUComponentDefinitionRef new_definition = create_definition();
  // Copy across all nested geometry
  SUEntitiesRef new_entities_ref = SU_INVALID;
  SU_RESULT res = SUComponentDefinitionGetEntities(new_definition, &new_entities_ref);
  assert(res == SU_ERROR_NONE);
  Entities new_entities(new_entities_ref);
	new_entities.add(other.entities());
  return new_definition;
}


/******************************
** Constructors / Destructor **
*******************************/
ComponentDefinition::ComponentDefinition():
  ComponentDefinition(create_definition(), true)
{
}


ComponentDefinition::ComponentDefinition(SUComponentDefinitionRef definition, bool attached):
  DrawingElement(SUComponentDefinitionToDrawingElement(definition), attached),
  m_definition(definition)
{}


/**
* Copy constructor.
*/
ComponentDefinition::ComponentDefinition(const ComponentDefinition& other):
  DrawingElement(SUComponentDefinitionToDrawingElement(copy_reference(other))),
  m_definition(SUComponentDefinitionFromDrawingElement(m_drawing_element))
{}


ComponentDefinition::~ComponentDefinition() {
  if (!m_attached && SUIsValid(m_definition)) {
    SUComponentDefinitionRelease(&m_definition);
  }
}


/**
* Copy assignment operator.
*/
ComponentDefinition& ComponentDefinition::operator=(const ComponentDefinition& other) {
  if (m_attached && SUIsValid(m_definition)) {
    SU_RESULT res = SUComponentDefinitionRelease(&m_definition);
    assert(res == SU_ERROR_NONE);
  }
  m_definition = copy_reference(other);
  m_drawing_element = SUComponentDefinitionToDrawingElement(m_definition);
  DrawingElement::operator=(other);
  
  // Copy across all ComponentDefinition properties
  this->name(other.name());
  this->behavior(other.behavior());
  return *this;
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

void ComponentDefinition::behavior(const Behavior& behavior) const {
  SUComponentBehavior behavior_ref = behavior.ref();
  SU_RESULT res = SUComponentDefinitionSetBehavior(m_definition, &behavior_ref);
  assert(res == SU_ERROR_NONE);
}
  
/****************
* Behavior class
*****************/

Behavior::Behavior(SUComponentBehavior behavior):
m_behavior(behavior)
{
}

SUComponentBehavior Behavior::ref() const {
	return m_behavior;
}


} /* namespace CW */