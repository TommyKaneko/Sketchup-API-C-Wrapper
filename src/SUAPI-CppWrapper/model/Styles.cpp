//
//  Styles.cpp
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

#include "SUAPI-CppWrapper/model/Styles.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/Style.hpp"
#include "SUAPI-CppWrapper/model/Scene.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
Styles::Styles():
  m_styles(SU_INVALID)
{}


Styles::Styles(SUStylesRef styles):
  m_styles(styles)
{}


/*******************
** Public Methods **
********************/
SUStylesRef Styles::ref() const {
  return m_styles;
}


bool Styles::operator!() const {
  return SUIsInvalid(m_styles);
}


void Styles::add_style(const std::string& path, bool activate) {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::add_style(): Styles is null");
  }
  SUResult res = SUStylesAddStyle(m_styles, path.c_str(), activate);
  assert(res == SU_ERROR_NONE); _unused(res);
}


size_t Styles::size() const {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::size(): Styles is null");
  }
  size_t count = 0;
  SUResult res = SUStylesGetNumStyles(m_styles, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


std::vector<Style> Styles::styles() const {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::styles(): Styles is null");
  }
  size_t count = 0;
  SUResult res = SUStylesGetNumStyles(m_styles, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Style>(0);
  }
  std::vector<SUStyleRef> refs(count, SU_INVALID);
  res = SUStylesGetStyles(m_styles, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Style> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.emplace_back(refs[i], true);
  }
  return result;
}


Style Styles::active_style() const {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::active_style(): Styles is null");
  }
  SUStyleRef style = SU_INVALID;
  SUResult res = SUStylesGetActiveStyle(m_styles, &style);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Style(style, true);
}


Style Styles::selected_style() const {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::selected_style(): Styles is null");
  }
  SUStyleRef style = SU_INVALID;
  SUResult res = SUStylesGetSelectedStyle(m_styles, &style);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Style(style, true);
}


void Styles::selected_style(const Style& style) {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::selected_style(): Styles is null");
  }
  SUResult res = SUStylesSetSelectedStyle(m_styles, style.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


Style Styles::style_by_guid(const std::string& guid) const {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::style_by_guid(): Styles is null");
  }
  SUStyleRef style = SU_INVALID;
  SUResult res = SUStylesGetStyleByGuid(m_styles, guid.c_str(), &style);
  if (res != SU_ERROR_NONE) {
    return Style(); // Not found
  }
  return Style(style, true);
}


Style Styles::style_by_path(const std::string& path) const {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::style_by_path(): Styles is null");
  }
  SUStyleRef style = SU_INVALID;
  SUResult res = SUStylesGetStyleByPath(m_styles, path.c_str(), &style);
  if (res != SU_ERROR_NONE) {
    return Style(); // Not found
  }
  return Style(style, true);
}


bool Styles::active_style_changed() const {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::active_style_changed(): Styles is null");
  }
  bool changed = false;
  SUResult res = SUStylesGetActiveStyleChanged(m_styles, &changed);
  assert(res == SU_ERROR_NONE); _unused(res);
  return changed;
}


void Styles::apply_style_to_scene(const Style& style, const Scene& scene) {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::apply_style_to_scene(): Styles is null");
  }
  SUResult res = SUStylesApplyStyleToScene(m_styles, style.ref(), scene.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Styles::remove_style(Style& style) {
  if (!(*this)) {
    throw std::logic_error("CW::Styles::remove_style(): Styles is null");
  }
  SUStyleRef style_ref = style.ref();
  SUResult res = SUStylesRemoveStyle(m_styles, &style_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
}

} /* namespace CW */
