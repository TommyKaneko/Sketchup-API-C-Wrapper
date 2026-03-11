//
//  DimensionRadial.cpp
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

#include "SUAPI-CppWrapper/model/DimensionRadial.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/InstancePath.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
DimensionRadial::DimensionRadial():
  Dimension()
{}


DimensionRadial::DimensionRadial(SUDimensionRadialRef dimension, bool attached):
  Dimension(SUDimensionRadialToDimension(dimension), attached)
{}


DimensionRadial::~DimensionRadial() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUDimensionRadialRef dim = this->radial_ref();
    SUResult res = SUDimensionRadialRelease(&dim);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


/*******************
** Public Methods **
********************/
SUDimensionRadialRef DimensionRadial::radial_ref() const {
  return SUDimensionRadialFromDimension(this->ref());
}


InstancePath DimensionRadial::curve_instance_path() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionRadial::curve_instance_path(): DimensionRadial is null");
  }
  SUInstancePathRef path = SU_INVALID;
  SUResult res = SUDimensionRadialGetCurveInstancePath(this->radial_ref(), &path);
  assert(res == SU_ERROR_NONE); _unused(res);
  return InstancePath(path);
}


void DimensionRadial::curve_instance_path(const InstancePath& path) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionRadial::curve_instance_path(): DimensionRadial is null");
  }
  SUResult res = SUDimensionRadialSetCurveInstancePath(this->radial_ref(), path.ref());
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_INVALID_ARGUMENT:
      throw std::invalid_argument("CW::DimensionRadial::curve_instance_path(): Invalid instance path");
    case SU_ERROR_GENERIC:
      throw std::invalid_argument("CW::DimensionRadial::curve_instance_path(): Instance path leaf is not an arc curve");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
}


bool DimensionRadial::is_diameter() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionRadial::is_diameter(): DimensionRadial is null");
  }
  bool diameter;
  SUResult res = SUDimensionRadialGetDiameter(this->radial_ref(), &diameter);
  assert(res == SU_ERROR_NONE); _unused(res);
  return diameter;
}


void DimensionRadial::is_diameter(bool diameter) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionRadial::is_diameter(): DimensionRadial is null");
  }
  SUResult res = SUDimensionRadialSetDiameter(this->radial_ref(), diameter);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Point3D DimensionRadial::leader_break_point() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionRadial::leader_break_point(): DimensionRadial is null");
  }
  SUPoint3D point;
  SUResult res = SUDimensionRadialGetLeaderBreakPoint(this->radial_ref(), &point);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(point);
}


void DimensionRadial::leader_break_point(const Point3D& point) {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionRadial::leader_break_point(): DimensionRadial is null");
  }
  SUPoint3D su_point = point;
  SUResult res = SUDimensionRadialSetLeaderBreakPoint(this->radial_ref(), &su_point);
  assert(res == SU_ERROR_NONE); _unused(res);
}


std::array<Point3D, 3> DimensionRadial::leader_points() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionRadial::leader_points(): DimensionRadial is null");
  }
  SUPoint3D points[3];
  SUResult res = SUDimensionRadialGetLeaderPoints(this->radial_ref(), points);
  assert(res == SU_ERROR_NONE); _unused(res);
  return {Point3D(points[0]), Point3D(points[1]), Point3D(points[2])};
}


} /* namespace CW */
