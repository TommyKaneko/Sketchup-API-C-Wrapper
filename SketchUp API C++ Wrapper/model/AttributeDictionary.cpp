//
//  AttributeDictionary.cpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <cassert>
#include <algorithm>

#include "AttributeDictionary.hpp"
#include "TypedValue.hpp"

namespace CW {


AttributeDictionary::AttributeDictionary():
	m_dict(SU_INVALID)
{
}

AttributeDictionary::AttributeDictionary(SUAttributeDictionaryRef dict_ref):
	m_dict(dict_ref)
{
}


AttributeDictionary::operator SUAttributeDictionaryRef() const {
	return m_dict;
}

AttributeDictionary::operator SUAttributeDictionaryRef*() {
	return &m_dict;
}


TypedValue AttributeDictionary::get_attribute(const std::string &key, const TypedValue &default_value) const {
  if (!(*this)) {
  	throw std::logic_error("CW::AttributeDictionary::get_attribute(): AttributeDictionary is null");
  }
	TypedValue value_out;
  SUTypedValueRef *val = value_out;
  const char* key_char = key.c_str();
  SU_RESULT res = SUAttributeDictionaryGetValue(m_dict, &key_char[0], val);
  if (res == SU_ERROR_NO_DATA) {
  	return default_value;
  }
  if (res != SU_ERROR_NONE) {
  	assert(false);
    //throw std::logic_error("CW::AttributeDictionary::get_attribute(): index range is between 0 and 15");
  }
  return value_out;
}

bool AttributeDictionary::set_attribute(const std::string &key, const TypedValue &value) {
  if (!(*this)) {
  	throw std::logic_error("CW::AttributeDictionary::set_attribute(): AttributeDictionary is null");
  }
  SUTypedValueRef val = value.ref();
  const char* key_char = key.c_str();
  SU_RESULT res = SUAttributeDictionarySetValue(m_dict, &key_char[0], val);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  else {
  	return false;
  }
}

std::vector<std::string> AttributeDictionary::get_keys() const {
  if (!(*this)) {
  	throw std::logic_error("CW::AttributeDictionary::get_keys(): AttributeDictionary is null");
  }
	size_t num_keys = 0;
  SU_RESULT res = SUAttributeDictionaryGetNumKeys(m_dict, &num_keys);
  assert(res == SU_ERROR_NONE);
  SUStringRef keys_ref[num_keys];
  for (size_t i=0; i < num_keys; i++) {
  	keys_ref[i] = SU_INVALID;
  }
  SUAttributeDictionaryGetKeys(m_dict, num_keys, &keys_ref[0], &num_keys);
  std::vector<std::string> keys;
  keys.reserve(num_keys);
  for (size_t i=0; i < num_keys; i++) {
  	keys.push_back(String(keys_ref[i]));
  }
  return keys;
}

TypedValue AttributeDictionary::get_value(const std::string &key) const {
  return get_attribute(key, TypedValue());
}

std::string AttributeDictionary::get_name() const {
	String string;
  SUStringRef *string_ref = string;
	SU_RESULT res = SUAttributeDictionaryGetName(m_dict, string_ref);
  assert(res == SU_ERROR_NONE);
  return string;
}

bool AttributeDictionary::operator !() const {
	if (SUIsValid(m_dict)) {
  	return false;
  }
  return true;
}


} /* namespace CW */
