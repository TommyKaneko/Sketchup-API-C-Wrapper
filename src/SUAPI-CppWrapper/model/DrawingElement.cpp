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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"


namespace CW {

DrawingElement::DrawingElement(SUDrawingElementRef drawing_element, bool attached):
  Entity(SUDrawingElementToEntity(drawing_element), attached)
{}


DrawingElement::DrawingElement(const DrawingElement& other, SUDrawingElementRef element_ref):
  Entity(other, SUDrawingElementToEntity(element_ref))
{}

DrawingElement::DrawingElement():
  Entity()
{}


/** Copy assignment operator */
DrawingElement& DrawingElement::operator=(const DrawingElement& other) {
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    this->copy_properties_from(other);
  }
  Entity::operator=(other);
  return (*this);
}


SUDrawingElementRef DrawingElement::ref() const {
  return SUDrawingElementFromEntity(m_entity);
}


DrawingElement::operator SUDrawingElementRef() const {
  return this->ref();
}


BoundingBox3D DrawingElement::bounds() {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::bounds(): DrawingElement is null");
  }
  SUBoundingBox3D box;
  SUResult res = SUDrawingElementGetBoundingBox(this->ref(), &box);
  assert(res == SU_ERROR_NONE); _unused(res);
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
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::casts_shadows(): DrawingElement is null");
  }
  bool cast_shadows_flag;
  SUResult res = SUDrawingElementGetCastsShadows(this->ref(), &cast_shadows_flag);
  assert(res == SU_ERROR_NONE); _unused(res);
  return cast_shadows_flag;
}


bool DrawingElement::casts_shadows(bool casts_shadows) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::casts_shadows(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetCastsShadows(this->ref(), casts_shadows);
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


bool DrawingElement::hidden() const {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::hidden(): DrawingElement is null");
  }
  bool hide_flag;
  SUDrawingElementGetHidden(this->ref(), &hide_flag);
  return hide_flag;
}


bool DrawingElement::hidden(bool hidden) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::hidden(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetHidden(this->ref(), hidden);
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


Layer DrawingElement::layer() const {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::layer(): DrawingElement is null");
  }
  SULayerRef layer_ref = SU_INVALID;
  SUResult res = SUDrawingElementGetLayer(this->ref(), &layer_ref);
  if (res == SU_ERROR_NULL_POINTER_OUTPUT || res == SU_ERROR_NO_DATA) {
    return Layer();
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return Layer(layer_ref, true);
}


bool DrawingElement::layer(Layer& layer){
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::layer(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetLayer(this->ref(), layer);
  if (res == SU_ERROR_NONE) {
    layer.attached(true);
    return true;
  }
  return false;
}


Material DrawingElement::material() const {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::material(): DrawingElement is null");
  }
  SUMaterialRef material_ref = SU_INVALID;
  SUResult res = SUDrawingElementGetMaterial(this->ref(), &material_ref);
  if (res == SU_ERROR_NO_DATA || res == SU_ERROR_NULL_POINTER_OUTPUT) {
    return Material();
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return Material(material_ref, true);
}


bool DrawingElement::material(const Material& material) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::material(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetMaterial(this->ref(), material);
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


bool DrawingElement::receive_shadows() const {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::receive_shadows(): DrawingElement is null");
  }
  bool receives_shadows_flag;
  SUResult res = SUDrawingElementGetReceivesShadows(this->ref(), &receives_shadows_flag);
  assert (res == SU_ERROR_NONE); _unused(res);
  return receives_shadows_flag;
}


bool DrawingElement::receive_shadows(bool receives_shadows_flag) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::receive_shadows(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetReceivesShadows(this->ref(), receives_shadows_flag);
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}

} /* namespace CW */
