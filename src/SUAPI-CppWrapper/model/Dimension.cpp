//
//  Dimension.cpp
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

#include "SUAPI-CppWrapper/model/Dimension.hpp"
#include "SUAPI-CppWrapper/model/DimensionLinear.hpp"
#include "SUAPI-CppWrapper/model/DimensionRadial.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/Font.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
Dimension::Dimension():
  DrawingElement(SU_INVALID, false)
{}


Dimension::Dimension(SUDimensionRef dimension, bool attached):
  DrawingElement(SUDimensionToDrawingElement(dimension), attached)
{}


Dimension::Dimension(const Dimension& other):
  DrawingElement(other, SUDimensionToDrawingElement(other.ref()))
{}


Dimension& Dimension::operator=(const Dimension& other) {
  m_entity = SUDimensionToEntity(other.ref());
  DrawingElement::operator=(other);
  return *this;
}


/*******************
** Public Methods **
********************/
SUDimensionRef Dimension::ref() const {
  return SUDimensionFromEntity(m_entity);
}


Dimension::operator SUDimensionRef() const {
  return this->ref();
}


Dimension::operator SUDimensionRef*() {
  return reinterpret_cast<SUDimensionRef*>(&m_entity);
}


bool Dimension::operator!() const {
  return SUIsInvalid(m_entity);
}


DimensionLinear Dimension::as_linear() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::as_linear(): Dimension is null");
  }
  if (this->type() != SUDimensionType_Linear) {
    throw std::logic_error("CW::Dimension::as_linear(): Dimension is not linear");
  }
  return DimensionLinear(SUDimensionLinearFromDimension(this->ref()), this->attached());
}


DimensionRadial Dimension::as_radial() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::as_radial(): Dimension is null");
  }
  if (this->type() != SUDimensionType_Radial) {
    throw std::logic_error("CW::Dimension::as_radial(): Dimension is not radial");
  }
  return DimensionRadial(SUDimensionRadialFromDimension(this->ref()), this->attached());
}


SUDimensionType Dimension::type() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::type(): Dimension is null");
  }
  SUDimensionType dim_type;
  SUResult res = SUDimensionGetType(this->ref(), &dim_type);
  assert(res == SU_ERROR_NONE); _unused(res);
  return dim_type;
}


String Dimension::text() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::text(): Dimension is null");
  }
  String string;
  SUResult res = SUDimensionGetText(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


void Dimension::text(const String& text) {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::text(): Dimension is null");
  }
  SUResult res = SUDimensionSetText(this->ref(), text.std_string().c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Dimension::text(const std::string& text) {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::text(): Dimension is null");
  }
  SUResult res = SUDimensionSetText(this->ref(), text.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


Plane3D Dimension::plane() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::plane(): Dimension is null");
  }
  SUPlane3D su_plane;
  SUResult res = SUDimensionGetPlane(this->ref(), &su_plane);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Plane3D(su_plane);
}


bool Dimension::text_3d() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::text_3d(): Dimension is null");
  }
  bool is_3d;
  SUResult res = SUDimensionGetText3D(this->ref(), &is_3d);
  assert(res == SU_ERROR_NONE); _unused(res);
  return is_3d;
}


void Dimension::text_3d(bool is_3d) {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::text_3d(): Dimension is null");
  }
  SUResult res = SUDimensionSetText3D(this->ref(), is_3d);
  assert(res == SU_ERROR_NONE); _unused(res);
}


SUArrowType Dimension::arrow_type() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::arrow_type(): Dimension is null");
  }
  SUArrowType type;
  SUResult res = SUDimensionGetArrowType(this->ref(), &type);
  assert(res == SU_ERROR_NONE); _unused(res);
  return type;
}


void Dimension::arrow_type(SUArrowType type) {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::arrow_type(): Dimension is null");
  }
  SUResult res = SUDimensionSetArrowType(this->ref(), type);
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_OUT_OF_RANGE:
      throw std::out_of_range("CW::Dimension::arrow_type(): Invalid arrow type value");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
}


Font Dimension::font() const {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::font(): Dimension is null");
  }
  SUFontRef font = SU_INVALID;
  SUResult res = SUDimensionGetFont(this->ref(), &font);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Font(font);
}


void Dimension::font(const Font& font) {
  if (!(*this)) {
    throw std::logic_error("CW::Dimension::font(): Dimension is null");
  }
  SUResult res = SUDimensionSetFont(this->ref(), font.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


} /* namespace CW */
