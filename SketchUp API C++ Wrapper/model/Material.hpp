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
class Color;
class Texture;

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
  * Returns a copy of the Material object, which is not attached to a model.
  */
  Material copy() const;
  
  /**
  * Returns the color of the material.
  */
  Color color() const;

  /**
  * Sets the color of the material.
  */
  void color(const Color& color);

  /**
  * Returns the name of the material.
  * @return string name of the material.
  */
  String name() const;
  
  /**
  * Sets the name of the material.
  * @param string name of the material.
  */
  void name(const String& string);

  /**
  * Returns the alpha value of the material.
  * @return opacity The alpha value within range [0.0, 1.0].
  */
  double opacity() const;
  
  /**
  * Sets the alpha value of the material.
  * @param alpha The alpha value within range [0.0, 1.0].
  */
  void opacity(const double alpha);
  
  /**
  * Returns the texture of the material
  */
  Texture texture() const;
  
  /**
  * Sets the texture of the material
  */
  void texture(const Texture& texture);
  
  /**
  * Returns the type of the material
  */
  SUMaterialType type() const;
  
  /**
  * Sets the texture of the material
  */
  void type(const SUMaterialType& material_type);
  
  /**
  * Returns whether the opacity value is used
  */
  bool use_alpha() const;

  /**
  * Sets whether the opacity value is used
  */
  void use_alpha(bool flag);
  
};

} /* namespace CW */

#endif /* Material_hpp */
