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

#include <cassert>

#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"

#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/**************************
* Private static methods **
***************************/

SUComponentInstanceRef ComponentInstance::copy_reference(const ComponentInstance& other) {
  if (other.m_attached) {
    return other.m_instance;
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
  DrawingElement(),
  m_instance(SU_INVALID)
{}


ComponentInstance::ComponentInstance(SUComponentInstanceRef instance, bool attached):
  DrawingElement(SUComponentInstanceToDrawingElement(instance), attached),
  m_instance(instance)
{}


ComponentInstance::ComponentInstance(const ComponentInstance& other):
  DrawingElement(other, SUComponentInstanceToDrawingElement(copy_reference(other))),
  m_instance(SUComponentInstanceFromDrawingElement(m_drawing_element))
{}


ComponentInstance::ComponentInstance(const ComponentInstance& other, SUComponentInstanceRef instance_ref):
  DrawingElement(other, SUComponentInstanceToDrawingElement(instance_ref)),
  m_instance(instance_ref)
{}


ComponentInstance::~ComponentInstance() {
  if (!m_attached && SUIsValid(m_instance)) {
    SUResult res = SUComponentInstanceRelease(&m_instance);
    assert(res == SU_ERROR_NONE);
  }
}

/******************
* Public Methods **
*******************/
/** Copy assignment operator */
ComponentInstance& ComponentInstance::operator=(const ComponentInstance& other) {
  if (!m_attached && SUIsValid(m_instance)) {
    SUResult res = SUComponentInstanceRelease(&m_instance);
    assert(res == SU_ERROR_NONE);
  }
  m_instance = copy_reference(other);
  m_drawing_element = SUComponentInstanceToDrawingElement(m_instance);
  DrawingElement::operator=(other);
  return *this;
}



SUComponentInstanceRef ComponentInstance::ref() {
  return m_instance;
}

ComponentInstance::operator SUComponentInstanceRef() const {
  return m_instance;
}
ComponentInstance::operator SUComponentInstanceRef*() {
  return &m_instance;
}

Transformation ComponentInstance::transformation() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::transformation(): ComponentInstance is null");
  }
  SUTransformation transform;
  SUResult res = SUComponentInstanceGetTransform(m_instance, &transform);
  assert(res == SU_ERROR_NONE);
  return Transformation(transform);
}


void ComponentInstance::transformation(const Transformation& transform) {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::transformation(): ComponentInstance is null");
  }
  SUTransformation su_transform = transform.ref();
  SUResult res = SUComponentInstanceSetTransform(m_instance, &su_transform);
  assert(res == SU_ERROR_NONE);
}


ComponentDefinition ComponentInstance::definition() const{
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::definition(): ComponentInstance is null");
  }
  SUComponentDefinitionRef component = SU_INVALID;
  SUResult res = SUComponentInstanceGetDefinition(m_instance, &component);
  assert(res == SU_ERROR_NONE);
  return ComponentDefinition(component);
}


String ComponentInstance::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::name(): ComponentInstance is null");
  }
  String string;
  SUStringRef * const string_ref = string;
  SUResult res = SUComponentInstanceGetName(m_instance, string_ref);
  assert(res == SU_ERROR_NONE);
  return string;
}


void ComponentInstance::name(const String& string) {
  if (!(*this)) {
    throw std::logic_error("CW::ComponentInstance::name(): ComponentInstance is null");
  }
  std::string name_string = string.std_string();
  SUResult res = SUComponentInstanceSetName(m_instance, name_string.c_str());
  assert(res == SU_ERROR_NONE);
}


} /* namespace CW */
