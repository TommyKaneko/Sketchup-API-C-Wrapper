//
//  ComponentInstance.cpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
ComponentInstance::ComponentInstance(SUComponentInstanceRef instance, bool attached):
  DrawingElement(SUComponentInstanceToDrawingElement(instance), attached),
  m_instance(instance)
{
}


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
  	SU_RESULT res = SUComponentInstanceRelease(&m_instance);
		assert(res == SU_ERROR_NONE);
  }
}

/******************
* Public Methods **
*******************/
/** Copy assignment operator */
ComponentInstance& ComponentInstance::operator=(const ComponentInstance& other) {
  if (!m_attached && SUIsValid(m_instance)) {
    SU_RESULT res = SUComponentInstanceRelease(&m_instance);
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
  SU_RESULT res = SUComponentInstanceGetTransform(m_instance, &transform);
  assert(res == SU_ERROR_NONE);
  return Transformation(transform);
}


void ComponentInstance::transformation(const Transformation& transform) {
  if (!(*this)) {
  	throw std::logic_error("CW::ComponentInstance::transformation(): ComponentInstance is null");
  }
	SUTransformation su_transform = transform.ref();
  SU_RESULT res = SUComponentInstanceSetTransform(m_instance, &su_transform);
  assert(res == SU_ERROR_NONE);
}


ComponentDefinition ComponentInstance::definition() const{
  if (!(*this)) {
  	throw std::logic_error("CW::ComponentInstance::definition(): ComponentInstance is null");
  }
  SUComponentDefinitionRef component = SU_INVALID;
  SU_RESULT res = SUComponentInstanceGetDefinition(m_instance, &component);
  assert(res == SU_ERROR_NONE);
  return ComponentDefinition(component);
}


String ComponentInstance::name() const {
  if (!(*this)) {
  	throw std::logic_error("CW::ComponentInstance::name(): ComponentInstance is null");
  }
  String string;
  SUStringRef * const string_ref = string;
	SU_RESULT res = SUComponentInstanceGetName(m_instance, string_ref);
  assert(res == SU_ERROR_NONE);
  return string;
}


void ComponentInstance::name(const String& string) {
  if (!(*this)) {
  	throw std::logic_error("CW::ComponentInstance::name(): ComponentInstance is null");
  }
	std::string name_string = string.std_string();
	SU_RESULT res = SUComponentInstanceSetName(m_instance, name_string.c_str());
  assert(res == SU_ERROR_NONE);
}


} /* namespace CW */
