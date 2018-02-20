//
//  DrawingElement.cpp
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
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

#include <cassert>
#include <stdexcept>

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
  SUResult res = SUDrawingElementGetBoundingBox(m_drawing_element, &box);
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
  SUResult res = SUDrawingElementGetCastsShadows(m_drawing_element, &cast_shadows_flag);
  assert(res == SU_ERROR_NONE);
  return cast_shadows_flag;
}


bool DrawingElement::casts_shadows(bool casts_shadows) {
  if (SUIsInvalid(m_drawing_element)) {
    throw std::logic_error("CW::DrawingElement::casts_shadows(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetCastsShadows(m_drawing_element, casts_shadows);
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
  SUResult res = SUDrawingElementSetHidden(m_drawing_element, hidden);
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
  SUResult res = SUDrawingElementGetLayer(m_drawing_element, &layer_ref);
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
  SUResult res = SUDrawingElementSetLayer(m_drawing_element, layer);
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
  SUResult res = SUDrawingElementGetMaterial(m_drawing_element, &material_ref);
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
  SUResult res = SUDrawingElementSetMaterial(m_drawing_element, material);
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
  SUResult res = SUDrawingElementGetReceivesShadows(m_drawing_element, &receives_shadows_flag);
  assert (res == SU_ERROR_NONE);
  return receives_shadows_flag;
}


bool DrawingElement::receive_shadows(bool receives_shadows_flag) {
  if (SUIsInvalid(m_drawing_element)) {
    throw std::logic_error("CW::DrawingElement::receive_shadows(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetReceivesShadows(m_drawing_element, receives_shadows_flag);
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}

} /* namespace CW */
