//
//  ComponentDefinition.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
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
#include <cassert>

#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"
#include "SUAPI-CppWrapper/model/Group.hpp"

namespace CW {

/******************************
** Private Static Methods **
*******************************/
SUComponentDefinitionRef ComponentDefinition::create_definition() {
  SUComponentDefinitionRef definition = SU_INVALID;
  SUResult res = SUComponentDefinitionCreate(&definition);
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
  SUResult res = SUComponentDefinitionGetEntities(new_definition, &new_entities_ref);
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
    SUResult res = SUComponentDefinitionRelease(&m_definition);
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


ComponentDefinition::operator SUComponentDefinitionRef() const {
  return m_definition;
}
ComponentDefinition::operator SUComponentDefinitionRef*() {
  return &m_definition;
}


BoundingBox3D ComponentDefinition::bounds() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::bounds(): ComponentDefinition is null");
  }
  // If this definition is empty, then the bounding box returned by SUDrawingElementGetBoundingBox has wild numbers.  So return min 0 and max 0.
  if (entities().size() == 0) {
    return BoundingBox3D(false);
  }
  SUBoundingBox3D box = SU_INVALID;
  SUResult res = SUDrawingElementGetBoundingBox(m_drawing_element, &box);
  assert(res == SU_ERROR_NONE);
  return BoundingBox3D(box);
}


ComponentInstance ComponentDefinition::create_instance() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::create_instance(): ComponentDefinition is null");
  }
  SUComponentInstanceRef instance = SU_INVALID;
  SUResult res = SUComponentDefinitionCreateInstance(m_definition, &instance);
  assert(res == SU_ERROR_NONE);
  return ComponentInstance(instance, false);
}

/*
Group ComponentDefinition::create_group() const {
  assert(this->is_group());
  SUComponentInstanceRef instance = SU_INVALID;
  SUResult res = SUComponentDefinitionCreateInstance(m_definition, &instance);
  assert(res == SU_ERROR_NONE);
  SUGroupRef group = SUGroupFromComponentInstance(instance);
  return Group(group, false);
}
*/


Entities ComponentDefinition::entities() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::entities(): ComponentDefinition is null");
  }
  SUEntitiesRef entities = SU_INVALID;
  SUResult res = SUComponentDefinitionGetEntities( m_definition, &entities);
  assert(res == SU_ERROR_NONE);
  return Entities(entities);
}


String ComponentDefinition::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::name(): ComponentDefinition is null");
  }
  SUStringRef name_string = SU_INVALID;
  SUStringCreate(&name_string);
  SUResult res = SUComponentDefinitionGetName(m_definition, &name_string);
  assert(res == SU_ERROR_NONE);
  return String(name_string);
}


bool ComponentDefinition::name(String name) {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::name(): ComponentDefinition is null");
  }
  SUResult res = SUComponentDefinitionSetName(m_definition, std::string(name).c_str());
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}
  

SUComponentDefinitionRef ComponentDefinition::ref() const {
  return m_definition;
}


bool ComponentDefinition::is_group() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::is_group(): ComponentDefinition is null");
  }
  SUComponentType type;
  SUResult res = SUComponentDefinitionGetType(m_definition, &type);
  assert(res == SU_ERROR_NONE);
  if (type == SUComponentType_Group) {
    return true;
  }
  else {
    return false;
  }
}


Behavior ComponentDefinition::behavior() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::behavior(): ComponentDefinition is null");
  }
  SUComponentBehavior behavior;
  SUResult res = SUComponentDefinitionGetBehavior(m_definition, &behavior);
  assert(res == SU_ERROR_NONE);
  return Behavior(behavior);
}

void ComponentDefinition::behavior(const Behavior& behavior) const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentDefinition::behavior(): ComponentDefinition is null");
  }
  SUComponentBehavior behavior_ref = behavior.ref();
  SUResult res = SUComponentDefinitionSetBehavior(m_definition, &behavior_ref);
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
