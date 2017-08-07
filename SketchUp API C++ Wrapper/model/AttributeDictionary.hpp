//
//  AttributeDictionary.hpp
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

#ifndef AttributeDictionary_hpp
#define AttributeDictionary_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include <SketchUpAPI/model/attribute_dictionary.h>
#include <SketchUpAPI/model/entity.h>

#include "String.hpp"
#include "TypedValue.hpp"

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
