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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include <cassert>
#include <stdexcept>
#include <algorithm>

#include "SUAPI-CppWrapper/model/AttributeDictionary.hpp"

#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

extern size_t SU_VERSION_MAJOR;

/******************
* Private Static methods
*******************/
SUAttributeDictionaryRef AttributeDictionary::create_attribute_dictionary(const std::string& name) {
  if (SU_VERSION_MAJOR < 18) {
    throw std::logic_error("AttributeDictionary::create_attribute_dictionary(): Cannot use function prior to SU version 2018");
  }
  SUAttributeDictionaryRef dict = SU_INVALID;
  SUResult res = SUAttributeDictionaryCreate(&dict, name.c_str());
  assert(res == SU_ERROR_NONE);
  _unused(res);
  return dict;
}


SUAttributeDictionaryRef AttributeDictionary::copy_reference(const AttributeDictionary& other) {
  if (SU_VERSION_MAJOR < 18) {
    throw std::logic_error("AttributeDictionary::create_attribute_dictionary(): Cannot use function prior to SU version 2018");
  }
  if (other.m_attached || !other) {
    return other.ref();
  }
  // The other Attributedictionary has not been attached to the model, so copy its properties to a new object
  SUAttributeDictionaryRef new_dict = create_attribute_dictionary(other.get_name());
  return new_dict;
}


/*****************************
* Constructor / Destructors **
******************************/
AttributeDictionary::AttributeDictionary():
  Entity(SU_INVALID)
{}
  

AttributeDictionary::AttributeDictionary(std::string name):
  AttributeDictionary(create_attribute_dictionary(name), false)
{}


AttributeDictionary::AttributeDictionary(SUAttributeDictionaryRef dict_ref, bool attached):
  Entity(SUAttributeDictionaryToEntity(dict_ref), attached)
{}


/** Copy constructor */
AttributeDictionary::AttributeDictionary(const AttributeDictionary& other):
  Entity(other, SUAttributeDictionaryToEntity(copy_reference(other)))
{
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    // Create a copy of the keys and values
    std::vector<std::string> keys = other.get_keys();
    for (size_t i=0; i < keys.size(); i++) {
      this->set_attribute(keys[i], other.get_value(keys[i]));
    }
  }
}


AttributeDictionary::~AttributeDictionary() {
  if (SU_VERSION_MAJOR >= 18) {
    if (!m_attached && SUIsValid(m_entity)) {
      SUAttributeDictionaryRef dict = this->ref();
      SUResult res = SUAttributeDictionaryRelease(&dict);
      assert(res == SU_ERROR_NONE);
      _unused(res);
    }
  }
}

/******************
* Public Methods **
*******************/
/** Copy assignment operator */
AttributeDictionary& AttributeDictionary::operator=(const AttributeDictionary& other) {
  if (SU_VERSION_MAJOR >= 18 && !m_attached && SUIsValid(m_entity)) {
    SUAttributeDictionaryRef dict = this->ref();
    SUResult res = SUAttributeDictionaryRelease(&dict);
    assert(res == SU_ERROR_NONE);
    _unused(res);
  }
  SUAttributeDictionaryRef dict = copy_reference(other);
  m_entity = SUAttributeDictionaryToEntity(dict);
  Entity::operator=(other);
  return *this;
}


SUAttributeDictionaryRef AttributeDictionary::ref() const {
  return SUAttributeDictionaryFromEntity(m_entity);
}


AttributeDictionary::operator SUAttributeDictionaryRef() const {
  return this->ref();
}


TypedValue AttributeDictionary::get_attribute(const std::string &key, const TypedValue &default_value) const {
  if (!(*this)) {
    throw std::logic_error("CW::AttributeDictionary::get_attribute(): AttributeDictionary is null");
  }
  TypedValue value_out;
  SUTypedValueRef *val = value_out;
  const char* key_char = key.c_str();
  SUResult res = SUAttributeDictionaryGetValue(this->ref(), &key_char[0], val);
  if (res == SU_ERROR_NO_DATA) {
    return default_value;
  }
  assert(res == SU_ERROR_NONE);
  _unused(res);
  return value_out;
}

bool AttributeDictionary::set_attribute(const std::string &key, const TypedValue &value) {
  if (!(*this)) {
    throw std::logic_error("CW::AttributeDictionary::set_attribute(): AttributeDictionary is null");
  }
  SUTypedValueRef val = value.ref();
  const char* key_char = key.c_str();
  SUResult res = SUAttributeDictionarySetValue(this->ref(), &key_char[0], val);
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
  SUResult res = SUAttributeDictionaryGetNumKeys(this->ref(), &num_keys);
  assert(res == SU_ERROR_NONE);
  std::vector<SUStringRef> keys_ref(num_keys, SU_INVALID);
  std::for_each(keys_ref.begin(), keys_ref.end(),
  [](SUStringRef& value) {
    SUResult res = SUStringCreate(&value);
    assert(res == SU_ERROR_NONE); _unused(res);
  });
  res = SUAttributeDictionaryGetKeys(this->ref(), num_keys, keys_ref.data(), &num_keys);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<std::string> keys(num_keys);
  std::transform(keys_ref.begin(), keys_ref.end(), keys.begin(),
  [](const SUStringRef& value) {
    return String(value).std_string();
  });
  return keys;
}

TypedValue AttributeDictionary::get_value(const std::string &key) const {
  return get_attribute(key, TypedValue());
}

std::string AttributeDictionary::get_name() const {
  String string;
  SUStringRef *string_ref = string;
  SUResult res = SUAttributeDictionaryGetName(this->ref(), string_ref);
  assert(res == SU_ERROR_NONE);
  _unused(res);
  return string;
}

bool AttributeDictionary::operator !() const {
  if (SUIsValid(m_entity)) {
    return false;
  }
  return true;
}


} /* namespace CW */
