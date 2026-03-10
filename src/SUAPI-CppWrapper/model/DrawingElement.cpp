//
//  DrawingElement.cpp
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"


namespace CW {

DrawingElement::DrawingElement(SUDrawingElementRef drawing_element, bool attached):
  Entity(SUDrawingElementToEntity(drawing_element), attached)
{}


DrawingElement::DrawingElement(const DrawingElement& other, SUDrawingElementRef element_ref):
  Entity(other, SUDrawingElementToEntity(element_ref))
{
  if (!other.m_attached && SUIsValid(other.ref())) {
    this->copy_properties_from(other);
  }
}

DrawingElement::DrawingElement():
  Entity()
{}


/** Copy assignment operator */
DrawingElement& DrawingElement::operator=(const DrawingElement& other) {
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    bool success = this->copy_properties_from(other);
    if (!success) {
      throw std::runtime_error("CW::DrawingElement::operator=: Failed to copy properties from detached DrawingElement");
    }
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
  this->casts_shadows(element.casts_shadows());
  this->hidden(element.hidden());
  this->receives_shadows(element.receives_shadows());
  // assigning layer and material properties may fail if the element is not attached to the model, so we only attempt it if both elements are attached
  if (!element.attached() || !this->attached()) {
    return true; // No-op for detached elements
  }
  // Only a layer or material that is attached to the same model can be assigned to the element
  if (element.model() != this->model()) {
    // Look for a layer with the same name
    std::vector<Layer> layers = this->model().layers();
    Layer matching_layer;
    for (const Layer& layer : layers) {
      if (layer.name() == element.layer().name()) {
        matching_layer = layer;
        break;
      }
    }
    if (matching_layer.is_valid()) {
      this->layer(matching_layer);
    } else {
      // No matching layer found, so we should create one in the model
      Layer new_layer = element.layer().copy();
      std::vector<Layer> new_layers = {new_layer};
      this->model().add_layers(new_layers);
      this->layer(new_layer);
    }

    // Look for a material with the same name
    std::vector<Material> materials = this->model().materials();
    Material matching_material;
    for (const Material& material : materials) {
      if (material.name() == element.material().name()) {
        matching_material = material;
        break;
      }
    }
    if (matching_material.is_valid()) {
      this->material(matching_material);
    } else {
      // No matching material found, so we should create one in the model
      Material new_material = element.material().copy();
      std::vector<Material> new_materials = {new_material};
      this->model().add_materials(new_materials);
      this->material(new_material);
    }
  }
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


void DrawingElement::casts_shadows(bool casts_shadows) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::casts_shadows(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetCastsShadows(this->ref(), casts_shadows);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool DrawingElement::hidden() const {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::hidden(): DrawingElement is null");
  }
  bool hide_flag;
  SUDrawingElementGetHidden(this->ref(), &hide_flag);
  return hide_flag;
}


void DrawingElement::hidden(bool hidden) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::hidden(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetHidden(this->ref(), hidden);
  assert(res == SU_ERROR_NONE); _unused(res);
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


void DrawingElement::layer(const Layer& layer){
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::layer(): DrawingElement is null");
  }
  if (!layer.is_valid()) {
    return; // No-op for invalid/empty layer
  }
  if (!layer.attached()) {
    throw std::logic_error("CW::DrawingElement::layer(): only a layer that is attached to a model can be assigned to a DrawingElement");
  }
  SUResult res = SUDrawingElementSetLayer(this->ref(), layer);
  assert(res == SU_ERROR_NONE); _unused(res);
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


void DrawingElement::material(const Material& material) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::material(): DrawingElement is null");
  }
  if (!material.is_valid()) {
    return; // No-op for invalid/empty material
  }
  if (!material.attached()) {
    throw std::logic_error("CW::DrawingElement::material(): only a material that is attached to a model can be assigned to a DrawingElement");
  }
  SUResult res = SUDrawingElementSetMaterial(this->ref(), material);
  switch (res) {
    case SU_ERROR_INVALID_ARGUMENT: {
      throw std::invalid_argument("CW::DrawingElement::material(): the material is owned by a layer or image");
    }
    default: {
      assert(res == SU_ERROR_NONE); _unused(res);
    }
  }
}


bool DrawingElement::receives_shadows() const {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::receive_shadows(): DrawingElement is null");
  }
  bool receives_shadows_flag;
  SUResult res = SUDrawingElementGetReceivesShadows(this->ref(), &receives_shadows_flag);
  assert (res == SU_ERROR_NONE); _unused(res);
  return receives_shadows_flag;
}


void DrawingElement::receives_shadows(bool receives_shadows_flag) {
  if (SUIsInvalid(m_entity)) {
    throw std::logic_error("CW::DrawingElement::receive_shadows(): DrawingElement is null");
  }
  SUResult res = SUDrawingElementSetReceivesShadows(this->ref(), receives_shadows_flag);
  assert(res == SU_ERROR_NONE); _unused(res);
}

} /* namespace CW */
