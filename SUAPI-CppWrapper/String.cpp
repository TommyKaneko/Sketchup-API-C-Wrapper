//
//  String.cpp
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

#include "SUAPI-CppWrapper/String.hpp"

#include <cassert>

namespace CW {

SUStringRef m_string;

String::String():
	m_string(create_string_ref()),
  m_encoding(StringEncoding::UTF8)
{
}

String::String(SUStringRef string_ref):
	m_string(string_ref),
  m_encoding(StringEncoding::UTF8)
{
}

String::String(const std::string &string_input, StringEncoding enc):
	m_string(create_string_ref(string_input, enc)),
  m_encoding(enc)
{
}

String::String(const char string_input[]):
	m_string(create_string_ref(&string_input[0])),
  m_encoding(StringEncoding::UTF8)
{
}

String::String(const unichar string_input[]):
	m_string(create_string_ref(&string_input[0])),
  m_encoding(StringEncoding::UTF16)
{
}


String::String(const String& other):
	m_string(create_string_ref(other.std_string(), StringEncoding::UTF8))
{
	// TODO support UTF16
}


String& String::operator=(String& other) {
	// Release old string and create new
	if (SUIsValid(m_string)) {
  	SU_RESULT res = SUStringRelease(&m_string);
    assert(res == SU_ERROR_NONE);
  }
  m_string = SU_INVALID;
  m_string = create_string_ref(other.std_string(), StringEncoding::UTF8);
  return *this;
}


bool operator==(const String &lhs, const String &rhs) {
	// Check validity
  // Check type
  if (lhs.m_encoding != rhs.m_encoding) {
  	return false;
  }
  // Check value
  if (lhs.std_string() == rhs.std_string()) {
  	return true;
  }
  return false;
}


SUStringRef String::create_string_ref() {
  SUStringRef string_ref = SU_INVALID;
	SU_RESULT res = SUStringCreate(&string_ref);
  assert(res == SU_ERROR_NONE);
  return string_ref;
}

SUStringRef String::create_string_ref(std::string string_input, StringEncoding enc) {
  SUStringRef string_ref = SU_INVALID;
	if (enc == StringEncoding::UTF8) {
    
    // TODO: to reduce code, use String::create_string_ref(const char* string_input[]).
    SUStringCreateFromUTF8(&string_ref, &string_input[0]);
  }
  else {
    // TODO UTF16 to be supported.
    //SUStringCreateFromUTF16(&string_ref, &string_input[0]);
  }
  return string_ref;
}

SUStringRef String::create_string_ref(const char string_input[]) {
  SUStringRef string_ref = SU_INVALID;
  SUStringCreateFromUTF8(&string_ref, &string_input[0]);
  return string_ref;
}

SUStringRef String::create_string_ref(const unichar string_input[]) {
  SUStringRef string_ref = SU_INVALID;
  // TODO UTF16 to be supported.
  //SUStringCreateFromUTF16(&string_ref, &string_input[0]);
  return string_ref;
}
  

String::~String() {
	if (SUIsValid(m_string)) {
  	SU_RESULT res = SUStringRelease(&m_string);
    assert(res == SU_ERROR_NONE);
  }
}

SUStringRef String::ref() const {
	return m_string;
}

std::string String::std_string() const {
	size_t out_length = 0;
  SU_RESULT res = SUStringGetUTF8Length(m_string, &out_length);
  assert(res == SU_ERROR_NONE);
  char char_array[out_length];
  res = SUStringGetUTF8(m_string, out_length+1, &char_array[0], &out_length);
  assert(res == SU_ERROR_NONE);
  return std::string(char_array);
}

String::operator std::string() const {
  return std_string();
}

//char& String::operator [](size_t i) {
  
//}


size_t String::size() const {
  size_t out_length = 0;
  SU_RESULT res = SUStringGetUTF8Length(m_string, &out_length);
  assert(res == SU_ERROR_NONE);
  return out_length;
}
  
bool String::empty() const {
  // size of 1 is empty due to the \n character at the end.
  if (size() == 1) {
    return true;
  }
  return false;
}


} /* namespace CW */
