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

#include "SUAPI-CppWrapper/model/Material.hpp"

#include <cassert>
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
  assert(res == SU_ERROR_NONE);
  return material;
}


SUMaterialRef Material::copy_reference(const Material& mat) {
  if (mat.m_attached || SUIsInvalid(mat.m_material)) {
    return mat.m_material;
  }
  return create_material();;
}

/*****************************
* Constructors / Destructor **
****************************/
Material::Material():
  Entity(SU_INVALID, false),
  m_material(SU_INVALID)
{}


Material::Material(SUMaterialRef material_ref, bool attached):
  Entity(SUMaterialToEntity(material_ref), attached),
  m_material(material_ref)
{}


Material::Material(const Material& other):
  Entity(other, SUMaterialToEntity(copy_reference(other))),
  m_material(SUMaterialFromEntity(m_entity))
{
  // Copy across properties of the other material
  if (!other.m_attached) {
    // TODO: copy across properties from other material
  }
}


Material& Material::operator=(const Material& other) {
  if (!m_attached && SUIsValid(m_material)) {
    SUResult res = SUMaterialRelease(&m_material);
    assert(res == SU_ERROR_NONE);
  }
  if (!other) {
    m_material = other.m_material;
    Entity::operator=(other);
    return *this;
  }
  else if (!other.m_attached) {
    // The other material has not been attached to the model, so copy its properties to a new object
    m_material = create_material();
    m_attached = other.m_attached;
    // TODO: copy across other material properties
  }
  else {
    // The other material has been attached to the model, so it is safe to transfer the SUMaterialRef object
    m_material = other.m_material;
    m_attached = true;
  }
  m_entity = SUMaterialToEntity(m_material);
  return *this;
}


Material::~Material() {
  if (!m_attached && SUIsValid(m_material)) {
    SUResult res = SUMaterialRelease(&m_material);
    assert(res == SU_ERROR_NONE);
  }
}


SUMaterialRef Material::ref() const {
  return m_material;
}


Material::operator SUMaterialRef() const {
  return ref();
}


Material::operator SUMaterialRef*() {
  return &m_material;
}

bool Material::operator!() const {
  if (SUIsInvalid(m_material)) {
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
  new_material.opacity(this->opacity());
  new_material.texture(this->texture());
  new_material.color(this->color());
  new_material.type(this->type());
  
  return new_material;
}


Color Material::color() const {
  if (!(*this)) {
    return Color();
  }
  SUColor new_color = SU_INVALID;
  SUResult res =  SUMaterialGetColor(m_material, &new_color);
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
  SUResult res = SUMaterialSetColor(m_material, &set_color);
  assert(res != SU_ERROR_INVALID_INPUT);
}


String Material::name() const {
  if (!(*this)) {
    return String();
  }
  SUStringRef name_ref = SU_INVALID;
  SUResult res = SUStringCreate(&name_ref);
  assert(res == SU_ERROR_NONE);
  res = SUMaterialGetName(m_material, &name_ref);
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
  SUResult res = SUMaterialSetName(m_material, cstr);
  assert(res == SU_ERROR_NONE);
  return;
}


double Material::opacity() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::opacity(): Material is null");
  }
  double alpha;
  SUResult res = SUMaterialGetOpacity(m_material, &alpha);
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
  SUResult res = SUMaterialSetOpacity(m_material, input_alpha);
  assert(res != SU_ERROR_OUT_OF_RANGE);
}
  

Texture Material::texture() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::TEXTURE(): Material is null");
  }
  SUTextureRef get_texture = SU_INVALID;
  SUResult res = SUMaterialGetTexture(m_material, &get_texture);
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
  SUResult res = SUMaterialSetTexture(m_material, texture_ref);
  assert(res == SU_ERROR_NONE);
}
  
  
SUMaterialType Material::type() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::type(): Material is null");
  }
  SUMaterialType mat_type;
  SUResult res = SUMaterialGetType(m_material, &mat_type);
  assert(res == SU_ERROR_NONE);
  return mat_type;
}


void Material::type(const SUMaterialType& material_type) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::type(): Material is null");
  }
  SUResult res = SUMaterialSetType(m_material, material_type);
  assert(res == SU_ERROR_NONE);
}


bool Material::use_alpha() const {
  if(!(*this)) {
    throw std::logic_error("CW::Material::use_alpha(): Material is null");
  }
  bool flag;
  SUResult res = SUMaterialGetUseOpacity(m_material, &flag);
  assert(res == SU_ERROR_NONE);
  return flag;
}


void Material::use_alpha(bool flag) {
  if(!(*this)) {
    throw std::logic_error("CW::Material::use_alpha(): Material is null");
  }
  assert(!!(*this));
  SUResult res = SUMaterialSetUseOpacity(m_material, flag);
  assert(res == SU_ERROR_NONE);
}



} /* namespace CW */
