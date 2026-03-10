//
//  SectionPlane.cpp
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

#include "SUAPI-CppWrapper/model/SectionPlane.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUSectionPlaneRef SectionPlane::create_section_plane() {
  SUSectionPlaneRef section_plane = SU_INVALID;
  SUResult res = SUSectionPlaneCreate(&section_plane);
  assert(res == SU_ERROR_NONE); _unused(res);
  return section_plane;
}


SUSectionPlaneRef SectionPlane::copy_reference(const SectionPlane& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  SUSectionPlaneRef new_sp = create_section_plane();
  // Copy plane data
  SUPlane3D plane;
  SUResult res = SUSectionPlaneGetPlane(other.ref(), &plane);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUSectionPlaneSetPlane(new_sp, &plane);
  assert(res == SU_ERROR_NONE); _unused(res);
  return new_sp;
}


/******************************
** Constructors / Destructor **
*******************************/
SectionPlane::SectionPlane():
  DrawingElement(SU_INVALID, false)
{}


SectionPlane::SectionPlane(const Plane3D& plane):
  SectionPlane(create_section_plane(), false)
{
  this->plane(plane);
}


SectionPlane::SectionPlane(SUSectionPlaneRef section_plane, bool attached):
  DrawingElement(SUSectionPlaneToDrawingElement(section_plane), attached)
{}


SectionPlane::SectionPlane(const SectionPlane& other):
  DrawingElement(other, SUSectionPlaneToDrawingElement(copy_reference(other)))
{
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    // Name and symbol are copied via the plane data above in copy_reference
  }
}


SectionPlane::~SectionPlane() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUSectionPlaneRef sp = this->ref();
    SUResult res = SUSectionPlaneRelease(&sp);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


/*******************
** Public Methods **
********************/
SectionPlane& SectionPlane::operator=(const SectionPlane& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUSectionPlaneRef sp = this->ref();
    SUResult res = SUSectionPlaneRelease(&sp);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUSectionPlaneToEntity(copy_reference(other));
  DrawingElement::operator=(other);
  return (*this);
}


SUSectionPlaneRef SectionPlane::ref() const {
  return SUSectionPlaneFromEntity(m_entity);
}

SectionPlane::operator SUSectionPlaneRef() const {
  return this->ref();
}

SectionPlane::operator SUSectionPlaneRef*() {
  return reinterpret_cast<SUSectionPlaneRef*>(&m_entity);
}


bool SectionPlane::operator!() const {
  return SUIsInvalid(m_entity);
}


void SectionPlane::plane(const Plane3D& plane) {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::plane(): SectionPlane is null");
  }
  SUPlane3D su_plane = plane;
  SUResult res = SUSectionPlaneSetPlane(this->ref(), &su_plane);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Plane3D SectionPlane::plane() const {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::plane(): SectionPlane is null");
  }
  SUPlane3D su_plane;
  SUResult res = SUSectionPlaneGetPlane(this->ref(), &su_plane);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Plane3D(su_plane);
}


bool SectionPlane::active() const {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::active(): SectionPlane is null");
  }
  bool is_active;
  SUResult res = SUSectionPlaneIsActive(this->ref(), &is_active);
  assert(res == SU_ERROR_NONE); _unused(res);
  return is_active;
}


void SectionPlane::name(const String& name) {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::name(): SectionPlane is null");
  }
  SUResult res = SUSectionPlaneSetName(this->ref(), name.std_string().c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void SectionPlane::name(const std::string& name) {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::name(): SectionPlane is null");
  }
  SUResult res = SUSectionPlaneSetName(this->ref(), name.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


String SectionPlane::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::name(): SectionPlane is null");
  }
  String string;
  SUResult res = SUSectionPlaneGetName(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


void SectionPlane::symbol(const String& symbol) {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::symbol(): SectionPlane is null");
  }
  SUResult res = SUSectionPlaneSetSymbol(this->ref(), symbol.std_string().c_str());
  switch (res) {
    case SU_ERROR_INVALID_INPUT: {
      throw std::invalid_argument("CW::SectionPlane::symbol(): symbol exceeds the maximum of 3 characters");
    }
    default: {
      assert(res == SU_ERROR_NONE); _unused(res);
    }
  }
}


void SectionPlane::symbol(const std::string& symbol) {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::symbol(): SectionPlane is null");
  }
  SUResult res = SUSectionPlaneSetSymbol(this->ref(), symbol.c_str());
  switch (res) {
    case SU_ERROR_INVALID_INPUT: {
      throw std::invalid_argument("CW::SectionPlane::symbol(): symbol exceeds the maximum of 3 characters");
    }
    default: {
      assert(res == SU_ERROR_NONE); _unused(res);
    }
  }
}


String SectionPlane::symbol() const {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::symbol(): SectionPlane is null");
  }
  String string;
  SUResult res = SUSectionPlaneGetSymbol(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


SectionPlane SectionPlane::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::SectionPlane::copy(): SectionPlane is null");
  }
  SectionPlane new_sp(this->plane());
  // Note: name and symbol are not copied here because they are reset when
  // the section plane is added to entities. Set them after adding to a model.
  new_sp.copy_attributes_from(*this);
  return new_sp;
}


} /* namespace CW */
