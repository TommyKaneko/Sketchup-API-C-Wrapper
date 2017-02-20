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
class ComponentDefinition;
class String;

class Group :public Entity {
	private:
  SUGroupRef m_group;
  
  /**
  * Create new SUGroupRef object
  */
  static SUGroupRef create_group();
	
  public:
  Group();
  Group(SUGroupRef group);
  
  /*
  * The class object can be converted to a SUGroupRef.
  */
  operator SUGroupRef() const;
  operator SUGroupRef*();
  
  /**
  * Return the ComponentDefinition of the Group.
  */
  ComponentDefinition definition() const;
  
  /**
  * Return the Entities object of the Group.
  */
  Entities entities() const;
  
  /**
  * Return the name of the Group.
  */
  String name() const;
  
  /**
  * Set the name of this Group.
  */
  void name(const String& string);
  
  /**
  * Return the Transformation object of the group.
  */
  Transformation transformation() const;

  /**
  * Set the Transformation of the group.
  */
  void transformation(const Transformation& transform);
  
  /**
  * Returns the raw SUGroupRef object.
  */
  SUGroupRef ref() const;
  
};

} /* namespace CW */

#endif /* Group_hpp */
