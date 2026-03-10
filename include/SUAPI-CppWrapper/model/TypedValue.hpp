//
//  TypedValue.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
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

#ifndef TypedValue_hpp
#define TypedValue_hpp

#include <string>
#include <vector>

#include <SketchUpAPI/model/typed_value.h>


namespace CW {

// Forward Declarations
class Color;
class String;
class Vector3D;

/**
 * @brief C++ wrapper for SUTypedValueRef — a generic value container.
 *
 * A typed value can hold a byte, int16, int32, float, double, bool,
 * Color, time (int64), String, Vector3D, or an array of TypedValues.
 * @see SUTypedValueRef
 */
class TypedValue {
  private:
  SUTypedValueRef m_typed_value;

  /** @brief Whether the underlying ref is owned by another object. */
  bool m_attached;

  /** @brief Creates a new SUTypedValueRef via the C API. */
  static SUTypedValueRef create_typed_value();

  public:
  /** @brief Constructs a new, empty TypedValue. */
  TypedValue();

  /**
   * @brief Wraps an existing SUTypedValueRef.
   * @param typed_val  The C API typed value reference.
   * @param attached   true if the ref is owned externally (will not be released).
   */
  TypedValue(SUTypedValueRef typed_val, bool attached = false);

  /** @brief Copy constructor. */
  TypedValue(const TypedValue& other);

  /** @brief Copy assignment operator. */
  TypedValue& operator= (const TypedValue& other);

  /** @brief Destructor — releases the typed value if not attached. */
  ~TypedValue();

  /** @brief Returns the underlying SUTypedValueRef. */
  SUTypedValueRef ref() const;

  /** @brief Implicit conversion to SUTypedValueRef. */
  operator SUTypedValueRef();

  /** @brief Returns a pointer to the underlying SUTypedValueRef. */
  operator SUTypedValueRef*();

  /**
   * @brief Checks whether the TypedValue is invalid (null).
   * @return true if the underlying ref is invalid.
   */
  bool operator!() const;

  /**
   * @brief Checks whether the TypedValue is empty.
   * @return true if the ref is invalid or the type is SUTypedValueType_Empty.
   */
  bool empty() const;

  /**
   * @brief Retrieves the type information of a typed value object.
   * @return The SUTypedValueType enum value.
   * @throws std::logic_error if the TypedValue is null.
   * @see SUTypedValueType
   */
  SUTypedValueType get_type() const;

  /**
   * @brief Retrieves the byte value of a typed value object.
   * @return The byte value.
   * @throws std::logic_error if the TypedValue is null or not of byte type.
   */
  char byte_value() const;

  /**
   * @brief Sets the byte value of a typed value object.
   * @param byte_val  The byte value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& byte_value(const char byte_val);
  operator char() const;

  /** @brief Constructs a TypedValue from a byte. */
  TypedValue(const char byte_val);

  /**
   * @brief Retrieves the int16 value of a typed value object.
   * @return The int16 value.
   * @throws std::logic_error if the TypedValue is null or not of int16 type.
   */
  int16_t int16_value() const;

  /**
   * @brief Sets the int16 value of a typed value object.
   * @param int16_val  The int16 value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& int16_value(const int16_t int16_val);
  operator int16_t() const;

  /** @brief Constructs a TypedValue from an int16. */
  TypedValue(const int16_t int16_val);

  /**
   * @brief Retrieves the int32 value of a typed value object.
   * @return The int32 value.
   * @throws std::logic_error if the TypedValue is null or not of int32 type.
   */
  int32_t int32_value() const;

  /**
   * @brief Sets the int32 value of a typed value object.
   * @param int32_val  The int32 value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& int32_value(const int32_t int32_val);
  operator int32_t() const;

  /** @brief Constructs a TypedValue from an int32. */
  TypedValue(const int32_t int32_val);

  /**
   * @brief Retrieves the float value of a typed value object.
   * @return The float value.
   * @throws std::logic_error if the TypedValue is null or not of float type.
   */
  float float_value() const;

  /**
   * @brief Sets the float value of a typed value object.
   * @param float_val  The float value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& float_value(const float float_val);
  operator float() const;

  /** @brief Constructs a TypedValue from a float. */
  TypedValue(const float float_val);

  /**
   * @brief Retrieves the double value of a typed value object.
   * @return The double value.
   * @throws std::logic_error if the TypedValue is null or not of double type.
   */
  double double_value() const;

  /**
   * @brief Sets the double value of a typed value object.
   * @param double_val  The double value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& double_value(const double double_val);
  operator double() const;

  /** @brief Constructs a TypedValue from a double. */
  TypedValue(const double double_val);

  /**
   * @brief Retrieves the boolean value of a typed value object.
   * @return The boolean value.
   * @throws std::logic_error if the TypedValue is null or not of bool type.
   */
  bool bool_value() const;

  /**
   * @brief Sets the boolean value of a typed value object.
   * @param bool_val  The boolean value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& bool_value(bool bool_val);
  operator bool() const;

  /** @brief Constructs a TypedValue from a bool. */
  TypedValue(const bool bool_val);

  /**
   * @brief Retrieves the color value of a typed value object.
   * @return A Color object.
   * @throws std::logic_error if the TypedValue is null or not of color type.
   */
  Color color_value() const;

  /**
   * @brief Sets the color value of a typed value object.
   * @param color_val  The Color value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& color_value(const Color &color_val);
  operator Color() const;

  /** @brief Constructs a TypedValue from a Color. */
  TypedValue(const Color &color_val);

  /**
   * @brief Retrieves the time value of a typed value object.
   *
   * The time value is in seconds since January 1, 1970.
   * @return The time value as int64_t.
   * @throws std::logic_error if the TypedValue is null or not of time type.
   */
  int64_t time_value() const;

  /**
   * @brief Sets the time value of a typed value object.
   *
   * The time value is in seconds since January 1, 1970.
   * @param time_val  The time value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& time_value(int64_t time_val);
  operator int64_t() const;

  /** @brief Constructs a TypedValue from a time value (seconds since epoch). */
  TypedValue(const int64_t time_value);

  /**
   * @brief Retrieves the string value of a typed value object.
   * @return A String object.
   * @throws std::logic_error if the TypedValue is null or not of string type.
   */
  String string_value() const;

  /**
   * @brief Sets the string value of a typed value object.
   * @param string_val  The String value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& string_value(const String &string_val);

  /**
   * @brief Sets the string value of a typed value object.
   * @param string_val  The std::string value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& string_value(const std::string &string_val);
  operator String() const;
  operator std::string() const;

  /** @brief Constructs a TypedValue from a C string. */
  explicit TypedValue(const char chars[]);

  /** @brief Constructs a TypedValue from a std::string. */
  TypedValue(const std::string& string);

  /** @brief Constructs a TypedValue from a String. */
  TypedValue(const String& string);

  /**
   * @brief Retrieves the 3-element vector value of a typed value object.
   * @return A Vector3D object.
   * @throws std::logic_error if the TypedValue is null or not of vector type.
   */
  Vector3D vector_value() const;

  /**
   * @brief Sets the 3-element vector value of a typed value object.
   * @param vector_val  The Vector3D value to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& vector_value(const Vector3D &vector_val);
  operator Vector3D() const;

  /** @brief Constructs a TypedValue from a Vector3D. */
  TypedValue(const Vector3D& vector);

  /**
   * @brief Retrieves the array of typed value objects.
   * @return A vector of TypedValue objects.
   * @throws std::logic_error if the TypedValue is null or not of array type.
   */
  std::vector<TypedValue> typed_value_array() const;

  /**
   * @brief Sets the array of typed value objects.
   * @param typed_val_array  The vector of TypedValue objects to set.
   * @return A reference to this TypedValue.
   */
  TypedValue& typed_value_array(std::vector<TypedValue> &typed_val_array);
  operator std::vector<TypedValue>() const;

  /** @brief Constructs a TypedValue from an array of TypedValues. */
  TypedValue(std::vector<TypedValue>& array);

  /**
   * @brief Equality comparison operator.
   * @return true if both TypedValues have the same type and value.
   */
  friend bool operator== (const TypedValue &val1, const TypedValue &val2);

  /**
   * @brief Inequality comparison operator.
   * @return true if the TypedValues differ in type or value.
   */
  friend bool operator!= (const TypedValue &val1, const TypedValue &val2);

};

} /* namespace CW */
#endif /* TypedValue_hpp */
