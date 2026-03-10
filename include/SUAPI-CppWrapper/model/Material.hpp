//
//  Material.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
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

#include "SUAPI-CppWrapper/model/Entity.hpp"

#include <SketchUpAPI/model/material.h>

namespace CW {

// Forward declarations
class String;
class Color;
class Texture;

/**
 * @brief Wrapper for SUMaterialRef.
 *
 * Represents a material that can be applied to drawing elements. A material
 * can have a color, texture, opacity, and type (colored, textured, or
 * colorized texture).
 *
 * @see SUMaterialRef in the SketchUp C API
 */
class Material :public Entity {
  private:
  static SUMaterialRef create_material();

  /**
   * @brief Creates a SUMaterialRef derived from an existing Material.
   *
   * If the Material is already attached to a model, its existing ref is
   * returned. Otherwise a new SUMaterialRef is created.
   *
   * @param mat The Material to derive from.
   * @return The SUMaterialRef.
   */
  static SUMaterialRef copy_reference(const Material& mat);

  public:
  /**
   * @brief Constructs a null Material.
   */
  Material();

  /**
   * @brief Constructs a new Material with the given name.
   * @param name The name for the material (UTF-8 encoded).
   */
  Material(String name);

  /**
   * @brief Constructs a Material from an existing SUMaterialRef.
   * @param material_ref The native material reference.
   * @param attached     Whether the material is already owned by a model.
   */
  Material(SUMaterialRef material_ref, bool attached = true);

  /**
   * @brief Copy constructor.
   * @param other The Material to copy.
   */
  Material(const Material& other);

  /**
   * @brief Copy assignment operator.
   * @param other The Material to assign from.
   * @return Reference to this object.
   */
  Material& operator=(const Material& other);

  /**
   * @brief Destructor. Releases the material if it is not attached to a model.
   */
  ~Material();

  /**
   * @brief Returns the wrapped SUMaterialRef.
   * @return The raw SUMaterialRef.
   */
  SUMaterialRef ref() const;

  /** @brief Implicit conversion to SUMaterialRef. */
  operator SUMaterialRef() const;

  /** @brief Implicit conversion to SUMaterialRef*. */
  operator SUMaterialRef*();

  /**
   * @brief Returns true if this is an invalid or unset material.
   * @return True if the material is null or has no entity reference.
   */
  bool operator !() const;

  /**
   * @brief Returns a copy of this Material, not attached to any model.
   * @return A new unattached Material with the same properties.
   */
  Material copy() const;

  /**
   * @brief Retrieves the color of the material.
   * @return The Color.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialGetColor
   */
  Color color() const;

  /**
   * @brief Sets the color of the material.
   * @param color The Color to set.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialSetColor
   */
  void color(const Color& color);

  /**
   * @brief Retrieves the internal name of the material.
   *
   * The internal name is the unprocessed identifier string stored with
   * the material.
   *
   * @return The name as a String.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialGetName
   */
  String name() const;

  /**
   * @brief Sets the name of the material.
   * @param string The name to set (UTF-8 encoded).
   * @throws std::logic_error If the material is null.
   * @see SUMaterialSetName
   */
  void name(const String& string);

  /**
   * @brief Retrieves the display name of the material.
   *
   * If the internal name is encased in square brackets, this returns the
   * name without brackets. Otherwise it matches the name from GetName.
   *
   * @return The display name as a String.
   * @throws std::logic_error If the material is null.
   * @since SketchUp 2017, API 5.0
   * @see SUMaterialGetNameLegacyBehavior
   */
  String display_name() const;

  /**
   * @brief Retrieves the opacity of the material.
   * @return The alpha value in the range [0.0, 1.0].
   * @throws std::logic_error If the material is null.
   * @see SUMaterialGetOpacity
   */
  double opacity() const;

  /**
   * @brief Sets the opacity of the material.
   * @param alpha The alpha value in the range [0.0, 1.0].
   * @throws std::logic_error If the material is null.
   * @see SUMaterialSetOpacity
   */
  void opacity(const double alpha);

  /**
   * @brief Retrieves the texture of the material.
   * @return The Texture object.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialGetTexture
   */
  Texture texture() const;

  /**
   * @brief Sets the texture of the material.
   * @param texture The Texture to set.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialSetTexture
   */
  void texture(Texture& texture);

  /**
   * @brief Retrieves the type of the material.
   * @return The SUMaterialType (e.g. colored, textured, colorized texture).
   * @throws std::logic_error If the material is null.
   * @see SUMaterialGetType
   */
  SUMaterialType type() const;

  /**
   * @brief Sets the type of the material.
   * @param material_type The SUMaterialType to set.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialSetType
   */
  void type(const SUMaterialType& material_type);

  /**
   * @brief Returns whether the opacity value is used.
   * @return True if alpha is used.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialGetUseOpacity
   */
  bool use_alpha() const;

  /**
   * @brief Sets whether the opacity value is used.
   * @param flag True to use alpha, false otherwise.
   * @throws std::logic_error If the material is null.
   * @see SUMaterialSetUseOpacity
   */
  void use_alpha(bool flag);

  /** @brief Hash function for use with unordered_map. */
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
