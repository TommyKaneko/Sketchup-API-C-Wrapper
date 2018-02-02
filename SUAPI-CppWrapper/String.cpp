//
//  String.cpp
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
    SUResult res = SUStringRelease(&m_string);
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
  SUResult res = SUStringCreate(&string_ref);
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
    SUResult res = SUStringRelease(&m_string);
    assert(res == SU_ERROR_NONE);
  }
}

SUStringRef String::ref() const {
  return m_string;
}

std::string String::std_string() const {
  size_t out_length = 0;
  SUResult res = SUStringGetUTF8Length(m_string, &out_length);
  assert(res == SU_ERROR_NONE);
  out_length++; // Allow for null termianted string
  char* char_array = new char[out_length];
  res = SUStringGetUTF8(m_string, out_length, &char_array[0], &out_length);
  assert(res == SU_ERROR_NONE);
  std::string str(char_array);
  delete[] char_array;
  return str;
}

String::operator std::string() const {
  return std_string();
}

//char& String::operator [](size_t i) {
  
//}


size_t String::size() const {
  size_t out_length = 0;
  SUResult res = SUStringGetUTF8Length(m_string, &out_length);
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
