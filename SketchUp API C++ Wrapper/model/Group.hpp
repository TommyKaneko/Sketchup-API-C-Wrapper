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

class Group :public Entity {
	private:
  SUGroupRef m_group;
	
  public:
  Group(SUGroupRef group);
  
};

} /* namespace CW */

#endif /* Group_hpp */
