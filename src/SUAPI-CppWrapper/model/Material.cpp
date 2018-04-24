//
//  Material.cpp
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Material.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/model/Texture.hpp"

namespace CW {

/**************************
* Static private methods **
***************************/

SUMaterialRef Material::create_material() {
  SUMaterialRef material = SU_INVALID;
  SUResult res = SUMaterialCreate(&material);
  assert(res == SU_ERROR_NONE); _unused(res);
  return material;
}


SUMaterialRef Material::copy_reference(const Material& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  return create_material();;
}

/*****************************
* Constructors / Destructor **
****************************/
Material::Material():
  Entity()
{}


Material::Material(SUMaterialRef material_ref, bool attached):
  Entity(SUMaterialToEntity(material_ref), attached)
{}


Material::Material(const Material& other):
  Entity(other, SUMaterialToEntity(copy_reference(other)))
{
  // Copy across properties of the other material
  if (!other.m_attached && SUIsValid(m_entity)) {
    this->name(other.name());
    this->type(other.type());
    this->opacity(other.opacity());
    this->color(other.color());
    this->texture(other.texture());
  }
}


Material& Material::operator=(const Material& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUMaterialRef material = this->ref();
    SUResult res = SUMaterialRelease(&material);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUMaterialToEntity(copy_reference(other));

  if (!other.m_attached && SUIsValid(other.m_entity)) {
    // The other material has not been attached to the model, so copy its properties to a new object
    this->name(other.name());
    this->type(other.type());
    this->opacity(other.opacity());
    this->color(other.color());
    this->texture(other.texture());
  }
  Entity::operator=(other);
  return *this;
}


Material::~Material() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUMaterialRef material = this->ref();
    SUResult res = SUMaterialRelease(&material);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


SUMaterialRef Material::ref() const {
  return SUMaterialFromEntity(m_entity);
}


Material::operator SUMaterialRef() const {
  return this->ref();
}

bool Material::operator!() const {
  if (SUIsInvalid(m_entity)) {
    return true;
  }
  return false;
}

Material Material::copy() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::copy(): Material is null");
  }
  Material new_material(create_material(), false);
  new_material.name(this->name());
  new_material.type(this->type());
  new_material.opacity(this->opacity());
  new_material.color(this->color());
  new_material.texture(this->texture());
  return new_material;
}


Color Material::color() const {
  if (!(*this)) {
    return Color();
  }
  SUColor new_color = SU_INVALID;
  SUResult res =  SUMaterialGetColor(this->ref(), &new_color);
  if (res == SU_ERROR_NONE) {
    return Color(new_color);
  }
  return Color();
}


void Material::color(const Color& color) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::color(): Material is null");
  }
  SUColor set_color = color.ref();
  SUResult res = SUMaterialSetColor(this->ref(), &set_color);
  assert(res != SU_ERROR_INVALID_INPUT); _unused(res);
}


String Material::name() const {
  if (!(*this)) {
    return String();
  }
  SUStringRef name_ref = SU_INVALID;
  SUResult res = SUStringCreate(&name_ref);
  assert(res == SU_ERROR_NONE);
  res = SUMaterialGetName(this->ref(), &name_ref);
  //assert(res != SU_ERROR_INVALID_OUTPUT);
  if (res == SU_ERROR_NONE) {
    return String(name_ref);
  }
  else {
    return String();
  }
}

void Material::name(const String& string) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::name(): Material is null");
  }
  const char *cstr = string.std_string().c_str();
  SUResult res = SUMaterialSetName(this->ref(), cstr);
  assert(res == SU_ERROR_NONE); _unused(res);
  return;
}


double Material::opacity() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::opacity(): Material is null");
  }
  double alpha;
  SUResult res = SUMaterialGetOpacity(this->ref(), &alpha);
  if (res == SU_ERROR_NONE) {
    return alpha;
  }
  return 1.0;
}
  

void Material::opacity(const double alpha) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::opacity(): Material is null");
  }
  double input_alpha = 1.0;
  if (alpha > 1.0) {
    input_alpha = 1.0;
  }
  else if (alpha < 0.0) {
    input_alpha = 0.0;
  }
  SUResult res = SUMaterialSetOpacity(this->ref(), input_alpha);
  assert(res != SU_ERROR_OUT_OF_RANGE); _unused(res);
}
  

Texture Material::texture() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::TEXTURE(): Material is null");
  }
  SUTextureRef get_texture = SU_INVALID;
  SUResult res = SUMaterialGetTexture(this->ref(), &get_texture);
  if (res != SU_ERROR_NONE) {
    return Texture();
  }
  return Texture(get_texture);
}


void Material::texture(const Texture& texture) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::texture(): Material is null");
  }
  if (!texture) {
    // no texture to apply
    return;
  }
  if (texture.attached()) {
    return this->texture(texture.copy());
  }
  SUTextureRef texture_ref = texture.ref();
  SUResult res = SUMaterialSetTexture(this->ref(), texture_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
}
  
  
SUMaterialType Material::type() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::type(): Material is null");
  }
  SUMaterialType mat_type;
  SUResult res = SUMaterialGetType(this->ref(), &mat_type);
  assert(res == SU_ERROR_NONE); _unused(res);
  return mat_type;
}


void Material::type(const SUMaterialType& material_type) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::type(): Material is null");
  }
  SUResult res = SUMaterialSetType(this->ref(), material_type);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Material::use_alpha() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::use_alpha(): Material is null");
  }
  bool flag;
  SUResult res = SUMaterialGetUseOpacity(this->ref(), &flag);
  assert(res == SU_ERROR_NONE); _unused(res);
  return flag;
}


void Material::use_alpha(bool flag) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::use_alpha(): Material is null");
  }
  assert(!!(*this));
  SUResult res = SUMaterialSetUseOpacity(this->ref(), flag);
  assert(res == SU_ERROR_NONE); _unused(res);
}



} /* namespace CW */
