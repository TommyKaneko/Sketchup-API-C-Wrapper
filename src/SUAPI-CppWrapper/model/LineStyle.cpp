//
//  LineStyle.cpp
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

#include "SUAPI-CppWrapper/model/LineStyle.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/Color.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SULineStyleRef LineStyle::copy_reference(const LineStyle& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  // Create a copy with the same name
  SULineStyleRef copy = SU_INVALID;
  SUStringRef name_ref = SU_INVALID;
  SUStringCreate(&name_ref);
  SULineStyleGetName(other.ref(), &name_ref);
  size_t length = 0;
  SUStringGetUTF8Length(name_ref, &length);
  std::string name(length, '\0');
  SUStringGetUTF8(name_ref, length + 1, &name[0], &length);
  SUStringRelease(&name_ref);
  SUResult res = SULineStyleCreateCopy(other.ref(), name.c_str(), &copy);
  assert(res == SU_ERROR_NONE); _unused(res);
  return copy;
}


/******************************
** Constructors / Destructor **
*******************************/
LineStyle::LineStyle():
  Entity(SU_INVALID, true)
{}


LineStyle::LineStyle(SULineStyleRef line_style, bool attached):
  Entity(SULineStyleToEntity(line_style), attached)
{}


LineStyle::LineStyle(const LineStyle& other):
  Entity(other, SULineStyleToEntity(copy_reference(other)))
{}


LineStyle::~LineStyle() {
  if (!m_attached && SUIsValid(m_entity)) {
    SULineStyleRef style = this->ref();
    SUResult res = SULineStyleRelease(&style);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


LineStyle& LineStyle::operator=(const LineStyle& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SULineStyleRef style = this->ref();
    SUResult res = SULineStyleRelease(&style);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SULineStyleToEntity(copy_reference(other));
  Entity::operator=(other);
  return *this;
}


/*******************
** Public Methods **
********************/
SULineStyleRef LineStyle::ref() const {
  return SULineStyleFromEntity(m_entity);
}


bool LineStyle::operator!() const {
  return SUIsInvalid(m_entity);
}


LineStyle LineStyle::create_copy(const std::string& name) const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::create_copy(): LineStyle is null");
  }
  SULineStyleRef copy = SU_INVALID;
  SUResult res = SULineStyleCreateCopy(this->ref(), name.c_str(), &copy);
  assert(res == SU_ERROR_NONE); _unused(res);
  return LineStyle(copy, false);
}


String LineStyle::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::name(): LineStyle is null");
  }
  SUStringRef name_ref = SU_INVALID;
  SUStringCreate(&name_ref);
  SUResult res = SULineStyleGetName(this->ref(), &name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return String(name_ref);
}


double LineStyle::width_pixels() const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::width_pixels(): LineStyle is null");
  }
  double width;
  SUResult res = SULineStyleGetWidthPixels(this->ref(), &width);
  assert(res == SU_ERROR_NONE); _unused(res);
  return width;
}


void LineStyle::width_pixels(double width) {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::width_pixels(): LineStyle is null");
  }
  SUResult res = SULineStyleSetWidthPixels(this->ref(), width);
  if (res == SU_ERROR_INVALID_ARGUMENT) {
    throw std::invalid_argument("CW::LineStyle::width_pixels(): Cannot modify built-in line style or invalid width");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
}


double LineStyle::length_multiplier() const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::length_multiplier(): LineStyle is null");
  }
  double multiplier;
  SUResult res = SULineStyleGetLengthMultiplier(this->ref(), &multiplier);
  assert(res == SU_ERROR_NONE); _unused(res);
  return multiplier;
}


void LineStyle::length_multiplier(double multiplier) {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::length_multiplier(): LineStyle is null");
  }
  SUResult res = SULineStyleSetLengthMultiplier(this->ref(), multiplier);
  if (res == SU_ERROR_INVALID_ARGUMENT) {
    throw std::invalid_argument("CW::LineStyle::length_multiplier(): Cannot modify built-in line style or invalid multiplier");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
}


Color LineStyle::color(bool& has_color) const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::color(): LineStyle is null");
  }
  SUColor su_color;
  SUResult res = SULineStyleGetColor(this->ref(), &su_color);
  if (res == SU_ERROR_NO_DATA) {
    has_color = false;
    return Color();
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  has_color = true;
  return Color(su_color);
}


void LineStyle::color(const Color& color) {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::color(): LineStyle is null");
  }
  SUColor su_color = color.ref();
  SUResult res = SULineStyleSetColor(this->ref(), &su_color);
  if (res == SU_ERROR_INVALID_ARGUMENT) {
    throw std::invalid_argument("CW::LineStyle::color(): Cannot modify built-in line style");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool LineStyle::is_user_created() const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyle::is_user_created(): LineStyle is null");
  }
  bool user_created;
  SUResult res = SULineStyleIsUserCreated(this->ref(), &user_created);
  assert(res == SU_ERROR_NONE); _unused(res);
  return user_created;
}


} /* namespace CW */
