//
//  DimensionLinear.cpp
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

#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/DimensionLinear.hpp"

#include <cassert>
#include <stdexcept>

#include <SketchUpAPI/model/instancepath.h>

#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUDimensionLinearRef DimensionLinear::create_dimension_linear(
    const Point3D& start, const Point3D& end, double offset) {
  SUDimensionLinearRef dim = SU_INVALID;
  SUPoint3D start_pt = start;
  SUPoint3D end_pt = end;
  SUResult res = SUDimensionLinearCreate(&dim, &start_pt, SU_INVALID,
                                         &end_pt, SU_INVALID, offset);
  assert(res == SU_ERROR_NONE); _unused(res);
  return dim;
}


SUDimensionLinearRef DimensionLinear::copy_reference(const DimensionLinear& other) {
  if (other.m_attached || !other) {
    return other.linear_ref();
  }
  // Create a new dimension with the same geometry
  Point3D start = other.start_point();
  Point3D end = other.end_point();
  // Compute offset from text position and start/end midpoint
  // Use a default offset of 2.0 for detached copies
  SUDimensionLinearRef new_dim = create_dimension_linear(start, end, 2.0);
  return new_dim;
}


/******************************
** Constructors / Destructor **
*******************************/
DimensionLinear::DimensionLinear():
  Dimension()
{}


DimensionLinear::DimensionLinear(const Point3D& start, const Point3D& end,
                                 double offset):
  Dimension(SUDimensionLinearToDimension(
    create_dimension_linear(start, end, offset)), false)
{}


DimensionLinear::DimensionLinear(SUDimensionLinearRef dimension, bool attached):
  Dimension(SUDimensionLinearToDimension(dimension), attached)
{}


DimensionLinear::DimensionLinear(const DimensionLinear& other):
  Dimension(SUDimensionLinearToDimension(copy_reference(other)))
{
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    m_attached = false;
    // Copy properties from the other dimension
    this->text(other.text());
    this->text_3d(other.text_3d());
    this->arrow_type(other.arrow_type());
  }
}


DimensionLinear::~DimensionLinear() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUDimensionLinearRef dim = this->linear_ref();
    SUResult res = SUDimensionLinearRelease(&dim);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


DimensionLinear& DimensionLinear::operator=(const DimensionLinear& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUDimensionLinearRef dim = this->linear_ref();
    SUResult res = SUDimensionLinearRelease(&dim);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUDimensionToEntity(
    SUDimensionLinearToDimension(copy_reference(other)));
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    m_attached = false;
    this->text(other.text());
    this->text_3d(other.text_3d());
    this->arrow_type(other.arrow_type());
  }
  DrawingElement::operator=(other);
  return *this;
}


/*******************
** Public Methods **
********************/
SUDimensionLinearRef DimensionLinear::linear_ref() const {
  return SUDimensionLinearFromDimension(this->ref());
}


Point3D DimensionLinear::start_point() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::start_point(): DimensionLinear is null");
  }
  SUPoint3D point;
  SUInstancePathRef path = SU_INVALID;
  SUResult res = SUDimensionLinearGetStartPoint(this->linear_ref(), &point, &path);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (SUIsValid(path)) {
    SUInstancePathRelease(&path);
  }
  return Point3D(point);
}


void DimensionLinear::start_point(const Point3D& point) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::start_point(): DimensionLinear is null");
  }
  SUPoint3D su_point = point;
  SUResult res = SUDimensionLinearSetStartPoint(this->linear_ref(), &su_point, SU_INVALID);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Point3D DimensionLinear::end_point() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::end_point(): DimensionLinear is null");
  }
  SUPoint3D point;
  SUInstancePathRef path = SU_INVALID;
  SUResult res = SUDimensionLinearGetEndPoint(this->linear_ref(), &point, &path);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (SUIsValid(path)) {
    SUInstancePathRelease(&path);
  }
  return Point3D(point);
}


void DimensionLinear::end_point(const Point3D& point) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::end_point(): DimensionLinear is null");
  }
  SUPoint3D su_point = point;
  SUResult res = SUDimensionLinearSetEndPoint(this->linear_ref(), &su_point, SU_INVALID);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Vector3D DimensionLinear::x_axis() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::x_axis(): DimensionLinear is null");
  }
  SUVector3D axis;
  SUResult res = SUDimensionLinearGetXAxis(this->linear_ref(), &axis);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(axis);
}


void DimensionLinear::x_axis(const Vector3D& axis) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::x_axis(): DimensionLinear is null");
  }
  SUVector3D su_axis = axis;
  SUResult res = SUDimensionLinearSetXAxis(this->linear_ref(), &su_axis);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Vector3D DimensionLinear::normal() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::normal(): DimensionLinear is null");
  }
  SUVector3D normal;
  SUResult res = SUDimensionLinearGetNormal(this->linear_ref(), &normal);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(normal);
}


void DimensionLinear::normal(const Vector3D& normal) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::normal(): DimensionLinear is null");
  }
  SUVector3D su_normal = normal;
  SUResult res = SUDimensionLinearSetNormal(this->linear_ref(), &su_normal);
  assert(res == SU_ERROR_NONE); _unused(res);
}


std::pair<double, double> DimensionLinear::position() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::position(): DimensionLinear is null");
  }
  SUPoint2D pos;
  SUResult res = SUDimensionLinearGetPosition(this->linear_ref(), &pos);
  assert(res == SU_ERROR_NONE); _unused(res);
  return std::make_pair(pos.x, pos.y);
}


void DimensionLinear::position(double x, double y) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::position(): DimensionLinear is null");
  }
  SUPoint2D pos{x, y};
  SUResult res = SUDimensionLinearSetPosition(this->linear_ref(), &pos);
  assert(res == SU_ERROR_NONE); _unused(res);
}


SUVerticalTextPositionType DimensionLinear::vertical_alignment() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::vertical_alignment(): DimensionLinear is null");
  }
  SUVerticalTextPositionType alignment;
  SUResult res = SUDimensionLinearGetVerticalAlignment(this->linear_ref(), &alignment);
  assert(res == SU_ERROR_NONE); _unused(res);
  return alignment;
}


void DimensionLinear::vertical_alignment(SUVerticalTextPositionType alignment) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::vertical_alignment(): DimensionLinear is null");
  }
  SUResult res = SUDimensionLinearSetVerticalAlignment(this->linear_ref(), alignment);
  assert(res == SU_ERROR_NONE); _unused(res);
}


SUHorizontalTextPositionType DimensionLinear::horizontal_alignment() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::horizontal_alignment(): DimensionLinear is null");
  }
  SUHorizontalTextPositionType alignment;
  SUResult res = SUDimensionLinearGetHorizontalAlignment(this->linear_ref(), &alignment);
  assert(res == SU_ERROR_NONE); _unused(res);
  return alignment;
}


void DimensionLinear::horizontal_alignment(SUHorizontalTextPositionType alignment) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::horizontal_alignment(): DimensionLinear is null");
  }
  SUResult res = SUDimensionLinearSetHorizontalAlignment(this->linear_ref(), alignment);
  assert(res == SU_ERROR_NONE); _unused(res);
}


SUDimensionLinearAlignmentType DimensionLinear::alignment() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::alignment(): DimensionLinear is null");
  }
  SUDimensionLinearAlignmentType alignment;
  SUResult res = SUDimensionLinearGetAlignment(this->linear_ref(), &alignment);
  assert(res == SU_ERROR_NONE); _unused(res);
  return alignment;
}


Point3D DimensionLinear::text_position() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionLinear::text_position(): DimensionLinear is null");
  }
  SUPoint3D point;
  SUResult res = SUDimensionLinearGetTextPosition(this->linear_ref(), &point);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(point);
}


} /* namespace CW */
