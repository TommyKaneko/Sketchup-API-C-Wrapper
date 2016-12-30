//
//  ComponentInstance.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef ComponentInstance_hpp
#define ComponentInstance_hpp

#include <stdio.h>

#include <SketchUpAPI/model/component_instance.h>

#include "Entity.hpp"
#include "Transformation.hpp"
#include "ComponentDefinition.hpp"

namespace CW {

class ComponentInstance :public DrawingElement {
	private:
  SUComponentInstanceRef m_instance;
	
  public:
  ComponentInstance(SUComponentInstanceRef instance);
  
  /**
  * Returns the raw SUComponentInstance object.
  */
  SUComponentInstanceRef ref();
  
  /**
  * Returns the Transformation object applied to this instance.
  */
  Transformation transformation();

  /**
  * Returns the Component Definition object of this instance.
  */
  ComponentDefinition definition();
};

} /* namespace CW */
#endif /* ComponentInstance_hpp */
