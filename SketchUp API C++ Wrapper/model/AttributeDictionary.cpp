//
//  AttributeDictionary.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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

AttributeDictionary::AttributeDictionary(SUEntityRef entity_ref):
	m_dict(SUAttributeDictionaryFromEntity(entity_ref))
{
}

TypedValue AttributeDictionary::get_attribute(const std::string &key, const TypedValue &default_value) const {
	TypedValue value_out;
  SUTypedValueRef *val = value_out;
  const char* key_char = key.c_str();
  SU_RESULT res = SUAttributeDictionaryGetValue(m_dict, &key_char[0], val);
  if (res == SU_ERROR_NO_DATA) {
  	return default_value;
  }
  assert(res == SU_ERROR_NONE);
  return value_out;
}

bool AttributeDictionary::set_attribute(const std::string &key, const TypedValue &value) {
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

AttributeDictionary::operator bool() {
	if (SUIsValid(m_dict)) {
  	return true;
  }
  return false;
}


} /* namespace CW */
