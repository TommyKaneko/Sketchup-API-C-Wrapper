//
//  Axes.cpp
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

#include "SUAPI-CppWrapper/model/Axes.hpp"

#include <cassert>

namespace CW {

/**************************
* Private static methods **
***************************/
SUAxesRef Axes::create_axes() {
  SUAxesRef axes = SU_INVALID;
  SUResult res = SUAxesCreate(&axes);
  assert(res == SU_ERROR_NONE);
  return axes;
}

SUAxesRef Axes::create_custom_axes(const SUPoint3D& origin, const SUVector3D& xaxis, const SUVector3D& yaxis, const SUVector3D& zaxis) {
  SUAxesRef axes = SU_INVALID;
  SUResult res = SUAxesCreateCustom(&axes, &origin, &xaxis, &yaxis, &zaxis);
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
    return SUAxesFromEntity(other.m_entity);
  }
  // The other axes has not been attached to the model, so copy its properties to a new object
  SUAxesRef new_axes = create_custom_axes(other.origin(), other.x_axis(), other.y_axis(), other.z_axis());
  return new_axes;
}

  
/*****************************
* Constructors / Destructor **
******************************/
Axes::Axes():
  DrawingElement(SU_INVALID)
{}


Axes::Axes(SUAxesRef axes, bool attached):
  DrawingElement(SUAxesToDrawingElement(axes), attached)
{}


Axes::Axes(Point3D origin, Vector3D x_axis, Vector3D y_axis, Vector3D z_axis):
  DrawingElement(SUAxesToDrawingElement(create_custom_axes(origin, x_axis, y_axis, z_axis)), false)
{}

/** Copy constructor */
Axes::Axes(const Axes& other):
  DrawingElement(other, SUAxesToDrawingElement(copy_reference(other)))
{}


Axes::~Axes() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUAxesRef axes = this->ref();
    SUResult res = SUAxesRelease(&axes);
    assert(res == SU_ERROR_NONE);
  }
}
  
  
/******************
* Public Methods **
*******************/

/** Copy assignment operator */
Axes& Axes::operator=(const Axes& other) {
  if (m_attached && SUIsValid(m_entity)) {
    SUAxesRef axes = this->ref();
    SUResult res = SUAxesRelease(&axes);
    assert(res == SU_ERROR_NONE);
  }
  m_entity = SUAxesToEntity(copy_reference(other));
  DrawingElement::operator=(other);
  return *this;
}


bool Axes::operator!() const {
  if (SUIsInvalid(m_entity)) {
    return true;
  }
  return false;
}

/**
* Returns the SU native reference
*/
SUAxesRef Axes::ref() const {
  return SUAxesFromEntity(m_entity);
}

Vector3D Axes::x_axis() const {
  if (!(*this)) {
    throw std::logic_error("CW::Axes::x_axis(): Axes is null");
  }
  SUVector3D axis;
  SUResult res = SUAxesGetXAxis(this->ref(), &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}

Vector3D Axes::y_axis() const {
  if (!(*this)) {
    throw std::logic_error("CW::Axes::x_axis(): Axes is null");
  }
  SUVector3D axis;
  SUResult res = SUAxesGetYAxis(this->ref(), &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}

Vector3D Axes::z_axis() const {
  if (!(*this)) {
    throw std::logic_error("CW::Axes::x_axis(): Axes is null");
  }
  SUVector3D axis;
  SUResult res = SUAxesGetZAxis(this->ref(), &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}


Point3D Axes::origin() const {
  if (!(*this)) {
    throw std::logic_error("CW::Axes::x_axis(): Axes is null");
  }
  SUPoint3D origin;
  SUResult res = SUAxesGetOrigin(this->ref(), &origin);
  assert(res == SU_ERROR_NONE);
  return Point3D(origin);
}


Transformation Axes::transformation() const {
  if (!(*this)) {
    throw std::logic_error("CW::Axes::x_axis(): Axes is null");
  }
  SUTransformation transform;
  SUResult res = SUAxesGetTransform(this->ref(), &transform);
  assert(res == SU_ERROR_NONE);
  return Transformation(transform);
}

} /* namespace CW */
