//
//  DimensionStyle.cpp
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

#include "SUAPI-CppWrapper/model/DimensionStyle.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/model/Font.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
DimensionStyle::DimensionStyle():
  m_style(SU_INVALID)
{}


DimensionStyle::DimensionStyle(SUDimensionStyleRef style):
  m_style(style)
{}


/*******************
** Public Methods **
********************/
SUDimensionStyleRef DimensionStyle::ref() const {
  return m_style;
}


bool DimensionStyle::operator!() const {
  return SUIsInvalid(m_style);
}


Font DimensionStyle::font() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::font(): DimensionStyle is null");
  }
  SUFontRef font = SU_INVALID;
  SUResult res = SUDimensionStyleGetFont(m_style, &font);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Font(font);
}


bool DimensionStyle::has_3d() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::has_3d(): DimensionStyle is null");
  }
  bool has_3d;
  SUResult res = SUDimensionStyleGet3D(m_style, &has_3d);
  assert(res == SU_ERROR_NONE); _unused(res);
  return has_3d;
}


SUArrowType DimensionStyle::arrow_type() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::arrow_type(): DimensionStyle is null");
  }
  SUArrowType type;
  SUResult res = SUDimensionStyleGetArrowType(m_style, &type);
  assert(res == SU_ERROR_NONE); _unused(res);
  return type;
}


size_t DimensionStyle::arrow_size() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::arrow_size(): DimensionStyle is null");
  }
  size_t size;
  SUResult res = SUDimensionStyleGetArrowSize(m_style, &size);
  assert(res == SU_ERROR_NONE); _unused(res);
  return size;
}


Color DimensionStyle::color() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::color(): DimensionStyle is null");
  }
  SUColor color;
  SUResult res = SUDimensionStyleGetColor(m_style, &color);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Color(color);
}


Color DimensionStyle::text_color() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::text_color(): DimensionStyle is null");
  }
  SUColor color;
  SUResult res = SUDimensionStyleGetTextColor(m_style, &color);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Color(color);
}


size_t DimensionStyle::extension_line_offset() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::extension_line_offset(): DimensionStyle is null");
  }
  size_t offset;
  SUResult res = SUDimensionStyleGetExtensionLineOffset(m_style, &offset);
  assert(res == SU_ERROR_NONE); _unused(res);
  return offset;
}


size_t DimensionStyle::extension_line_overshoot() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::extension_line_overshoot(): DimensionStyle is null");
  }
  size_t overshoot;
  SUResult res = SUDimensionStyleGetExtensionLineOvershoot(m_style, &overshoot);
  assert(res == SU_ERROR_NONE); _unused(res);
  return overshoot;
}


size_t DimensionStyle::line_weight() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::line_weight(): DimensionStyle is null");
  }
  size_t weight;
  SUResult res = SUDimensionStyleGetLineWeight(m_style, &weight);
  assert(res == SU_ERROR_NONE); _unused(res);
  return weight;
}


bool DimensionStyle::highlight_non_associative() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::highlight_non_associative(): DimensionStyle is null");
  }
  bool highlight;
  SUResult res = SUDimensionStyleGetHighlightNonAssociativeDimensions(m_style, &highlight);
  assert(res == SU_ERROR_NONE); _unused(res);
  return highlight;
}


Color DimensionStyle::highlight_non_associative_color() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::highlight_non_associative_color(): DimensionStyle is null");
  }
  SUColor color;
  SUResult res = SUDimensionStyleGetHighlightNonAssociativeDimensionsColor(m_style, &color);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Color(color);
}


bool DimensionStyle::show_radial_prefix() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::show_radial_prefix(): DimensionStyle is null");
  }
  bool show;
  SUResult res = SUDimensionStyleGetShowRadialPrefix(m_style, &show);
  assert(res == SU_ERROR_NONE); _unused(res);
  return show;
}


bool DimensionStyle::hide_out_of_plane() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::hide_out_of_plane(): DimensionStyle is null");
  }
  bool hide;
  SUResult res = SUDimensionStyleGetHideOutOfPlane(m_style, &hide);
  assert(res == SU_ERROR_NONE); _unused(res);
  return hide;
}


double DimensionStyle::hide_out_of_plane_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::hide_out_of_plane_value(): DimensionStyle is null");
  }
  double tolerance;
  SUResult res = SUDimensionStyleGetHideOutOfPlaneValue(m_style, &tolerance);
  assert(res == SU_ERROR_NONE); _unused(res);
  return tolerance;
}


bool DimensionStyle::hide_small() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::hide_small(): DimensionStyle is null");
  }
  bool hide;
  SUResult res = SUDimensionStyleGetHideSmall(m_style, &hide);
  assert(res == SU_ERROR_NONE); _unused(res);
  return hide;
}


double DimensionStyle::hide_small_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::DimensionStyle::hide_small_value(): DimensionStyle is null");
  }
  double tolerance;
  SUResult res = SUDimensionStyleGetHideSmallValue(m_style, &tolerance);
  assert(res == SU_ERROR_NONE); _unused(res);
  return tolerance;
}


} /* namespace CW */
