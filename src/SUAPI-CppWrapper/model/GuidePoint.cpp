//
//  GuidePoint.cpp
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

#include "SUAPI-CppWrapper/model/GuidePoint.hpp"

#include <cassert>
#include <stdexcept>

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUGuidePointRef GuidePoint::create_guide_point(const Point3D& position) {
  SUGuidePointRef guide_point = SU_INVALID;
  SUPoint3D pos = position;
  SUResult res = SUGuidePointCreate(&guide_point, &pos);
  if (res != SU_ERROR_NONE) {
    throw std::invalid_argument("CW::GuidePoint::create_guide_point(): failed to create guide point");
  }
  return guide_point;
}


SUGuidePointRef GuidePoint::copy_reference(const GuidePoint& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  SUPoint3D pos;
  SUResult res = SUGuidePointGetPosition(other.ref(), &pos);
  assert(res == SU_ERROR_NONE); _unused(res);
  return create_guide_point(Point3D(pos));
}


/******************************
** Constructors / Destructor **
*******************************/
GuidePoint::GuidePoint():
  DrawingElement(SU_INVALID, false)
{}


GuidePoint::GuidePoint(const Point3D& position):
  DrawingElement(SUGuidePointToDrawingElement(create_guide_point(position)), false)
{}


GuidePoint::GuidePoint(SUGuidePointRef guide_point, bool attached):
  DrawingElement(SUGuidePointToDrawingElement(guide_point), attached)
{}


GuidePoint::GuidePoint(const GuidePoint& other):
  DrawingElement(other, SUGuidePointToDrawingElement(copy_reference(other)))
{}


GuidePoint::~GuidePoint() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUGuidePointRef guide_point = this->ref();
    SUResult res = SUGuidePointRelease(&guide_point);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


/*******************
** Public Methods **
********************/
GuidePoint& GuidePoint::operator=(const GuidePoint& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUGuidePointRef guide_point = this->ref();
    SUResult res = SUGuidePointRelease(&guide_point);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUGuidePointToEntity(copy_reference(other));
  DrawingElement::operator=(other);
  return (*this);
}


SUGuidePointRef GuidePoint::ref() const {
  return SUGuidePointFromEntity(m_entity);
}

GuidePoint::operator SUGuidePointRef() const {
  return this->ref();
}

GuidePoint::operator SUGuidePointRef*() {
  return reinterpret_cast<SUGuidePointRef*>(&m_entity);
}


bool GuidePoint::operator!() const {
  return SUIsInvalid(m_entity);
}


Point3D GuidePoint::position() const {
  if (!(*this)) {
    throw std::logic_error("CW::GuidePoint::position(): GuidePoint is null");
  }
  SUPoint3D pos;
  SUResult res = SUGuidePointGetPosition(this->ref(), &pos);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(pos);
}


Point3D GuidePoint::from_position() const {
  if (!(*this)) {
    throw std::logic_error("CW::GuidePoint::from_position(): GuidePoint is null");
  }
  SUPoint3D pos;
  SUResult res = SUGuidePointGetFromPosition(this->ref(), &pos);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(pos);
}


bool GuidePoint::display_as_line() const {
  if (!(*this)) {
    throw std::logic_error("CW::GuidePoint::display_as_line(): GuidePoint is null");
  }
  bool as_line;
  SUResult res = SUGuidePointGetDisplayAsLine(this->ref(), &as_line);
  assert(res == SU_ERROR_NONE); _unused(res);
  return as_line;
}


GuidePoint GuidePoint::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::GuidePoint::copy(): GuidePoint is null");
  }
  GuidePoint new_point(this->position());
  new_point.copy_attributes_from(*this);
  return new_point;
}


} /* namespace CW */
