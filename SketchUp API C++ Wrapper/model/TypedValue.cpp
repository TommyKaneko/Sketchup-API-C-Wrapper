//
//  TypedValue.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 13/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//
#include <cassert>

#include "TypedValue.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "String.hpp"

namespace CW {

TypedValue::TypedValue():
	m_typed_value(create_typed_value())
{}

TypedValue::TypedValue(SUTypedValueRef typed_val):
	m_typed_value(typed_val)
{}
 
TypedValue::TypedValue(const char chars[]):
	m_typed_value(create_typed_value())
{
	this->string_value(String(chars));
}
//TypedValue(const std::string string);


TypedValue::operator SUTypedValueRef() {
	return m_typed_value;
}

TypedValue::operator SUTypedValueRef*() {
	return &m_typed_value;
}

/*
* Static member function
*/
SUTypedValueRef TypedValue::create_typed_value() {
  SUTypedValueRef typed_value = SU_INVALID;
  SU_RESULT res = SUTypedValueCreate(&typed_value);
	assert(res == SU_ERROR_NONE);
  return typed_value;
}

TypedValue::~TypedValue() {
	SUTypedValueRelease(&m_typed_value);
}


SUTypedValueType TypedValue::get_type() const {
  SUTypedValueType type;
  SU_RESULT res = SUTypedValueGetType(m_typed_value, &type);
  assert(res == SU_ERROR_NONE);
  return type;
}


char TypedValue::byte_value() const {
	char byte_val;
  SU_RESULT res = SUTypedValueGetByte(m_typed_value, &byte_val);
  assert(res == SU_ERROR_NONE);
  return byte_val;
}


TypedValue& TypedValue::byte_value(char byte_val) {
	SU_RESULT res = SUTypedValueSetByte(m_typed_value, byte_val);
  assert(res == SU_ERROR_NONE);
  return *this;
}


TypedValue::operator char() const {
	return byte_value();
}


int16_t TypedValue::int16_value() const {
	int16_t int16_val;
	SU_RESULT res = SUTypedValueGetInt16(m_typed_value, &int16_val);
  assert(res == SU_ERROR_NONE);
  return int16_val;
}

TypedValue& TypedValue::int16_value(int16_t int16_val) {
	SU_RESULT res = SUTypedValueSetInt16(m_typed_value, int16_val);
  assert(res == SU_ERROR_NONE);
  return *this;
}
TypedValue::operator int16_t() const {
	return int16_value();
}


int32_t TypedValue::int32_value() const {
	int32_t int32_val;
	SU_RESULT res = SUTypedValueGetInt32(m_typed_value, &int32_val);
  assert(res == SU_ERROR_NONE);
  return int32_val;
}

TypedValue& TypedValue::int32_value(int32_t int32_val) {
	SU_RESULT res = SUTypedValueSetInt32(m_typed_value, int32_val);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator int32_t() const {
	return int32_value();
}


float TypedValue::float_value() const{
	float float_val;
	SU_RESULT res = SUTypedValueGetFloat(m_typed_value, &float_val);
  assert(res == SU_ERROR_NONE);
  return float_val;
}

TypedValue& TypedValue::float_value(float float_val) {
	SU_RESULT res = SUTypedValueSetFloat(m_typed_value, float_val);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator float() const {
	return float_value();
}

double TypedValue::double_value() const {
	double double_val;
	SU_RESULT res = SUTypedValueGetDouble(m_typed_value, &double_val);
  assert(res == SU_ERROR_NONE);
  return double_val;
}

TypedValue& TypedValue::double_value(double double_val) {
	SU_RESULT res = SUTypedValueSetDouble(m_typed_value, double_val);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator double() const {
	return double_value();
}

bool TypedValue::bool_value() const {
	bool bool_val;
	SU_RESULT res = SUTypedValueGetBool(m_typed_value, &bool_val);
  assert(res == SU_ERROR_NONE);
  return bool_val;
}

TypedValue& TypedValue::bool_value(bool bool_val) {
	SU_RESULT res = SUTypedValueSetBool(m_typed_value, bool_val);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator bool() const {
	return bool_value();
}


Color TypedValue::color_value() const {
	SUColor color_val;
	SU_RESULT res = SUTypedValueGetColor(m_typed_value, &color_val);
  assert(res == SU_ERROR_NONE);
  return Color(color_val);
}

TypedValue& TypedValue::color_value(Color color_val) {
	SUColor color = color_val.ref();
  SU_RESULT res = SUTypedValueSetColor(m_typed_value, &color);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator Color() const {
	return color_value();
}

int64_t TypedValue::time_value() const {
	int64_t time_val;
	SU_RESULT res = SUTypedValueGetTime(m_typed_value, &time_val);
  assert(res == SU_ERROR_NONE);
  return time_val;
}

TypedValue& TypedValue::time_value(int64_t time_val) {
	SU_RESULT res = SUTypedValueSetTime(m_typed_value, time_val);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator int64_t() const {
	return time_value();
}

String TypedValue::string_value() const {
	String string;
	SU_RESULT res = SUTypedValueGetString(m_typed_value, string);
  assert(res == SU_ERROR_NONE);
  return string;
}

TypedValue& TypedValue::string_value(String string_val) {
  std::string std_string = string_val;
  SU_RESULT res = SUTypedValueSetString(m_typed_value, std_string.c_str());
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue& TypedValue::string_value(std::string string_val) {
  return string_value(String(string_val));
}

TypedValue::operator String() const {
	return string_value();
}
TypedValue::operator std::string() const {
  return string_value();
}


Vector3D TypedValue::vector_value() const {
	double vector3d_value[3];
	SU_RESULT res = SUTypedValueGetVector3d(m_typed_value, &vector3d_value[0]);
  assert(res == SU_ERROR_NONE);
  return Vector3D(vector3d_value[0], vector3d_value[1], vector3d_value[2]);
}

TypedValue& TypedValue::vector_value(Vector3D vector_val) {
	double vector3d_value[3] = {vector_val.x, vector_val.y, vector_val.z};
	SU_RESULT res = SUTypedValueSetVector3d(m_typed_value, &vector3d_value[0]);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator Vector3D() const {
	return vector_value();
}

std::vector<TypedValue> TypedValue::typed_value_array() const {
  size_t count;
  SU_RESULT res = SUTypedValueGetNumArrayItems(m_typed_value, &count);
  assert(res == SU_ERROR_NONE);
  SUTypedValueRef values[count];
  res = SUTypedValueGetArrayItems(m_typed_value, count, &values[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<TypedValue> typed_vals;
  typed_vals.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	typed_vals.push_back(TypedValue(values[i]));
  }
  return typed_vals;
}

TypedValue& TypedValue::typed_value_array(std::vector<TypedValue> typed_val_array) {
  size_t count = typed_val_array.size();
  SUTypedValueRef values[count];
  for (size_t i=0; i < count; ++i) {
  	values[i] = typed_val_array[i];
  }
  SU_RESULT res = SUTypedValueSetArrayItems(m_typed_value, count, &values[0]);
  assert(res == SU_ERROR_NONE);
  return *this;
}

TypedValue::operator std::vector<TypedValue>() const {
	return typed_value_array();
}

} /* namespace CW */