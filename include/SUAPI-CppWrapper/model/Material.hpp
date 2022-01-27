//
//  Material.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include "SUAPI-CppWrapper/model/Entity.hpp"

#include <SketchUpAPI/model/material.h>

namespace CW {

// Forward declarations
class String;
class Color;
class Texture;

class Material :public Entity {
  private:
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
  * Returns the internal name of the material.  The internal name is the unprocessed identifier string stored with the material.
  * @return string name of the material.
  */
  String name() const;
  
  /**
  * Sets the name of the material.
  * @param string name of the material.
  */
  void name(const String& string);
  
  /**
  * Retrieves the name of a material object. This method was added for users who require the functionality of Material.name() prior to SketchUp 2017, API 5.0. If the internal name is encased in square brackets, [], this method will return the name without brackets, otherwise the name will match the name retrieved by SUMaterialGetName.
  * @since SketchUp 2017, API 5.0
  * @return string name of the material.
  */
  String display_name() const;

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
  
  /**
  * Hash function for use with unordered_map
  */
  friend std::hash<CW::Material>;
};

} /* namespace CW */

namespace std {
  template <> struct hash<CW::Material>
  {
    size_t operator()(const CW::Material& k) const
    {
      static const size_t shift = static_cast<size_t>(log2(1 + sizeof(CW::Material)));
      return reinterpret_cast<size_t>(k.m_entity.ptr) >> shift;
    }
  };

}

#endif /* Material_hpp */
