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

#include "ComponentInstance.hpp"

#include "String.hpp"

namespace CW {


ComponentInstance::ComponentInstance(SUComponentInstanceRef instance):
  DrawingElement(SUComponentInstanceToDrawingElement(instance)),
  m_instance(instance)
{
}


SUComponentInstanceRef ComponentInstance::ref() {
	return m_instance;
}

Transformation ComponentInstance::transformation() {
  SUTransformation transform;
  SU_RESULT res = SUComponentInstanceGetTransform(m_instance, &transform);
  assert(res == SU_ERROR_NONE);
  return Transformation(transform);
}


ComponentDefinition ComponentInstance::definition() {
  SUComponentDefinitionRef component = SU_INVALID;
  SU_RESULT res = SUComponentInstanceGetDefinition(m_instance, &component);
  assert(res == SU_ERROR_NONE);
  return ComponentDefinition(component);
}


String ComponentInstance::name() const {
  String string;
  SUStringRef * const string_ref = string;
	SU_RESULT res = SUComponentInstanceGetName(m_instance, string_ref);
  assert(res == SU_ERROR_NONE);
  return string;
}


void ComponentInstance::name(const String& string) {
	std::string name_string = string.std_string();
	SU_RESULT res = SUComponentInstanceSetName(m_instance, name_string.c_str());
  assert(res == SU_ERROR_NONE);
}


} /* namespace CW */