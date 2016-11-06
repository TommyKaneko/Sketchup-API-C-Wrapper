//
//  TypedValue.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 13/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef TypedValue_hpp
#define TypedValue_hpp

#include <stdio.h>
#include <string>

#include <SketchUpAPI/model/typed_value.h>


namespace CW {

class TypedValue {
	private:
  SUTypedValueRef m_typed_value;
  
  public:
  TypedValue();
  TypedValue(const char chars[]);
  TypedValue(const std::string string);
  
  operator SUTypedValueRef();
  const operator SUTypedValueRef*();
	
  /** 
  * TypedValue can be converted to a number of different types.
  */
  operator std::string();
  operator double();
};

} /* namespace CW */
#endif /* TypedValue_hpp */
