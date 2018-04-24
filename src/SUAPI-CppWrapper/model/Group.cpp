//
//  Group.cpp
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Group.hpp"

#include <cassert>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"
#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"


namespace CW {

/**************************
* Private static methods **
***************************/
SUGroupRef Group::create_group() {
  SUGroupRef group_ref = SU_INVALID;
  SUResult res = SUGroupCreate(&group_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return group_ref;
}


SUGroupRef Group::copy_reference(const Group& other) {
  if (other.m_attached) {
    return other.ref();
  }
  // The other group has not been attached to the model, so copy its properties to a new object
  Group new_group;
  new_group.transformation(other.transformation());
  new_group.name(other.name());
  new_group.entities().add(other.entities());
  return new_group.ref();
}


Group::Group():
  Group(create_group(), false)
{}


Group::Group(SUGroupRef group, bool attached):
  ComponentInstance(SUGroupToComponentInstance(group), attached)
{}


/** Copy constructor */
Group::Group(const Group& other):
  ComponentInstance(other, SUGroupToComponentInstance(copy_reference(other)))
{}


Group::~Group() {
  if (!m_attached && SUIsValid(m_entity)) {
    // There is no release function - we assume that the parent class ComponentInstance takes care of releasing.
  }
}


Group& Group::operator=(const Group& other) {
  // SUGroupRef does not have a release function, so we rely on the component instance to do that for us.
  ComponentInstance::operator=(other);
  // Groups have nothing else to copy.
  return *this;
}


SUGroupRef Group::ref() const {
  return SUGroupFromEntity(m_entity);
}


Group::operator SUGroupRef() const {
  return ref();
}


ComponentDefinition Group::definition() const {
  if(!(*this)) {
    throw std::logic_error("CW::Group::definition(): Group is null");
  }
  SUComponentDefinitionRef def_ref = SU_INVALID;
  SUResult res = SUGroupGetDefinition(this->ref(), &def_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ComponentDefinition(def_ref);
}


Entities Group::entities() const {
  if(!(*this)) {
    throw std::logic_error("CW::Group::entities(): Group is null");
  }
  SUEntitiesRef entities = SU_INVALID;
  SUGroupGetEntities(this->ref(), &entities);
  return Entities(entities);
}


String Group::name() const {
  if(!(*this)) {
    throw std::logic_error("CW::Group::name(): Group is null");
  }
  String string;
  SUStringRef * const string_ref = string;
  SUResult res = SUGroupGetName(this->ref(), string_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


void Group::name(const String& string) {
  if(!(*this)) {
    throw std::logic_error("CW::Group::name(): Group is null");
  }
  std::string name_string = string.std_string();
  SUResult res = SUGroupSetName(this->ref(), name_string.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


Transformation Group::transformation() const {
  if(!(*this)) {
    throw std::logic_error("CW::Group::transformation(): Group is null");
  }
  SUTransformation transform{};
  SUGroupGetTransform(this->ref(), &transform);
  return Transformation(transform);
}


void Group::transformation(const Transformation& transform) {
  if(!(*this)) {
    throw std::logic_error("CW::Group::transformation(): Group is null");
  }
  SUTransformation transform_ref = transform.ref();
  SUResult res = SUGroupSetTransform(this->ref(), &transform_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
}

} /* namespace CW */
