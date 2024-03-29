//
//  TypedValue.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

TypedValue::TypedValue():
  m_typed_value(create_typed_value()),
  m_attached(false)
{}


TypedValue::TypedValue(SUTypedValueRef typed_val, bool attached):
  m_typed_value(typed_val),
  m_attached(attached)
{}
 
 
/** Copy constructor */
TypedValue::TypedValue(const TypedValue& other):
  TypedValue()
{
  // Redo copy assignment operator
  (*this) = other;
}


TypedValue& TypedValue::operator= (const TypedValue& other) {
  switch (other.get_type()) {
    case SUTypedValueType_Array:
      {
      std::vector<TypedValue> other_typed = other.typed_value_array();
      typed_value_array(other_typed);
      }
      break;
    case SUTypedValueType_Bool:
      bool_value(other.bool_value());
      break;
    case SUTypedValueType_Byte:
      byte_value(other.byte_value());
      break;
    case SUTypedValueType_Color:
      color_value(other.color_value());
      break;
    case SUTypedValueType_Double:
      double_value(other.double_value());
      break;
    case SUTypedValueType_Empty:
      // Empty TypedValues are all the same.
      break;
    case SUTypedValueType_Float:
      float_value(other.float_value());
      break;
    case SUTypedValueType_Int32:
      int32_value(other.int32_value());
      break;
    case SUTypedValueType_Short:
      // TODO
      throw "this part hasn't been made yet.";
      break;
    case SUTypedValueType_String:
      string_value(other.string_value());
      break;
    case SUTypedValueType_Time:
      time_value(other.time_value());
      break;
    case SUTypedValueType_Vector3D:
      vector_value(other.vector_value());
      break;
  }
  return (*this);
}


SUTypedValueRef TypedValue::ref() const {
  return m_typed_value;
}


TypedValue::operator SUTypedValueRef() {
  return ref();
}


TypedValue::operator SUTypedValueRef*() {
  return &m_typed_value;
}


bool TypedValue::operator!() const {
  if (SUIsValid(m_typed_value)) {
    return false;
  }
  return true;
}


bool TypedValue::empty() const {
  if (SUIsInvalid(m_typed_value) ||
      get_type() == SUTypedValueType::SUTypedValueType_Empty) {
    return true;
  }
  return false;
}


/*
* Static member function
*/
SUTypedValueRef TypedValue::create_typed_value() {
  SUTypedValueRef typed_value = SU_INVALID;
  SUResult res = SUTypedValueCreate(&typed_value);
  assert(res == SU_ERROR_NONE); _unused(res);
  return typed_value;
}


TypedValue::~TypedValue() {
  if (SUIsValid(m_typed_value) && !m_attached) {
    SUResult res = SUTypedValueRelease(&m_typed_value);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


SUTypedValueType TypedValue::get_type() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::get_type(): TypedValue is null");
  }
  SUTypedValueType type;
  SUResult res = SUTypedValueGetType(m_typed_value, &type);
  assert(res == SU_ERROR_NONE); _unused(res);
  return type;
}


char TypedValue::byte_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::byte_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Byte) {
    throw std::logic_error("CW::TypedValue::byte_value(): TypedValue is not SUTypedValueType_Byte type");
  }
  char byte_val;
  SUResult res = SUTypedValueGetByte(m_typed_value, &byte_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return byte_val;
}


TypedValue& TypedValue::byte_value(const char byte_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SUTypedValueSetByte(m_typed_value, byte_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator char() const {
  return byte_value();
}


TypedValue::TypedValue(const char byte_val):
  TypedValue()
{
  this->byte_value(byte_val);
}


int16_t TypedValue::int16_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::int16_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Short) {
    throw std::logic_error("CW::TypedValue::int16_value(): TypedValue is not SUTypedValueType_Short (Int16) type");
  }
  int16_t int16_val;
  SUResult res = SUTypedValueGetInt16(m_typed_value, &int16_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return int16_val;
}


TypedValue& TypedValue::int16_value(const int16_t int16_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SUTypedValueSetInt16(m_typed_value, int16_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator int16_t() const {
  return int16_value();
}



TypedValue::TypedValue(const int16_t int16_val):
  TypedValue()
{
  this->int16_value(int16_val);
}


int32_t TypedValue::int32_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::int32_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Int32) {
    throw std::logic_error("CW::TypedValue::int32_value(): TypedValue is not SUTypedValueType_Int32 type");
  }
  int32_t int32_val;
  SUResult res = SUTypedValueGetInt32(m_typed_value, &int32_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return int32_val;
}


TypedValue& TypedValue::int32_value(const int32_t int32_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SUTypedValueSetInt32(m_typed_value, int32_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator int32_t() const {
  return int32_value();
}


TypedValue::TypedValue(const int32_t int32_val):
  TypedValue()
{
  this->int32_value(int32_val);
}


float TypedValue::float_value() const{
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::float_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Float) {
    throw std::logic_error("CW::TypedValue::float_value(): TypedValue is not SUTypedValueType_Float type");
  }
  float float_val;
  SUResult res = SUTypedValueGetFloat(m_typed_value, &float_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return float_val;
}


TypedValue& TypedValue::float_value(const float float_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SUTypedValueSetFloat(m_typed_value, float_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator float() const {
  return float_value();
}


TypedValue::TypedValue(const float float_val):
  TypedValue()
{
  this->float_value(float_val);
}


double TypedValue::double_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::double_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Double) {
    throw std::logic_error("CW::TypedValue::double_value(): TypedValue is not SUTypedValueType_Double type");
  }
  double double_val;
  SUResult res = SUTypedValueGetDouble(m_typed_value, &double_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return double_val;
}


TypedValue& TypedValue::double_value(const double double_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SUTypedValueSetDouble(m_typed_value, double_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator double() const {
  return double_value();
}


TypedValue::TypedValue(const double double_val):
  TypedValue()
{
  this->double_value(double_val);
}


bool TypedValue::bool_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::bool_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Bool) {
    throw std::logic_error("CW::TypedValue::bool_value(): TypedValue is not SUTypedValueType_Bool type");
  }
  bool bool_val;
  SUResult res = SUTypedValueGetBool(m_typed_value, &bool_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return bool_val;
}


TypedValue& TypedValue::bool_value(const bool bool_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SUTypedValueSetBool(m_typed_value, bool_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator bool() const {
  return bool_value();
}


TypedValue::TypedValue(const bool bool_val):
  TypedValue()
{
  this->bool_value(bool_val);
}


Color TypedValue::color_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::color_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Color) {
    throw std::logic_error("CW::TypedValue::color_value(): TypedValue is not SUTypedValueType_Color type");
  }
  SUColor color_val;
  SUResult res = SUTypedValueGetColor(m_typed_value, &color_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Color(color_val);
}


TypedValue& TypedValue::color_value(const Color &color_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUColor color = color_val.ref();
  SUResult res = SUTypedValueSetColor(m_typed_value, &color);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator Color() const {
  return color_value();
}


TypedValue::TypedValue(const Color &color_val):
  TypedValue()
{
  this->color_value(color_val);
}


int64_t TypedValue::time_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::time_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Time) {
    throw std::logic_error("CW::TypedValue::time_value(): TypedValue is not SUTypedValueType_Time type");
  }
  int64_t time_val;
  SUResult res = SUTypedValueGetTime(m_typed_value, &time_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return time_val;
}


TypedValue& TypedValue::time_value(const int64_t time_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SUTypedValueSetTime(m_typed_value, time_val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator int64_t() const {
  return time_value();
}


TypedValue::TypedValue(const int64_t time_val):
  TypedValue()
{
  this->time_value(time_val);
}


String TypedValue::string_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::string_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_String) {
    throw std::logic_error("CW::TypedValue::string_value(): TypedValue is not SUTypedValueType_String type");
  }
  String string;
  SUStringRef &string_ref = string;
  SUResult res = SUTypedValueGetString(m_typed_value, &string_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


TypedValue& TypedValue::string_value(const String &string_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  std::string std_string = string_val;
  SUResult res = SUTypedValueSetString(m_typed_value, std_string.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue& TypedValue::string_value(const std::string &string_val) {
  return string_value(String(string_val));
}


TypedValue::operator String() const {
  return string_value();
}


TypedValue::operator std::string() const {
  return string_value();
}


TypedValue::TypedValue(const char chars[]):
  TypedValue(String(chars))
{}


TypedValue::TypedValue(const std::string& string):
  TypedValue(string.data())
{}


TypedValue::TypedValue(const String& string):
  TypedValue()
{
  this->string_value(string);
}



Vector3D TypedValue::vector_value() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::vector_value(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Vector3D) {
    throw std::logic_error("CW::TypedValue::vector_value(): TypedValue is not SUTypedValueType_Vector3D type");
  }
  double vector3d_value[3];
  SUResult res = SUTypedValueGetVector3d(m_typed_value, &vector3d_value[0]);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(vector3d_value[0], vector3d_value[1], vector3d_value[2]);
}


TypedValue& TypedValue::vector_value(const Vector3D &vector_val) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  double vector3d_value[3] = {vector_val.x, vector_val.y, vector_val.z};
  SUResult res = SUTypedValueSetVector3d(m_typed_value, &vector3d_value[0]);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator Vector3D() const {
  return vector_value();
}


TypedValue::TypedValue(const Vector3D& vector):
  TypedValue()
{
  this->vector_value(vector);
}


std::vector<TypedValue> TypedValue::typed_value_array() const {
  if (!(*this)) {
    throw std::logic_error("CW::TypedValue::typed_value_array(): TypedValue is null");
  }
  if (get_type() != SUTypedValueType_Array) {
    throw std::logic_error("CW::TypedValue::typed_value_array(): TypedValue is not SUTypedValueType_Array type");
  }
  size_t count = 0;
  SUResult res = SUTypedValueGetNumArrayItems(m_typed_value, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUTypedValueRef> value_refs(count, SU_INVALID);
  res = SUTypedValueGetArrayItems(m_typed_value, count, value_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  // TypedValue from TypedValue arrays should not actually be released.  This poses a problem for the wrapper object which will want to release it.  The solution is to copy these special TypedValues that don't release into new TypedValue objects that do.
  std::vector<TypedValue> temp_vals(count);
  std::transform(value_refs.begin(), value_refs.end(), temp_vals.begin(),
    [](const SUTypedValueRef& value) {
      return TypedValue(value, true);
    });
  std::vector<TypedValue> typed_vals(count);
  // Force a copy operation to create an object that will be released normally.
  for (size_t i=0; i < typed_vals.size(); i++) {
    typed_vals[i] = temp_vals[i];
  }
  return typed_vals;
}


TypedValue& TypedValue::typed_value_array(std::vector<TypedValue> &typed_val_array) {
  if (!(*this)) {
    m_typed_value = create_typed_value();
  }
  SUResult res = SU_ERROR_NONE;
  std::vector<SUTypedValueRef> refs{typed_val_array.size(), SU_INVALID };
  std::transform(typed_val_array.begin(), typed_val_array.end(), refs.begin(),
    [](const CW::TypedValue& typed_value) {return typed_value.ref(); });
  res = SUTypedValueSetArrayItems(m_typed_value, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


TypedValue::operator std::vector<TypedValue>() const {
  return typed_value_array();
}


TypedValue::TypedValue(std::vector<TypedValue> &typed_val_array):
  TypedValue()
{
  this->typed_value_array(typed_val_array);
}


bool operator== (const TypedValue &val1, const TypedValue &val2) {
  // Check validity
  if (SUIsInvalid(val1.m_typed_value) || SUIsInvalid(val1.m_typed_value)) {
    if (SUIsInvalid(val1.m_typed_value) && SUIsInvalid(val1.m_typed_value)) {
      return true;
    }
    return false;
  }
  // Check types
  if (val1.get_type() != val1.get_type()) {
    return false;
  }
  switch (val1.get_type()) {
    case SUTypedValueType_Array:
      if (val1.typed_value_array() == val2.typed_value_array()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Bool:
      if (val1.bool_value() == val2.bool_value()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Byte:
      if (val1.byte_value() == val2.byte_value()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Color:
      if (val1.color_value() == val2.color_value()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Double:
      if (val1.double_value() == val2.double_value()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Empty:
      // Empty TypedValues are all the same.
      return true;
      break;
    case SUTypedValueType_Float:
      if (val1.float_value() == val2.float_value()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Int32:
      if (val1.int32_value() == val2.int32_value()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Short:
      // TODO
      throw "this part hasn't been made yet.";
      break;
    case SUTypedValueType_String:
      if (val1.string_value() == val2.string_value()) {
        return true;
      }
      return false;

      break;
    case SUTypedValueType_Time:
      if (val1.time_value() == val2.time_value()) {
        return true;
      }
      return false;
      break;
    case SUTypedValueType_Vector3D:
      if (val1.vector_value() == val2.vector_value()) {
        return true;
      }
      return false;
      break;
  }
  return false;
}

bool operator!= (const TypedValue &val1, const TypedValue &val2) {
  if (val1 == val2) {
    return false;
  }
  return true;
}


} /* namespace CW */
