//
//  String.hpp
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
  operator SUStringRef() {return m_string;}
  
  /**
  * Compares two strings for equality.
  */
  friend bool operator==(const String &lhs, const String &rhs);
  
  /*
  * Convert to std::string
  */
  std::string std_string() const;
  operator std::string() const;
  
//	char& operator [](size_t i);

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
