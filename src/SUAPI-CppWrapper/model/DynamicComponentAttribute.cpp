//
//  DynamicComponentAttribute.cpp
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

#include "SUAPI-CppWrapper/model/DynamicComponentAttribute.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
DynamicComponentAttribute::DynamicComponentAttribute():
  m_attribute(SU_INVALID)
{}


DynamicComponentAttribute::DynamicComponentAttribute(SUDynamicComponentAttributeRef attribute):
  m_attribute(attribute)
{}


/*******************
** Public Methods **
********************/
SUDynamicComponentAttributeRef DynamicComponentAttribute::ref() const {
  return m_attribute;
}


bool DynamicComponentAttribute::operator!() const {
  return SUIsInvalid(m_attribute);
}


String DynamicComponentAttribute::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::DynamicComponentAttribute::name(): DynamicComponentAttribute is null");
  }
  String name;
  SUStringRef name_ref = name.ref();
  SUResult res = SUDynamicComponentAttributeGetName(m_attribute, &name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return name;
}


String DynamicComponentAttribute::display_name() const {
  if (!(*this)) {
    throw std::logic_error("CW::DynamicComponentAttribute::display_name(): DynamicComponentAttribute is null");
  }
  String display_name;
  SUStringRef display_name_ref = display_name.ref();
  SUResult res = SUDynamicComponentAttributeGetDisplayName(m_attribute, &display_name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return display_name;
}


bool DynamicComponentAttribute::visible() const {
  if (!(*this)) {
    throw std::logic_error("CW::DynamicComponentAttribute::visible(): DynamicComponentAttribute is null");
  }
  bool visible = false;
  SUResult res = SUDynamicComponentAttributeGetVisibility(m_attribute, &visible);
  assert(res == SU_ERROR_NONE); _unused(res);
  return visible;
}


String DynamicComponentAttribute::display_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::DynamicComponentAttribute::display_value(): DynamicComponentAttribute is null");
  }
  String display_value;
  SUStringRef display_value_ref = display_value.ref();
  SUResult res = SUDynamicComponentAttributeGetDisplayValue(m_attribute, &display_value_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return display_value;
}

} /* namespace CW */
