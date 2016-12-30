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
#include <vector>

#include <SketchUpAPI/model/typed_value.h>


namespace CW {

// Forward Declarations
class Color;
class String;
class Vector3D;

class TypedValue {
	private:
  SUTypedValueRef m_typed_value;
  
  static SUTypedValueRef create_typed_value();
  
  public:
  TypedValue();
  TypedValue(SUTypedValueRef typed_val);
  TypedValue(const char chars[]);
  //TypedValue(const std::string string);
  
  ~TypedValue();
  
  operator SUTypedValueRef();
  operator SUTypedValueRef*();
  
  /**
  * Retrieves the type information of a typed value object.
  * @see enum SUTypedValueType for values
  */
  SUTypedValueType get_type() const;

  /**
  * Retrieves/ Set the byte value of a typed value object.
  */
  char byte_value() const;
  TypedValue& byte_value(char byte_val);
	operator char() const;
  
  /**
 	* Retrieves/Sets the int16 value of a typed value object.
  */
  int16_t int16_value() const;
  TypedValue& int16_value(int16_t int16_val);
	operator int16_t() const;

  /**
 	* Retrieves/Sets the int32 value of a typed value object.
  */
  int32_t int32_value() const;
  TypedValue& int32_value(int32_t int32_val);
	operator int32_t() const;

  /**
  * Retrieves/Sets the float value of a typed value object.
  */
  float float_value() const;
  TypedValue& float_value(float float_val);
	operator float() const;

  /**
  * Retrieves/Sets the double value of a typed value object.
  */
  double double_value() const;
  TypedValue& double_value(double double_val);
	operator double() const;

  /**
  * Retrieves the boolean value of a typed value object.
  */
  bool bool_value() const;
  TypedValue& bool_value(bool bool_val);
	operator bool() const;

  /**
  * Retrieves the color value of a typed value object.
  */
  Color color_value() const;
  TypedValue& color_value(Color color_val);
	operator Color() const;

  /**
  * Retrieves/Sets the time value of a typed value object.  The time value is in seconds since January 1, 1970.
  */
  int64_t time_value() const;
  TypedValue& time_value(int64_t time_val);
	operator int64_t() const;

  /**
  * Retrieves/Sets the string value of a typed value object.
  */
  String string_value() const;
  TypedValue& string_value(String string_val);
  TypedValue& string_value(std::string string_val);
	operator String() const;
	operator std::string() const;

  /**
  * Retrieves/Sets the 3-element vector value of a typed value object
  */
  Vector3D vector_value() const;
  TypedValue& vector_value(Vector3D vector_val);
	operator Vector3D() const;

  /**
  * Retrieve/Set the array of typed value objects of a type value object.
  */
  std::vector<TypedValue> typed_value_array() const;
  TypedValue& typed_value_array(std::vector<TypedValue> typed_val_array);
	operator std::vector<TypedValue>() const;

};

} /* namespace CW */
#endif /* TypedValue_hpp */
