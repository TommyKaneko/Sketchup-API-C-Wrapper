//
//  Group.hpp
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
