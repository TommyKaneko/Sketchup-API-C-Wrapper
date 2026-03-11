//
//  ClassificationInfo.cpp
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

#include "SUAPI-CppWrapper/model/ClassificationInfo.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/ClassificationAttribute.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
ClassificationInfo::ClassificationInfo():
  m_classification_info(SU_INVALID)
{}


ClassificationInfo::ClassificationInfo(SUClassificationInfoRef info):
  m_classification_info(info)
{}


ClassificationInfo::ClassificationInfo(ClassificationInfo&& other) noexcept:
  m_classification_info(other.m_classification_info)
{
  other.m_classification_info = SU_INVALID;
}


ClassificationInfo& ClassificationInfo::operator=(ClassificationInfo&& other) noexcept {
  if (this != &other) {
    if (!SUIsInvalid(m_classification_info)) {
      SUClassificationInfoRelease(&m_classification_info);
    }
    m_classification_info = other.m_classification_info;
    other.m_classification_info = SU_INVALID;
  }
  return *this;
}


ClassificationInfo::~ClassificationInfo() {
  if (!SUIsInvalid(m_classification_info)) {
    SUClassificationInfoRelease(&m_classification_info);
  }
}


/*******************
** Public Methods **
********************/
SUClassificationInfoRef ClassificationInfo::ref() const {
  return m_classification_info;
}


bool ClassificationInfo::operator!() const {
  return SUIsInvalid(m_classification_info);
}


size_t ClassificationInfo::num_schemas() const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationInfo::num_schemas(): ClassificationInfo is null");
  }
  size_t count = 0;
  SUResult res = SUClassificationInfoGetNumSchemas(m_classification_info, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


String ClassificationInfo::schema_name(size_t index) const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationInfo::schema_name(): ClassificationInfo is null");
  }
  String name;
  SUStringRef name_ref = name.ref();
  SUResult res = SUClassificationInfoGetSchemaName(m_classification_info, index, &name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return name;
}


String ClassificationInfo::schema_type(size_t index) const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationInfo::schema_type(): ClassificationInfo is null");
  }
  String type;
  SUStringRef type_ref = type.ref();
  SUResult res = SUClassificationInfoGetSchemaType(m_classification_info, index, &type_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return type;
}


ClassificationAttribute ClassificationInfo::schema_attribute(size_t index) const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationInfo::schema_attribute(): ClassificationInfo is null");
  }
  SUClassificationAttributeRef attr_ref = SU_INVALID;
  SUResult res = SUClassificationInfoGetSchemaAttribute(m_classification_info, index, &attr_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ClassificationAttribute(attr_ref);
}


ClassificationAttribute ClassificationInfo::schema_attribute_by_path(const String& path) const {
  if (!(*this)) {
    throw std::logic_error("CW::ClassificationInfo::schema_attribute_by_path(): ClassificationInfo is null");
  }
  SUClassificationAttributeRef attr_ref = SU_INVALID;
  SUResult res = SUClassificationInfoGetSchemaAttributeByPath(m_classification_info, path.ref(), &attr_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ClassificationAttribute(attr_ref);
}

} /* namespace CW */
