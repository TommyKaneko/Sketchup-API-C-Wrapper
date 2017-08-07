//
//  Material.cpp
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

#include "Material.hpp"

#include <cassert>
#include "String.hpp"
#include "Color.hpp"
#include "Texture.hpp"

namespace CW {

/**************************
* Static private methods **
***************************/

SUMaterialRef Material::create_material() {
	SUMaterialRef material = SU_INVALID;
  SU_RESULT res = SUMaterialCreate(&material);
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
    SU_RESULT res = SUMaterialRelease(&m_material);
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
		SU_RESULT res = SUMaterialRelease(&m_material);
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
	Material new_material(create_material());
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
	SU_RESULT res =	SUMaterialGetColor(m_material, &new_color);
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
  SU_RESULT res = SUMaterialSetColor(m_material, &set_color);
  assert(res != SU_ERROR_INVALID_INPUT);
}


String Material::name() const {
	if (!(*this)) {
  	return String();
  }
  SUStringRef name;
  SU_RESULT res = SUMaterialGetName(m_material, &name);
	//assert(res != SU_ERROR_INVALID_OUTPUT);
  if (res == SU_ERROR_NONE) {
  	return String(name);
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
  SU_RESULT res = SUMaterialSetName(m_material, cstr);
  assert(res == SU_ERROR_NONE);
  return;
}


double Material::opacity() const {
  if(!(*this)) {
  	throw std::logic_error("CW::Material::opacity(): Material is null");
  }
  double alpha;
  SU_RESULT res = SUMaterialGetOpacity(m_material, &alpha);
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
  SU_RESULT res = SUMaterialSetOpacity(m_material, input_alpha);
  assert(res != SU_ERROR_OUT_OF_RANGE);
}
  

Texture Material::texture() const {
  if(!(*this)) {
  	throw std::logic_error("CW::Material::TEXTURE(): Material is null");
  }
	SUTextureRef get_texture = SU_INVALID;
	SU_RESULT res = SUMaterialGetTexture(m_material, &get_texture);
  if (res != SU_ERROR_NONE) {
  	return Texture();
  }
	return Texture(get_texture);
}


void Material::texture(const Texture& texture) {
  if(!(*this)) {
  	throw std::logic_error("CW::Material::texture(): Material is null");
  }
  SUTextureRef texture_ref = texture.ref();
	SU_RESULT res = SUMaterialSetTexture(m_material, texture_ref);
}
  
  
SUMaterialType Material::type() const {
  if(!(*this)) {
  	throw std::logic_error("CW::Material::type(): Material is null");
  }
  SUMaterialType mat_type;
	SU_RESULT res = SUMaterialGetType(m_material, &mat_type);
	assert(res == SU_ERROR_NONE);
  return mat_type;
}


void Material::type(const SUMaterialType& material_type) {
  if(!(*this)) {
  	throw std::logic_error("CW::Material::type(): Material is null");
  }
	SU_RESULT res = SUMaterialSetType(m_material, material_type);
	assert(res == SU_ERROR_NONE);
}


bool Material::use_alpha() const {
  if(!(*this)) {
  	throw std::logic_error("CW::Material::use_alpha(): Material is null");
  }
  bool flag;
  SU_RESULT res = SUMaterialGetUseOpacity(m_material, &flag);
	assert(res == SU_ERROR_NONE);
	return flag;
}


void Material::use_alpha(bool flag) {
  if(!(*this)) {
  	throw std::logic_error("CW::Material::use_alpha(): Material is null");
  }
	assert(!!(*this));
  SU_RESULT res = SUMaterialSetUseOpacity(m_material, flag);
	assert(res == SU_ERROR_NONE);
}



} /* namespace CW */