//
//  Text.cpp
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

#include "SUAPI-CppWrapper/model/Text.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/Font.hpp"
#include "SUAPI-CppWrapper/model/InstancePath.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUTextRef Text::create_text() {
  SUTextRef text = SU_INVALID;
  SUResult res = SUTextCreate(&text);
  assert(res == SU_ERROR_NONE); _unused(res);
  return text;
}


SUTextRef Text::copy_reference(const Text& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  SUTextRef new_text = create_text();

  // Copy string
  SUStringRef str = SU_INVALID;
  SUStringCreate(&str);
  SUResult res = SUTextGetString(other.ref(), &str);
  assert(res == SU_ERROR_NONE); _unused(res);
  size_t length = 0;
  SUStringGetUTF8Length(str, &length);
  std::string text_str(length, '\0');
  SUStringGetUTF8(str, length + 1, &text_str[0], &length);
  SUStringRelease(&str);
  res = SUTextSetString(new_text, text_str.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy leader type
  SUTextLeaderType leader;
  res = SUTextGetLeaderType(other.ref(), &leader);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUTextSetLeaderType(new_text, leader);
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy arrow type
  SUArrowType arrow;
  res = SUTextGetArrowType(other.ref(), &arrow);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUTextSetArrowType(new_text, arrow);
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy point (simple form - no instance path)
  SUPoint3D point;
  SUInstancePathRef path = SU_INVALID;
  res = SUTextGetPoint(other.ref(), &point, &path);
  if (res == SU_ERROR_NONE) {
    // Set point without instance path for the copy
    res = SUTextSetPoint(new_text, &point, SU_INVALID);
    assert(res == SU_ERROR_NONE); _unused(res);
    if (SUIsValid(path)) {
      SUInstancePathRelease(&path);
    }
  }

  // Copy leader vector
  SUVector3D vector;
  res = SUTextGetLeaderVector(other.ref(), &vector);
  if (res == SU_ERROR_NONE) {
    SUTextSetLeaderVector(new_text, &vector);
  }

  // Copy color
  SUColor color;
  res = SUTextGetColor(other.ref(), &color);
  if (res == SU_ERROR_NONE) {
    SUTextSetColor(new_text, &color);
  }

  return new_text;
}


/******************************
** Constructors / Destructor **
*******************************/
Text::Text():
  DrawingElement(SU_INVALID, false)
{}


Text::Text(SUTextRef text, bool attached):
  DrawingElement(SUTextToDrawingElement(text), attached)
{}


Text::Text(const Text& other):
  DrawingElement(other, SUTextToDrawingElement(copy_reference(other)))
{
}


Text::~Text() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUTextRef text = this->ref();
    SUResult res = SUTextRelease(&text);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


/*******************
** Public Methods **
********************/
Text& Text::operator=(const Text& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUTextRef text = this->ref();
    SUResult res = SUTextRelease(&text);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUTextToEntity(copy_reference(other));
  DrawingElement::operator=(other);
  return (*this);
}


SUTextRef Text::ref() const {
  return SUTextFromEntity(m_entity);
}

Text::operator SUTextRef() const {
  return this->ref();
}

Text::operator SUTextRef*() {
  return reinterpret_cast<SUTextRef*>(&m_entity);
}


bool Text::operator!() const {
  return SUIsInvalid(m_entity);
}


void Text::string(const String& text_string) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::string(): Text is null");
  }
  SUResult res = SUTextSetString(this->ref(), text_string.std_string().c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Text::string(const std::string& text_string) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::string(): Text is null");
  }
  SUResult res = SUTextSetString(this->ref(), text_string.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


String Text::string() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::string(): Text is null");
  }
  String string;
  SUResult res = SUTextGetString(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


void Text::font(const Font& font) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::font(): Text is null");
  }
  SUResult res = SUTextSetFont(this->ref(), font.ref());
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_INVALID_INPUT:
      throw std::invalid_argument("CW::Text::font(): Invalid font");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
}


Font Text::font() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::font(): Text is null");
  }
  SUFontRef font = SU_INVALID;
  SUResult res = SUTextGetFont(this->ref(), &font);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Font(font);
}


void Text::leader_type(SUTextLeaderType leader) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::leader_type(): Text is null");
  }
  SUResult res = SUTextSetLeaderType(this->ref(), leader);
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_OUT_OF_RANGE:
      throw std::out_of_range("CW::Text::leader_type(): Invalid leader type value");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
}


SUTextLeaderType Text::leader_type() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::leader_type(): Text is null");
  }
  SUTextLeaderType leader;
  SUResult res = SUTextGetLeaderType(this->ref(), &leader);
  assert(res == SU_ERROR_NONE); _unused(res);
  return leader;
}


void Text::arrow_type(SUArrowType arrow) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::arrow_type(): Text is null");
  }
  SUResult res = SUTextSetArrowType(this->ref(), arrow);
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_OUT_OF_RANGE:
      throw std::out_of_range("CW::Text::arrow_type(): Invalid arrow type value");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
}


SUArrowType Text::arrow_type() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::arrow_type(): Text is null");
  }
  SUArrowType arrow;
  SUResult res = SUTextGetArrowType(this->ref(), &arrow);
  assert(res == SU_ERROR_NONE); _unused(res);
  return arrow;
}


void Text::point(const Point3D& point) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::point(): Text is null");
  }
  SUPoint3D su_point = point;
  SUResult res = SUTextSetPoint(this->ref(), &su_point, SU_INVALID);
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Text::point(const Point3D& point, const InstancePath& path) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::point(): Text is null");
  }
  SUPoint3D su_point = point;
  SUResult res = SUTextSetPoint(this->ref(), &su_point, path.ref());
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_INVALID_ARGUMENT:
      throw std::invalid_argument("CW::Text::point(): Instance path refers to an invalid path");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
}


Point3D Text::point() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::point(): Text is null");
  }
  SUPoint3D su_point;
  SUInstancePathRef path = SU_INVALID;
  SUResult res = SUTextGetPoint(this->ref(), &su_point, &path);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (SUIsValid(path)) {
    SUInstancePathRelease(&path);
  }
  return Point3D(su_point);
}


void Text::leader_vector(const Vector3D& vector) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::leader_vector(): Text is null");
  }
  SUVector3D su_vector = vector;
  SUResult res = SUTextSetLeaderVector(this->ref(), &su_vector);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Vector3D Text::leader_vector() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::leader_vector(): Text is null");
  }
  SUVector3D su_vector;
  SUResult res = SUTextGetLeaderVector(this->ref(), &su_vector);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(su_vector);
}


void Text::color(const Color& color) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::color(): Text is null");
  }
  SUColor su_color = color.ref();
  SUResult res = SUTextSetColor(this->ref(), &su_color);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Color Text::color() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::color(): Text is null");
  }
  SUColor su_color;
  SUResult res = SUTextGetColor(this->ref(), &su_color);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Color(su_color);
}


void Text::screen_position(double percent_x, double percent_y) {
  if (!(*this)) {
    throw std::logic_error("CW::Text::screen_position(): Text is null");
  }
  SUResult res = SUTextSetScreenPosition(this->ref(), percent_x, percent_y);
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_NO_DATA:
      throw std::logic_error("CW::Text::screen_position(): Text has a leader; use point() instead");
    case SU_ERROR_OUT_OF_RANGE:
      throw std::out_of_range("CW::Text::screen_position(): Percentages must be between 0 and 1");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
}


std::pair<double, double> Text::screen_position() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::screen_position(): Text is null");
  }
  double percent_x, percent_y;
  SUResult res = SUTextGetScreenPosition(this->ref(), &percent_x, &percent_y);
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_NO_DATA:
      throw std::logic_error("CW::Text::screen_position(): Text has a leader; position is determined by point()");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
  return std::make_pair(percent_x, percent_y);
}


Text Text::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::Text::copy(): Text is null");
  }
  // Create a fresh text and copy all properties manually
  SUTextRef new_ref = create_text();

  // Copy string
  SUStringRef str = SU_INVALID;
  SUStringCreate(&str);
  SUResult res = SUTextGetString(this->ref(), &str);
  assert(res == SU_ERROR_NONE); _unused(res);
  size_t length = 0;
  SUStringGetUTF8Length(str, &length);
  std::string text_str(length, '\0');
  SUStringGetUTF8(str, length + 1, &text_str[0], &length);
  SUStringRelease(&str);
  res = SUTextSetString(new_ref, text_str.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy leader type
  SUTextLeaderType leader;
  res = SUTextGetLeaderType(this->ref(), &leader);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUTextSetLeaderType(new_ref, leader);
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy arrow type
  SUArrowType arrow;
  res = SUTextGetArrowType(this->ref(), &arrow);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUTextSetArrowType(new_ref, arrow);
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy point (simple form - no instance path for cross-model copy)
  SUPoint3D pt;
  SUInstancePathRef path = SU_INVALID;
  res = SUTextGetPoint(this->ref(), &pt, &path);
  if (res == SU_ERROR_NONE) {
    res = SUTextSetPoint(new_ref, &pt, SU_INVALID);
    assert(res == SU_ERROR_NONE); _unused(res);
    if (SUIsValid(path)) {
      SUInstancePathRelease(&path);
    }
  }

  // Copy leader vector
  SUVector3D vec;
  res = SUTextGetLeaderVector(this->ref(), &vec);
  if (res == SU_ERROR_NONE) {
    SUTextSetLeaderVector(new_ref, &vec);
  }

  // Copy color
  SUColor col;
  res = SUTextGetColor(this->ref(), &col);
  if (res == SU_ERROR_NONE) {
    SUTextSetColor(new_ref, &col);
  }

  // Copy font
  SUFontRef font = SU_INVALID;
  res = SUTextGetFont(this->ref(), &font);
  if (res == SU_ERROR_NONE && SUIsValid(font)) {
    SUTextSetFont(new_ref, font);
  }

  Text new_text(new_ref, false);
  new_text.copy_attributes_from(*this);
  return new_text;
}


} /* namespace CW */
