//
//  GuideLine.cpp
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

#include "SUAPI-CppWrapper/model/GuideLine.hpp"

#include <cassert>
#include <stdexcept>

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUGuideLineRef GuideLine::create_guide_line_finite(const Point3D& start, const Point3D& end) {
  SUGuideLineRef guide_line = SU_INVALID;
  SUPoint3D start_pt = start;
  SUPoint3D end_pt = end;
  SUResult res = SUGuideLineCreateFinite(&guide_line, &start_pt, &end_pt);
  if (res != SU_ERROR_NONE) {
    throw std::invalid_argument("CW::GuideLine::create_guide_line_finite(): failed to create finite guide line");
  }
  return guide_line;
}


SUGuideLineRef GuideLine::create_guide_line_infinite(const Point3D& point, const Vector3D& direction) {
  SUGuideLineRef guide_line = SU_INVALID;
  SUPoint3D pt = point;
  SUVector3D dir = direction;
  SUResult res = SUGuideLineCreateInfinite(&guide_line, &pt, &dir);
  if (res != SU_ERROR_NONE) {
    throw std::invalid_argument("CW::GuideLine::create_guide_line_infinite(): failed to create infinite guide line");
  }
  return guide_line;
}


SUGuideLineRef GuideLine::copy_reference(const GuideLine& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  // Get the data from the other guide line and create a copy
  SUPoint3D start_pt;
  SUVector3D dir;
  bool isinfinite;
  SUResult res = SUGuideLineGetData(other.ref(), &start_pt, &dir, &isinfinite);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (isinfinite) {
    return create_guide_line_infinite(Point3D(start_pt), Vector3D(dir));
  } else {
    Point3D end_pt(start_pt.x + dir.x, start_pt.y + dir.y, start_pt.z + dir.z);
    return create_guide_line_finite(Point3D(start_pt), end_pt);
  }
}


/******************************
** Constructors / Destructor **
*******************************/
GuideLine::GuideLine():
  DrawingElement(SU_INVALID, false)
{}


GuideLine::GuideLine(const Point3D& start, const Point3D& end):
  DrawingElement(SUGuideLineToDrawingElement(create_guide_line_finite(start, end)), false)
{}


GuideLine::GuideLine(SUGuideLineRef guide_line, bool attached):
  DrawingElement(SUGuideLineToDrawingElement(guide_line), attached)
{}


GuideLine::GuideLine(const GuideLine& other):
  DrawingElement(other, SUGuideLineToDrawingElement(copy_reference(other)))
{}


GuideLine::~GuideLine() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUGuideLineRef guide_line = this->ref();
    SUResult res = SUGuideLineRelease(&guide_line);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


/*******************
** Public Methods **
********************/
GuideLine& GuideLine::operator=(const GuideLine& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUGuideLineRef guide_line = this->ref();
    SUResult res = SUGuideLineRelease(&guide_line);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUGuideLineToEntity(copy_reference(other));
  DrawingElement::operator=(other);
  return (*this);
}


GuideLine GuideLine::create_finite(const Point3D& start, const Point3D& end) {
  return GuideLine(create_guide_line_finite(start, end), false);
}


GuideLine GuideLine::create_infinite(const Point3D& point, const Vector3D& direction) {
  return GuideLine(create_guide_line_infinite(point, direction), false);
}


SUGuideLineRef GuideLine::ref() const {
  return SUGuideLineFromEntity(m_entity);
}

GuideLine::operator SUGuideLineRef() const {
  return this->ref();
}

GuideLine::operator SUGuideLineRef*() {
  return reinterpret_cast<SUGuideLineRef*>(&m_entity);
}


bool GuideLine::operator!() const {
  return SUIsInvalid(m_entity);
}


bool GuideLine::get_data(Point3D& start, Vector3D& direction) const {
  if (!(*this)) {
    throw std::logic_error("CW::GuideLine::get_data(): GuideLine is null");
  }
  SUPoint3D start_pt;
  SUVector3D dir;
  bool isinfinite;
  SUResult res = SUGuideLineGetData(this->ref(), &start_pt, &dir, &isinfinite);
  assert(res == SU_ERROR_NONE); _unused(res);
  start = Point3D(start_pt);
  direction = Vector3D(dir);
  return isinfinite;
}


bool GuideLine::is_infinite() const {
  Point3D start;
  Vector3D direction;
  return get_data(start, direction);
}


Point3D GuideLine::start() const {
  Point3D start_pt;
  Vector3D dir;
  get_data(start_pt, dir);
  return start_pt;
}


Vector3D GuideLine::direction() const {
  Point3D start_pt;
  Vector3D dir;
  get_data(start_pt, dir);
  return dir;
}


Point3D GuideLine::end() const {
  Point3D start_pt;
  Vector3D dir;
  bool infinite = get_data(start_pt, dir);
  if (infinite) {
    throw std::logic_error("CW::GuideLine::end(): Guide line is infinite, no end point");
  }
  return Point3D(start_pt.x + dir.x, start_pt.y + dir.y, start_pt.z + dir.z);
}


GuideLine GuideLine::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::GuideLine::copy(): GuideLine is null");
  }
  Point3D start_pt;
  Vector3D dir;
  bool infinite = get_data(start_pt, dir);
  GuideLine new_guide_line;
  if (infinite) {
    new_guide_line = create_infinite(start_pt, dir);
  } else {
    Point3D end_pt(start_pt.x + dir.x, start_pt.y + dir.y, start_pt.z + dir.z);
    new_guide_line = create_finite(start_pt, end_pt);
  }
  new_guide_line.copy_attributes_from(*this);
  return new_guide_line;
}


} /* namespace CW */
