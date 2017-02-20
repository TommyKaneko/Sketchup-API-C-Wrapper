//
//  Axes.cpp
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

#include "Axes.hpp"

#include <cassert>

namespace CW {

/**************************
* Private static methods **
***************************/
SUAxesRef Axes::create_axes() {
	SUAxesRef axes = SU_INVALID;
  SU_RESULT res = SUAxesCreate(&axes);
  assert(res == SU_ERROR_NONE);
  return axes;
}

SUAxesRef Axes::create_custom_axes(const SUPoint3D& origin, const SUVector3D& xaxis, const SUVector3D& yaxis, const SUVector3D& zaxis) {
	SUAxesRef axes = SU_INVALID;
	SU_RESULT res = SUAxesCreateCustom(&axes, &origin, &xaxis, &yaxis, &zaxis);
  if(res != SU_ERROR_NONE) {
  	// There is a problem with this axis, so return an invalid one
    if (SUIsValid(axes)) {
    	SUAxesRelease(&axes);
    }
    return SU_INVALID;
  }
  return axes;
}

SUAxesRef Axes::copy_reference(const Axes& other) {
	if (other.m_attached) {
  	return other.m_axes;
  }
  // The other axes has not been attached to the model, so copy its properties to a new object
  SUAxesRef new_axes = create_custom_axes(other.origin(), other.x_axis(), other.y_axis(), other.z_axis());
  return new_axes;
}

  
/*****************************
* Constructors / Destructor **
******************************/
Axes::Axes():
  DrawingElement(SU_INVALID),
  m_axes(SU_INVALID)
{}


Axes::Axes(SUAxesRef axes, bool attached):
  DrawingElement(SUAxesToDrawingElement(axes), attached),
  m_axes(axes)
{}


Axes::Axes(Point3D origin, Vector3D x_axis, Vector3D y_axis, Vector3D z_axis):
  DrawingElement(SUAxesToDrawingElement(create_custom_axes(origin, x_axis, y_axis, z_axis)), false),
  m_axes(SUAxesFromDrawingElement(m_drawing_element))
{}

/** Copy constructor */
Axes::Axes(const Axes& other):
	DrawingElement(other, SUAxesToDrawingElement(copy_reference(other))),
  m_axes(SUAxesFromDrawingElement(m_drawing_element))
{}


Axes::~Axes() {
	if (!m_attached && SUIsValid(m_axes)) {
		SU_RESULT res = SUAxesRelease(&m_axes);
    assert(res == SU_ERROR_NONE);
	}
}
  
  
/******************
* Public Methods **
*******************/

/** Copy assignment operator */
Axes& Axes::operator=(const Axes& other) {
  if (m_attached && SUIsValid(m_axes)) {
    SU_RESULT res = SUAxesRelease(&m_axes);
    assert(res == SU_ERROR_NONE);
  }
  m_axes = copy_reference(other);
  m_drawing_element = SUAxesToDrawingElement(m_axes);
  DrawingElement::operator=(other);
  return *this;
}


Vector3D Axes::x_axis() const {
  SUVector3D axis;
  SU_RESULT res = SUAxesGetXAxis(m_axes, &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}

Vector3D Axes::y_axis() const {
  SUVector3D axis;
  SU_RESULT res = SUAxesGetYAxis(m_axes, &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}

Vector3D Axes::z_axis() const {
  SUVector3D axis;
  SU_RESULT res = SUAxesGetZAxis(m_axes, &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}


Point3D Axes::origin() const {
  SUPoint3D origin;
  SU_RESULT res = SUAxesGetOrigin(m_axes, &origin);
  assert(res == SU_ERROR_NONE);
  return Point3D(origin);
}


Transformation Axes::transformation() const {
  SUTransformation transform;
  SU_RESULT res = SUAxesGetTransform(m_axes, &transform);
  assert(res == SU_ERROR_NONE);
  return Transformation(transform);
}

} /* namespace CW */