//
//  DrawingElement.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "DrawingElement.hpp"

#include "Layer.hpp"
#include "Material.hpp"
#include "Geometry.hpp"


namespace CW {

DrawingElement::DrawingElement(SUDrawingElementRef drawing_element):
	m_drawing_element(drawing_element),
  Entity(SUDrawingElementToEntity(drawing_element))
{
}


BoundingBox3D DrawingElement::bounds() {
	SUBoundingBox3D box;
	SUDrawingElementGetBoundingBox(m_drawing_element, &box);
  return BoundingBox3D(box);
}


bool DrawingElement::copy_properties_from(DrawingElement element) {
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
	SUDrawingElementGetCastsShadows(m_drawing_element, &cast_shadows_flag);
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
	SUDrawingElementGetLayer(m_drawing_element, &layer_ref);
	return Layer(layer_ref);
}


bool DrawingElement::layer(Layer layer){
	SU_RESULT res = SUDrawingElementSetLayer(m_drawing_element, layer);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


Material DrawingElement::material() const {
	SUMaterialRef material_ref = SU_INVALID;
	SUDrawingElementGetMaterial(m_drawing_element, &material_ref);
	return Material(material_ref);
}


bool DrawingElement::material(const Material material) {
	SU_RESULT res = SUDrawingElementSetMaterial(m_drawing_element, material);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool DrawingElement::receive_shadows() const {
	bool receives_shadows_flag;
	SUDrawingElementGetReceivesShadows(m_drawing_element, &receives_shadows_flag);
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