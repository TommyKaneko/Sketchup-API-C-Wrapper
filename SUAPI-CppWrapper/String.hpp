//
//  String.hpp
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

#ifndef String_hpp
#define String_hpp

#include <stdio.h>
#include <string>

#include <SketchUpAPI/unicodestring.h>
namespace CW {

/*
* StringRef wrapper
*/
enum class StringEncoding {
  UTF8,
  UTF16
};

class String {
  private:
  SUStringRef m_string;
  StringEncoding m_encoding;
  
  // Disallow copying for simplicity
  //String(const String& copy);
  //String& operator= (const String& copy);
  
  static SUStringRef create_string_ref();
  static SUStringRef create_string_ref(const std::string string_input, StringEncoding enc = StringEncoding::UTF8);
  static SUStringRef create_string_ref(const char string_input[]);
  static SUStringRef create_string_ref(const unichar string_input[]);
  
  public:
  String();
  String(SUStringRef string_ref);
  String(const std::string &string_input, StringEncoding enc = StringEncoding::UTF8);
  String(const char string_input[]);
  String(const unichar string_input[]);
  
  /** Copy Constructor */
  String(const String& other);
  
  /** Copy Assignment operator */
  String& operator=(String& other);
  
  
  ~String();
  
  operator SUStringRef&() {return m_string;}
  operator SUStringRef*() {return &m_string;}
  operator SUStringRef() const {return m_string;}
  
  /**
  * Compares two strings for equality.
  */
  friend bool operator==(const String &lhs, const String &rhs);
  
  /*
  * Convert to std::string
  */
  std::string std_string() const;
  operator std::string() const;
  
//  char& operator [](size_t i);

  /*
  * Return the SUStringRef object.
  */
  SUStringRef ref() const;
  
  /*
  * Return the length of the string, in bytes. Including the null terminator at the end.
  */
  size_t size() const;
  
  /**
  * Return whether this string is empty or not.
  */
  bool empty() const;
  
};

} /* namespace CW */


#endif /* String_hpp */
