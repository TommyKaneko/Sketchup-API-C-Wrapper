//
//  Group.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>

#include <SketchUpAPI/model/group.h>

#include "Entity.hpp"

namespace CW {

// Forward Declarations
class Entities;
class Transformation;

class Group :public Entity {
	private:
  SUGroupRef m_group;
	
  public:
  Group();
  Group(SUGroupRef group);
  
  /**
  * Return the Entities object of the Group.
  */
  Entities entities() const;
  
  /**
  * Return the Transformation object of the group.
  */
  Transformation transformation() const;
  
};

} /* namespace CW */

#endif /* Group_hpp */
