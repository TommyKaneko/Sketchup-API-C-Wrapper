//
//  ClassificationAttribute.cpp
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

#include "SUAPI-CppWrapper/model/ClassificationAttribute.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
ClassificationAttribute::ClassificationAttribute():
  m_attribute(SU_INVALID)
{}


ClassificationAttribute::ClassificationAttribute(SUClassificationAttributeRef attribute):
  m_attribute(attribute)
{}


/*******************
** Public Methods **
********************/
SUClassificationAttributeRef ClassificationAttribute::ref() const {
  return m_attribute;
}


bool ClassificationAttribute::operator!() const {
  return SUIsInvalid(m_attribute);
}


TypedValue ClassificationAttribute::value() const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationAttribute::value(): ClassificationAttribute is null");
  }
  SUTypedValueRef tv = SU_INVALID;
  SUTypedValueCreate(&tv);
  SUResult res = SUClassificationAttributeGetValue(m_attribute, &tv);
  assert(res == SU_ERROR_NONE); _unused(res);
  return TypedValue(tv, false);
}


String ClassificationAttribute::path() const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationAttribute::path(): ClassificationAttribute is null");
  }
  String path;
  SUStringRef path_ref = path.ref();
  SUResult res = SUClassificationAttributeGetPath(m_attribute, &path_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return path;
}


size_t ClassificationAttribute::num_children() const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationAttribute::num_children(): ClassificationAttribute is null");
  }
  size_t count = 0;
  SUResult res = SUClassificationAttributeGetNumChildren(m_attribute, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


ClassificationAttribute ClassificationAttribute::child(size_t index) const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationAttribute::child(): ClassificationAttribute is null");
  }
  SUClassificationAttributeRef child_ref = SU_INVALID;
  SUResult res = SUClassificationAttributeGetChild(m_attribute, index, &child_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ClassificationAttribute(child_ref);
}

} /* namespace CW */
