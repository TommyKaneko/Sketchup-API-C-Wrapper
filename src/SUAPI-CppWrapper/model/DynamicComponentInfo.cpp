//
//  DynamicComponentInfo.cpp
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

#include "SUAPI-CppWrapper/model/DynamicComponentInfo.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/DynamicComponentAttribute.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
DynamicComponentInfo::DynamicComponentInfo():
  m_dc_info(SU_INVALID)
{}


DynamicComponentInfo::DynamicComponentInfo(SUDynamicComponentInfoRef dc_info):
  m_dc_info(dc_info)
{}


DynamicComponentInfo::DynamicComponentInfo(DynamicComponentInfo&& other) noexcept:
  m_dc_info(other.m_dc_info)
{
  other.m_dc_info = SU_INVALID;
}


DynamicComponentInfo& DynamicComponentInfo::operator=(DynamicComponentInfo&& other) noexcept {
  if (this != &other) {
    if (!SUIsInvalid(m_dc_info)) {
      SUDynamicComponentInfoRelease(&m_dc_info);
    }
    m_dc_info = other.m_dc_info;
    other.m_dc_info = SU_INVALID;
  }
  return *this;
}


DynamicComponentInfo::~DynamicComponentInfo() {
  if (!SUIsInvalid(m_dc_info)) {
    SUDynamicComponentInfoRelease(&m_dc_info);
  }
}


/*******************
** Public Methods **
********************/
SUDynamicComponentInfoRef DynamicComponentInfo::ref() const {
  return m_dc_info;
}


bool DynamicComponentInfo::operator!() const {
  return SUIsInvalid(m_dc_info);
}


size_t DynamicComponentInfo::num_attributes() const {
  if (!(*this)) {
    throw std::logic_error("CW::DynamicComponentInfo::num_attributes(): DynamicComponentInfo is null");
  }
  size_t count = 0;
  SUResult res = SUDynamicComponentInfoGetNumDCAttributes(m_dc_info, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


std::vector<DynamicComponentAttribute> DynamicComponentInfo::attributes() const {
  if (!(*this)) {
    throw std::logic_error("CW::DynamicComponentInfo::attributes(): DynamicComponentInfo is null");
  }
  size_t count = 0;
  SUResult res = SUDynamicComponentInfoGetNumDCAttributes(m_dc_info, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<DynamicComponentAttribute>();
  }
  std::vector<SUDynamicComponentAttributeRef> refs(count, SU_INVALID);
  res = SUDynamicComponentInfoGetDCAttributes(m_dc_info, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<DynamicComponentAttribute> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(DynamicComponentAttribute(refs[i]));
  }
  return result;
}

} /* namespace CW */
