//
//  Font.cpp
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

#include "SUAPI-CppWrapper/model/Font.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
Font::Font():
  Entity()
{}


Font::Font(SUFontRef font, bool attached):
  Entity(SUFontToEntity(font), attached)
{}


/*******************
** Public Methods **
********************/
SUFontRef Font::ref() const {
  return SUFontFromEntity(m_entity);
}

Font::operator SUFontRef() const {
  return this->ref();
}

Font::operator SUFontRef*() {
  return reinterpret_cast<SUFontRef*>(&m_entity);
}


bool Font::operator!() const {
  return SUIsInvalid(m_entity);
}


String Font::face_name() const {
  if (!(*this)) {
    throw std::logic_error("CW::Font::face_name(): Font is null");
  }
  String string;
  SUResult res = SUFontGetFaceName(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


size_t Font::point_size() const {
  if (!(*this)) {
    throw std::logic_error("CW::Font::point_size(): Font is null");
  }
  size_t size;
  SUResult res = SUFontGetPointSize(this->ref(), &size);
  assert(res == SU_ERROR_NONE); _unused(res);
  return size;
}


bool Font::bold() const {
  if (!(*this)) {
    throw std::logic_error("CW::Font::bold(): Font is null");
  }
  bool is_bold;
  SUResult res = SUFontGetBold(this->ref(), &is_bold);
  assert(res == SU_ERROR_NONE); _unused(res);
  return is_bold;
}


bool Font::italic() const {
  if (!(*this)) {
    throw std::logic_error("CW::Font::italic(): Font is null");
  }
  bool is_italic;
  SUResult res = SUFontGetItalic(this->ref(), &is_italic);
  assert(res == SU_ERROR_NONE); _unused(res);
  return is_italic;
}


bool Font::use_world_size() const {
  if (!(*this)) {
    throw std::logic_error("CW::Font::use_world_size(): Font is null");
  }
  bool uses_world;
  SUResult res = SUFontGetUseWorldSize(this->ref(), &uses_world);
  assert(res == SU_ERROR_NONE); _unused(res);
  return uses_world;
}


double Font::world_size() const {
  if (!(*this)) {
    throw std::logic_error("CW::Font::world_size(): Font is null");
  }
  double size;
  SUResult res = SUFontGetWorldSize(this->ref(), &size);
  assert(res == SU_ERROR_NONE); _unused(res);
  return size;
}


} /* namespace CW */
