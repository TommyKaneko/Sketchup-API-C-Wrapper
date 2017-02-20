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

#include "DrawingElement.hpp"

namespace CW {

// Forward declarations:
class Entities;
class ComponentInstance;
class Behavior;
class String;

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
  
  /**
  * Copy assignment operator.
  */
  ComponentDefinition& operator=(const ComponentDefinition& other);
  
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
