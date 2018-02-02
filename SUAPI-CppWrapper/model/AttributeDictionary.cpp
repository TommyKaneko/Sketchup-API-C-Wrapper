//
//  AttributeDictionary.cpp
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

#include <cassert>
#include <algorithm>

#include "SUAPI-CppWrapper/model/AttributeDictionary.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"

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
  SUResult res = SUAttributeDictionaryGetValue(m_dict, &key_char[0], val);
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
  SUResult res = SUAttributeDictionarySetValue(m_dict, &key_char[0], val);
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
  SUResult res = SUAttributeDictionaryGetNumKeys(m_dict, &num_keys);
  assert(res == SU_ERROR_NONE);
  SUStringRef* keys_ref = new SUStringRef[num_keys];
  for (size_t i=0; i < num_keys; i++) {
    keys_ref[i] = SU_INVALID;
    SUStringCreate(&keys_ref[i]);
  }
  SUAttributeDictionaryGetKeys(m_dict, num_keys, &keys_ref[0], &num_keys);
  std::vector<std::string> keys;
  keys.reserve(num_keys);
  for (size_t i=0; i < num_keys; i++) {
    keys.push_back(String(keys_ref[i]));
  }
  delete[] keys_ref;
  return keys;
}

TypedValue AttributeDictionary::get_value(const std::string &key) const {
  return get_attribute(key, TypedValue());
}

std::string AttributeDictionary::get_name() const {
  String string;
  SUStringRef *string_ref = string;
  SUResult res = SUAttributeDictionaryGetName(m_dict, string_ref);
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
