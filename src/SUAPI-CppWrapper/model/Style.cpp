//
//  Style.cpp
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

#include "SUAPI-CppWrapper/model/Style.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUStyleRef Style::create_style() {
  SUStyleRef style = SU_INVALID;
  SUResult res = SUStyleCreate(&style);
  assert(res == SU_ERROR_NONE); _unused(res);
  return style;
}


SUStyleRef Style::copy_reference(const Style& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  // For detached styles, create a new empty style and copy properties
  SUStyleRef new_style = create_style();
  // Copy name
  SUStringRef name_ref = SU_INVALID;
  SUStringCreate(&name_ref);
  SUStyleGetName(other.ref(), &name_ref);
  size_t length = 0;
  SUStringGetUTF8Length(name_ref, &length);
  std::string name(length, '\0');
  SUStringGetUTF8(name_ref, length + 1, &name[0], &length);
  SUStringRelease(&name_ref);
  SUStyleSetName(new_style, name.c_str());
  // Copy description
  SUStringRef desc_ref = SU_INVALID;
  SUStringCreate(&desc_ref);
  SUStyleGetDescription(other.ref(), &desc_ref);
  length = 0;
  SUStringGetUTF8Length(desc_ref, &length);
  std::string desc(length, '\0');
  SUStringGetUTF8(desc_ref, length + 1, &desc[0], &length);
  SUStringRelease(&desc_ref);
  SUStyleSetDescription(new_style, desc.c_str());
  return new_style;
}


/******************************
** Constructors / Destructor **
*******************************/
Style::Style():
  Entity(SU_INVALID, true)
{}


Style::Style(const std::string& name):
  Entity(SUStyleToEntity(create_style()), false)
{
  SUStyleSetName(this->ref(), name.c_str());
}


Style::Style(SUStyleRef style, bool attached):
  Entity(SUStyleToEntity(style), attached)
{}


Style::Style(const Style& other):
  Entity(other, SUStyleToEntity(copy_reference(other)))
{}


Style::~Style() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUStyleRef style = this->ref();
    SUResult res = SUStyleRelease(&style);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


Style& Style::operator=(const Style& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUStyleRef style = this->ref();
    SUResult res = SUStyleRelease(&style);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUStyleToEntity(copy_reference(other));
  Entity::operator=(other);
  return *this;
}


/*******************
** Public Methods **
********************/
SUStyleRef Style::ref() const {
  return SUStyleFromEntity(m_entity);
}


bool Style::operator!() const {
  return SUIsInvalid(SUStyleToEntity(this->ref()));
}


// TODO: Styles cannot be created independently of a model, so copying is not currently supported. To copy a style, save it to file and load it into the other model.
// Style Style::copy() const {
//   if (!(*this)) {
//     throw std::logic_error("CW::Style::copy(): Style is null");
//   }
//   return Style(copy_reference(*this), false);
//}



String Style::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::name(): Style is null");
  }
  String name;
  SUStringRef name_ref = name.ref();
  SUResult res = SUStyleGetName(this->ref(), &name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return name;
}


void Style::name(const std::string& name) {
  if (!(*this)) {
    throw std::logic_error("CW::Style::name(): Style is null");
  }
  SUResult res = SUStyleSetName(this->ref(), name.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


String Style::display_name() const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::display_name(): Style is null");
  }
  String name;
  SUStringRef name_ref = name.ref();
  SUResult res = SUStyleGetDisplayName(this->ref(), &name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return name;
}


String Style::description() const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::description(): Style is null");
  }
  String desc;
  SUStringRef desc_ref = desc.ref();
  SUResult res = SUStyleGetDescription(this->ref(), &desc_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return desc;
}


void Style::description(const std::string& description) {
  if (!(*this)) {
    throw std::logic_error("CW::Style::description(): Style is null");
  }
  SUResult res = SUStyleSetDescription(this->ref(), description.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


String Style::path() const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::path(): Style is null");
  }
  String path;
  SUStringRef path_ref = path.ref();
  SUResult res = SUStyleGetPath(this->ref(), &path_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return path;
}


String Style::guid() const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::guid(): Style is null");
  }
  String guid;
  SUStringRef guid_ref = guid.ref();
  SUResult res = SUStyleGetGuid(this->ref(), &guid_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return guid;
}


bool Style::displays_watermark() const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::displays_watermark(): Style is null");
  }
  bool shows_mark = false;
  SUResult res = SUStyleGetDisplaysWatermark(this->ref(), &shows_mark);
  assert(res == SU_ERROR_NONE); _unused(res);
  return shows_mark;
}


void Style::save_to_file(const std::string& file_path) const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::save_to_file(): Style is null");
  }
  SUResult res = SUStyleSaveToFile(this->ref(), file_path.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


TypedValue Style::get_property(enum SUStylePropertyType type) const {
  if (!(*this)) {
    throw std::logic_error("CW::Style::get_property(): Style is null");
  }
  SUTypedValueRef value = SU_INVALID;
  SUTypedValueCreate(&value);
  SUResult res = SUStyleGetProperty(this->ref(), type, &value);
  assert(res == SU_ERROR_NONE); _unused(res);
  return TypedValue(value, false);
}


void Style::set_property(enum SUStylePropertyType type, const TypedValue& value) {
  if (!(*this)) {
    throw std::logic_error("CW::Style::set_property(): Style is null");
  }
  SUResult res = SUStyleSetProperty(this->ref(), type, value.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}

} /* namespace CW */
