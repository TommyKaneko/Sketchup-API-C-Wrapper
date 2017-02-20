//
//  Material.hpp
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
