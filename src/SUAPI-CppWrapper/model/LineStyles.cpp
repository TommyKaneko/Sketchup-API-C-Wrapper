//
//  LineStyles.cpp
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

#include "SUAPI-CppWrapper/model/LineStyles.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/LineStyle.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
LineStyles::LineStyles():
  m_line_styles(SU_INVALID)
{}


LineStyles::LineStyles(SULineStylesRef line_styles):
  m_line_styles(line_styles)
{}


/*******************
** Public Methods **
********************/
SULineStylesRef LineStyles::ref() const {
  return m_line_styles;
}


bool LineStyles::operator!() const {
  return SUIsInvalid(m_line_styles);
}


size_t LineStyles::size() const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyles::size(): LineStyles is null");
  }
  size_t count = 0;
  SUResult res = SULineStylesGetNumLineStyles(m_line_styles, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


std::vector<String> LineStyles::names() const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyles::names(): LineStyles is null");
  }
  size_t count = 0;
  SUResult res = SULineStylesGetNumLineStyles(m_line_styles, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<String>(0);
  }
  std::vector<SUStringRef> refs(count, SU_INVALID);
  for (size_t i = 0; i < count; ++i) {
    SUStringCreate(&refs[i]);
  }
  res = SULineStylesGetLineStyleNames(m_line_styles, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<String> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(String(refs[i]));
  }
  return result;
}


LineStyle LineStyles::line_style_by_name(const std::string& name) const {
  if (!(*this)) {
    throw std::logic_error("CW::LineStyles::line_style_by_name(): LineStyles is null");
  }
  SULineStyleRef style = SU_INVALID;
  SUResult res = SULineStylesGetLineStyleByName(m_line_styles, name.c_str(), &style);
  if (res != SU_ERROR_NONE) {
    return LineStyle(); // Not found
  }
  return LineStyle(style, true);
}


} /* namespace CW */
