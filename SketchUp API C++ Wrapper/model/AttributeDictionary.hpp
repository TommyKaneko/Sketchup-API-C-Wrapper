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
  AttributeDictionary();
  AttributeDictionary(SUAttributeDictionaryRef dict);
  AttributeDictionary(SUEntityRef entity_ref);
	
  TypedValue get_attribute(const std::string &key, const TypedValue &default_value) const;

  bool set_attribute(const std::string &key, const TypedValue &value);

  std::vector<std::string> get_keys() const;

  TypedValue get_value(const std::string &key) const;

  std::string get_name() const;
  
  /**
  * Checks whether this is a valid object.
  */
  operator bool();

};

} /* namespace CW */

#endif /* AttributeDictionary_hpp */
