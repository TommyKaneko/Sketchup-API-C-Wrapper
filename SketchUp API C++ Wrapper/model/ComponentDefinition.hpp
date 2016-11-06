//
//  ComponentDefinition.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 17/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef ComponentDefinition_hpp
#define ComponentDefinition_hpp

#include <stdio.h>

#include <SketchUpAPI/model/component_definition.h>

#include "Entity.hpp"

namespace CW {

class ComponentDefinition :public Entity {
	private:
  SUComponentDefinitionRef m_definition;
  
  public:
  ComponentDefinition(SUComponentDefinitionRef definition);
  
};

} /* namespace CW */
#endif /* ComponentDefinition_hpp */
