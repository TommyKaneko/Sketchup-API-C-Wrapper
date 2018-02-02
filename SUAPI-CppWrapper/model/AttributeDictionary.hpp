//
//  AttributeDictionary.hpp
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

#ifndef AttributeDictionary_hpp
#define AttributeDictionary_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include <SketchUpAPI/model/attribute_dictionary.h>
#include <SketchUpAPI/model/entity.h>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"

namespace CW {

class TypedValue;
/*
* Entity object wrapper
*/
class AttributeDictionary {
  private:
  SUAttributeDictionaryRef m_dict;
  
  public:
  /**
  * Constructor from existing SUAttributeDictionaryRef object
  * @param dict - existing SUAttributeDictionaryRef object to wrap with this class.
  */
  AttributeDictionary(SUAttributeDictionaryRef dict);
  
  /**
  * Constructor representing a null object.
  */
  AttributeDictionary();
  
  /** Cast to native object **/
  operator SUAttributeDictionaryRef() const;
  operator SUAttributeDictionaryRef*();
  
  /**
  * Returns the value of the attribute with the specified key.
  * @param &key - the key of the attribute
  * @param &default_value - the default value to return if the attribute with the key does not exist.
  */
  TypedValue get_attribute(const std::string &key, const TypedValue &default_value) const;
  
  /**
  * Alias of AttributeDictionary::get_attribute().
  * @return TypedValue object.  If the attribute does not exist, a null TypedValue object will be returned.
  */
  TypedValue get_value(const std::string &key) const;

  /**
  * Sets the specified attribute's value.
  * @param &key - the key of the attribute to set.
  * @param &value - the value to set.
  */
  bool set_attribute(const std::string &key, const TypedValue &value);

  /**
  * Returns a vector array of keys in the Attribute Dictionary.
  */
  std::vector<std::string> get_keys() const;
  
  /**
  * Returns the name of the AttributeDictionary.
  */
  std::string get_name() const;
  
  /**
  * Returns true if this is a null object.
  */
  bool operator!() const;

};

} /* namespace CW */

#endif /* AttributeDictionary_hpp */
