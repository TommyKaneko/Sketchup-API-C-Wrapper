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
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

#include <cassert>

#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"


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
    this->copy_properties_from(other);
  }
  m_entity = SUDrawingElementToEntity(m_drawing_element);
  Entity::operator=(other);
  return (*this);
}


DrawingElement::operator SUDrawingElementRef() const {
	return m_drawing_element;
}

DrawingElement::operator SUDrawingElementRef*() {
	return &m_drawing_element;
}


BoundingBox3D DrawingElement::bounds() {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::bounds(): DrawingElement is null");
  }
	SUBoundingBox3D box = SU_INVALID;
	SU_RESULT res = SUDrawingElementGetBoundingBox(m_drawing_element, &box);
  assert(res == SU_ERROR_NONE);
  return BoundingBox3D(box);
}


bool DrawingElement::copy_properties_from(const DrawingElement& element) {
  bool success = this->casts_shadows(element.casts_shadows());
  if (!success)
  	return false;
  success = this->hidden(element.hidden());
  if (!success)
  	return false;
  success = this->receive_shadows(element.receive_shadows());
  if (!success)
  	return false;
  Layer elem_layer = element.layer();
  success = this->layer(elem_layer);
  if (!success)
  	return false;
  success = this->material(element.material());
  if (!success)
  	return false;
  
  return true;
}


bool DrawingElement::casts_shadows() const {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::casts_shadows(): DrawingElement is null");
  }
	bool cast_shadows_flag;
	SU_RESULT res = SUDrawingElementGetCastsShadows(m_drawing_element, &cast_shadows_flag);
  assert(res == SU_ERROR_NONE);
  return cast_shadows_flag;
}


bool DrawingElement::casts_shadows(bool casts_shadows) {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::casts_shadows(): DrawingElement is null");
  }
	SU_RESULT res = SUDrawingElementSetCastsShadows(m_drawing_element, casts_shadows);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool DrawingElement::hidden() const {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::hidden(): DrawingElement is null");
  }
	bool hide_flag;
	SUDrawingElementGetHidden(m_drawing_element, &hide_flag);
	return hide_flag;
}


bool DrawingElement::hidden(bool hidden) {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::hidden(): DrawingElement is null");
  }
	SU_RESULT res = SUDrawingElementSetHidden(m_drawing_element, hidden);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


Layer DrawingElement::layer() const {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::layer(): DrawingElement is null");
  }
	SULayerRef layer_ref = SU_INVALID;
	SU_RESULT res = SUDrawingElementGetLayer(m_drawing_element, &layer_ref);
  if (res == SU_ERROR_NULL_POINTER_OUTPUT || res == SU_ERROR_NO_DATA) {
  	return Layer();
  }
  assert(res == SU_ERROR_NONE);
  return Layer(layer_ref, true);
}


bool DrawingElement::layer(Layer& layer){
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::layer(): DrawingElement is null");
  }
	SU_RESULT res = SUDrawingElementSetLayer(m_drawing_element, layer);
  if (res == SU_ERROR_NONE) {
  	layer.attached(true);
    return true;
  }
  return false;
}


Material DrawingElement::material() const {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::material(): DrawingElement is null");
  }
	SUMaterialRef material_ref = SU_INVALID;
	SU_RESULT res = SUDrawingElementGetMaterial(m_drawing_element, &material_ref);
  if (res == SU_ERROR_NO_DATA || res == SU_ERROR_NULL_POINTER_OUTPUT) {
  	return Material();
  }
  assert(res == SU_ERROR_NONE);
  return Material(material_ref, true);
}


bool DrawingElement::material(const Material& material) {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::material(): DrawingElement is null");
  }
	SU_RESULT res = SUDrawingElementSetMaterial(m_drawing_element, material);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool DrawingElement::receive_shadows() const {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::receive_shadows(): DrawingElement is null");
  }
	bool receives_shadows_flag;
	SU_RESULT res = SUDrawingElementGetReceivesShadows(m_drawing_element, &receives_shadows_flag);
	assert (res == SU_ERROR_NONE);
	return receives_shadows_flag;
}


bool DrawingElement::receive_shadows(bool receives_shadows_flag) {
  if (SUIsInvalid(m_drawing_element)) {
  	throw std::logic_error("CW::DrawingElement::receive_shadows(): DrawingElement is null");
  }
	SU_RESULT res = SUDrawingElementSetReceivesShadows(m_drawing_element, receives_shadows_flag);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}

} /* namespace CW */
