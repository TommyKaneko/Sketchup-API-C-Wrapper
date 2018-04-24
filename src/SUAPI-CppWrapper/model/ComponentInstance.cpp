//
//  ComponentInstance.cpp
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

#include <cassert>

#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"

#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/**************************
* Private static methods **
***************************/

SUComponentInstanceRef ComponentInstance::copy_reference(const ComponentInstance& other) {
  if (other.m_attached) {
    return other.ref();
  }
  // The other face has not been attached to the model, so copy its properties to a new object
  ComponentInstance new_instance = other.definition().create_instance();
  new_instance.transformation(other.transformation());
  new_instance.name(other.name());
  return new_instance.ref();
}

/*****************************
* Constructors / Destructor **
******************************/
ComponentInstance::ComponentInstance():
  DrawingElement()
{}


ComponentInstance::ComponentInstance(SUComponentInstanceRef instance, bool attached):
  DrawingElement(SUComponentInstanceToDrawingElement(instance), attached)
{}


ComponentInstance::ComponentInstance(const ComponentInstance& other):
  DrawingElement(other, SUComponentInstanceToDrawingElement(copy_reference(other)))
{}


ComponentInstance::ComponentInstance(const ComponentInstance& other, SUComponentInstanceRef instance_ref):
  DrawingElement(other, SUComponentInstanceToDrawingElement(instance_ref))
{}


ComponentInstance::~ComponentInstance() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUComponentInstanceRef instance = SUComponentInstanceFromEntity(m_entity);
    SUResult res = SUComponentInstanceRelease(&instance);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}

/******************
* Public Methods **
*******************/
/** Copy assignment operator */
ComponentInstance& ComponentInstance::operator=(const ComponentInstance& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUComponentInstanceRef instance = SUComponentInstanceFromEntity(m_entity);
    SUResult res = SUComponentInstanceRelease(&instance);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUComponentInstanceToEntity(copy_reference(other));
  DrawingElement::operator=(other);
  return *this;
}


SUComponentInstanceRef ComponentInstance::ref() const {
  return SUComponentInstanceFromEntity(m_entity);
}


ComponentInstance::operator SUComponentInstanceRef() const {
  return this->ref();
}


Transformation ComponentInstance::transformation() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::transformation(): ComponentInstance is null");
  }
  SUTransformation transform;
  SUResult res = SUComponentInstanceGetTransform(this->ref(), &transform);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Transformation(transform);
}


void ComponentInstance::transformation(const Transformation& transform) {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::transformation(): ComponentInstance is null");
  }
  SUTransformation su_transform = transform.ref();
  SUResult res = SUComponentInstanceSetTransform(this->ref(), &su_transform);
  assert(res == SU_ERROR_NONE); _unused(res);
}


ComponentDefinition ComponentInstance::definition() const{
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::definition(): ComponentInstance is null");
  }
  SUComponentDefinitionRef component = SU_INVALID;
  SUResult res = SUComponentInstanceGetDefinition(this->ref(), &component);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ComponentDefinition(component);
}


String ComponentInstance::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::name(): ComponentInstance is null");
  }
  String string;
  SUStringRef * const string_ref = string;
  SUResult res = SUComponentInstanceGetName(this->ref(), string_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


void ComponentInstance::name(const String& string) {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::name(): ComponentInstance is null");
  }
  std::string name_string = string.std_string();
  SUResult res = SUComponentInstanceSetName(this->ref(), name_string.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


} /* namespace CW */
