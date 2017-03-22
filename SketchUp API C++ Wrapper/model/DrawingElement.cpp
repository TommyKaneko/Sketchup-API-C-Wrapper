//
//  DrawingElement.cpp
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
#include "DrawingElement.hpp"

#include <cassert>

#include "Layer.hpp"
#include "Material.hpp"
#include "Geometry.hpp"


namespace CW {

DrawingElement::DrawingElement(SUDrawingElementRef drawing_element, bool attached):
  Entity(SUDrawingElementToEntity(drawing_element), attached),
	m_drawing_element(drawing_element)
{
}


DrawingElement::DrawingElement(const DrawingElement& other, SUDrawingElementRef element_ref):
	Entity(other, SUDrawingElementToEntity(element_ref)),
  m_drawing_element(element_ref)
{}

DrawingElement::DrawingElement():
	Entity(),
  m_drawing_element(SU_INVALID)
{}


/** Copy assignment operator */
DrawingElement& DrawingElement::operator=(const DrawingElement& other) {
  if (!other.m_attached && SUIsValid(other.m_drawing_element)) {
    this->copy_attributes_from(other);
  }
  return (*this);
}


DrawingElement::operator SUDrawingElementRef() const {
	return m_drawing_element;
}

DrawingElement::operator SUDrawingElementRef*() {
	return &m_drawing_element;
}


BoundingBox3D DrawingElement::bounds() {
	SUBoundingBox3D box = SU_INVALID;
	SU_RESULT res = SUDrawingElementGetBoundingBox(m_drawing_element, &box);
  assert(res == SU_ERROR_NONE);
  return BoundingBox3D(box);
}


bool DrawingElement::copy_properties_from(const DrawingElement& element) {
  bool success = casts_shadows(element.casts_shadows());
  if (!success)
  	return false;
  success = hidden(element.hidden());
  if (!success)
  	return false;
  success = receive_shadows(element.receive_shadows());
  if (!success)
  	return false;
  success = layer(element.layer());
  if (!success)
  	return false;
  success = material(element.material());
  if (!success)
  	return false;
  
  return true;
}


bool DrawingElement::casts_shadows() const {
	bool cast_shadows_flag;
	SU_RESULT res = SUDrawingElementGetCastsShadows(m_drawing_element, &cast_shadows_flag);
  assert(res == SU_ERROR_NONE);
  return cast_shadows_flag;
}


bool DrawingElement::casts_shadows(bool casts_shadows) {
	SU_RESULT res = SUDrawingElementSetCastsShadows(m_drawing_element, casts_shadows);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool DrawingElement::hidden() const {
	bool hide_flag;
	SUDrawingElementGetHidden(m_drawing_element, &hide_flag);
	return hide_flag;
}


bool DrawingElement::hidden(bool hidden) {
	SU_RESULT res = SUDrawingElementSetHidden(m_drawing_element, hidden);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


Layer DrawingElement::layer() const {
	SULayerRef layer_ref = SU_INVALID;
	SU_RESULT res = SUDrawingElementGetLayer(m_drawing_element, &layer_ref);
  if (res == SU_ERROR_NULL_POINTER_OUTPUT) {
  	return Layer();
  }
  assert(res == SU_ERROR_NONE);
  return Layer(layer_ref, true);
}


bool DrawingElement::layer(const Layer& layer){
	SU_RESULT res = SUDrawingElementSetLayer(m_drawing_element, layer);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


Material DrawingElement::material() const {
	SUMaterialRef material_ref = SU_INVALID;
	SU_RESULT res = SUDrawingElementGetMaterial(m_drawing_element, &material_ref);
  if (res == SU_ERROR_NO_DATA || res == SU_ERROR_NULL_POINTER_OUTPUT) {
  	return Material();
  }
  assert(res == SU_ERROR_NONE);
  return Material(material_ref, true);
}


bool DrawingElement::material(const Material& material) {
	SU_RESULT res = SUDrawingElementSetMaterial(m_drawing_element, material);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool DrawingElement::receive_shadows() const {
	bool receives_shadows_flag;
	SU_RESULT res = SUDrawingElementGetReceivesShadows(m_drawing_element, &receives_shadows_flag);
	assert (res == SU_ERROR_NONE);
	return receives_shadows_flag;
}


bool DrawingElement::receive_shadows(bool receives_shadows_flag) {
	SU_RESULT res = SUDrawingElementSetReceivesShadows(m_drawing_element, receives_shadows_flag);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}

} /* namespace CW */