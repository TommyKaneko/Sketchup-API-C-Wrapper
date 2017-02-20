//
//  AttributeDictionary.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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
