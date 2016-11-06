//
//  String.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 13/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "String.hpp"
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

String::String(const std::string string_input, StringEncoding enc):
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

SUStringRef String::create_string_ref() {
  SUStringRef string_ref = SU_INVALID;
	SUStringCreate(&string_ref);
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
	SUStringRelease(&m_string);
}

SUStringRef String::ref() {
	return m_string;
}

String::operator std::string() {
	std::string string;
  // TODO
}

} /* namespace CW */
