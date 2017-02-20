//
//  ComponentInstance.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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