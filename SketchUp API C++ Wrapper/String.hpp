//
//  String.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 13/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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
  String(const std::string string_input, StringEncoding enc = StringEncoding::UTF8);
  String(const char string_input[]);
  String(const unichar string_input[]);
  
  ~String();
  
  operator SUStringRef*() {return &m_string;}
  operator SUStringRef() {return m_string;}
  
  /*
  * Convert to std::string
  */
  operator std::string();

  /*
  * Return the SUStringRef object.
  */
  SUStringRef ref();
	
  /*
  * Return the length of the string, in bytes. Including the null terminator at the end.
  */
  size_t size();
  
};

} /* namespace CW */


#endif /* String_hpp */
