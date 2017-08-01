//
//  ComponentDefinition.hpp
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

#ifndef ComponentDefinition_hpp
#define ComponentDefinition_hpp

#include <stdio.h>

#include <SketchUpAPI/model/component_definition.h>

#include "DrawingElement.hpp"

namespace CW {

// Forward declarations:
class Entities;
class ComponentInstance;
class Group;
class Behavior;
class String;
class Model;

/**
* This class represents a component definition.
*/
class ComponentDefinition :public DrawingElement {
	private:
  SUComponentDefinitionRef m_definition;
  
  static SUComponentDefinitionRef create_definition();
	
  /**
  * Copies a ComponentDefinition object into a new SUComponentDefinitionRef.  If the other ComponentDefinition object has been attached to a model already, it will return the same SUComponentDefinitionRef object that it points to.
  */
  static SUComponentDefinitionRef copy_reference(const ComponentDefinition& other);
  
  public:
  /**
  * Constructor creates empty Component Definition, ready to add to a model.
  */
  ComponentDefinition();
  
  /**
  * Constructor creates Component Definition from existing SUComponentDefinitionRef.
  */
  ComponentDefinition(SUComponentDefinitionRef definition, bool attached = true);
  
  /**
  * Copy constructor.
  */
  ComponentDefinition(const ComponentDefinition& other);
  
  /**
  * Destructor will release the definition object if it had not been added to a model.
  */
  ~ComponentDefinition();
  
  /** Copy assignment operator */
  ComponentDefinition& operator=(const ComponentDefinition& other);
  
  /** Cast to native objects */
  operator SUComponentDefinitionRef() const;
  operator SUComponentDefinitionRef*();
  
  /**
  * Returns the bounding box of this Component Definition
  */
  BoundingBox3D bounds() const;
  
  /**
  * Creates a new ComponentInstance from this ComponentDefinition.
  */
  ComponentInstance create_instance() const;

  /**
  * Creates a new Group from this ComponentDefinition. If the definition is not a Group definition, the method will fail. TODO: requires testing
  *
  * This method is not possible to implement with the current API (2017)
  */
  //Group create_group() const;
  
  /**
  * Gets the entities in the definition.
  */
  Entities entities() const;
  
  /**
  * Returns the SUComponentDefinitionRef that this class wraps.
  */
  SUComponentDefinitionRef ref() const;

  /**
  * Gets the name of the component.
  */
  String name() const;
  
  /**
  * Sets the name of the component.
  */
  bool name(String name);
  
  /**
  * Returns whether this definition is of a group.
  */
  bool is_group() const;
  
  /**
  * Returns the Behavior object of this component.
  */
  Behavior behavior() const;
  void behavior(const Behavior& behavior) const;
  
  /**
  * Returns the instances of this ComponentDefinition. TODO: Not possible
  */
  //std::vector<ComponentInstance> instances() const;

};
  
class Behavior {
private:
  SUComponentBehavior m_behavior;
  
public:
  Behavior(SUComponentBehavior behavior);
  
  SUComponentBehavior::SUSnapToBehavior &snap = m_behavior.component_snap;

  SUComponentBehavior ref() const;
  
  bool &cuts_opening = m_behavior.component_cuts_opening;
  
  bool &always_face_camera = m_behavior.component_always_face_camera;
  
  bool &shadows_face_sun = m_behavior.component_shadows_face_sun;
  
  size_t &no_scale_mask = m_behavior.component_no_scale_mask;
};

} /* namespace CW */
#endif /* ComponentDefinition_hpp */
