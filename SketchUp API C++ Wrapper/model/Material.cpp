//
//  Material.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "Material.hpp"

#include <cassert>
#include "String.hpp"

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
  if (!other.m_attached) {
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
  if (SUIsValid(m_material)) {
  	return true;
  }
  return false;
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


} /* namespace CW */