//
//  ComponentInstance.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include <cassert>

#include "ComponentInstance.hpp"

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

} /* namespace CW */