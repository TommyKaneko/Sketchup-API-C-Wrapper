//
//  Material.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include "Entity.hpp"

#include <SketchUpAPI/model/material.h>

namespace CW {

// Forward declarations
class String;

class Material :public Entity {
	private:
  SUMaterialRef m_material;
  
  static SUMaterialRef create_material();

  /**
  * Creates a SUMaterialRef derived from an existing Material object.
  * @param mat - Material object to derive the new SUMaterialRef object from
  * @return if the Material object is already attached to a model, its SUMaterialRef object will be returned. If the Material object has not been attached to a model, a new SUMaterialRef object will be created. Bear in mind all properties will not be copied in the latter case
  */
  static SUMaterialRef copy_reference(const Material& mat);
  
  public:
  Material();
  Material(SUMaterialRef material_ref, bool attached = true);
	
  /** Copy constructor */
  Material(const Material& other);
  
  /** Copy assigment operator */
  Material& operator=(const Material& other);
  
  /**
  * Destructor
  */
  ~Material();

  
  SUMaterialRef ref() const;
  
  operator SUMaterialRef() const;
  operator SUMaterialRef*();
  
  /**
  * Returns true if this is an invalid object, or if no material is set.  An entitity with the default material would return false.
  */
  bool operator !() const;
  
  /**
  * Returns the name of the material.
  * @return string name of the material.
  */
  String name() const;
  
};

} /* namespace CW */

#endif /* Material_hpp */
