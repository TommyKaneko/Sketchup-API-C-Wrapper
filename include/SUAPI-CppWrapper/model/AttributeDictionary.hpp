//
//  AttributeDictionary.hpp
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

#ifndef AttributeDictionary_hpp
#define AttributeDictionary_hpp

#include <vector>
#include <string>

#include <SketchUpAPI/model/attribute_dictionary.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

class TypedValue;
class String;

/**
 * @brief C++ wrapper for SUAttributeDictionaryRef.
 *
 * An attribute dictionary is a collection of key-value pairs where keys
 * are strings and values are TypedValue objects. Attribute dictionaries
 * can be attached to any Entity.
 * @see SUAttributeDictionaryRef
 */
class AttributeDictionary :public Entity {
  /**
   * @brief Creates a SUAttributeDictionaryRef object using the given name.
   * @param name  Name of the created AttributeDictionary object.
   * @since SketchUp 2018, API v6.0
   * @return SUAttributeDictionaryRef object with the given name.
   */
  static SUAttributeDictionaryRef create_attribute_dictionary(const std::string& name);

  /**
   * @brief Creates a SUAttributeDictionaryRef derived from an existing object.
   * @param dict  AttributeDictionary object to derive the new ref from.
   * @since SketchUp 2018, API v6.0
   * @return If the AttributeDictionary is already attached to a model, its
   *         ref is returned. Otherwise a new ref is created (keys and
   *         values will not be copied — this must be done manually).
   */
  static SUAttributeDictionaryRef copy_reference(const AttributeDictionary& dict);

  public:
  /** @brief Constructs a null AttributeDictionary. */
  AttributeDictionary();

  /**
   * @brief Constructs a new AttributeDictionary with the given name.
   * @param name  Name of the dictionary to create.
   * @since SketchUp 2018, API v6.0
   */
  AttributeDictionary(std::string name);

  /**
   * @brief Wraps an existing SUAttributeDictionaryRef.
   * @param dict      Existing SUAttributeDictionaryRef to wrap.
   * @param attached  true if the ref is already owned by a model.
   */
  AttributeDictionary(SUAttributeDictionaryRef dict, bool attached = true);

  /** @brief Copy constructor. */
  AttributeDictionary(const AttributeDictionary& other);

  /**
   * @brief Destructor — releases the dictionary if not attached.
   * @since SketchUp 2018, API v6.0
   */
  ~AttributeDictionary();

  /** @brief Copy assignment operator. */
  AttributeDictionary& operator=(const AttributeDictionary& other);

  /** @brief Returns the underlying SUAttributeDictionaryRef. */
  SUAttributeDictionaryRef ref() const;

  /** @brief Implicit conversion to SUAttributeDictionaryRef. */
  operator SUAttributeDictionaryRef() const;

  /** @brief Returns a pointer to the underlying SUAttributeDictionaryRef. */
  operator SUAttributeDictionaryRef*();

  /**
   * @brief Returns the value of the attribute with the specified key.
   * @param key            The key of the attribute.
   * @param default_value  The value to return if the key does not exist.
   * @return The TypedValue for the key, or @p default_value if not found.
   * @throws std::logic_error if the AttributeDictionary is null.
   */
  TypedValue get_attribute(const std::string &key, const TypedValue &default_value) const;

  /**
   * @brief Returns the value of the attribute with the specified key.
   * @param key  The key of the attribute.
   * @return The TypedValue for the key, or a null TypedValue if not found.
   * @throws std::logic_error if the AttributeDictionary is null.
   */
  TypedValue get_value(const std::string &key) const;

  /**
   * @brief Sets the specified attribute's value.
   * @param key    The key of the attribute to set.
   * @param value  The value to set.
   * @throws std::logic_error if the AttributeDictionary is null.
   * @throws std::logic_error if the dictionary is read-only.
   */
  void set_attribute(const std::string &key, const TypedValue &value);

  /**
   * @brief Returns a vector of all keys in the AttributeDictionary.
   * @return A vector of key strings.
   * @throws std::logic_error if the AttributeDictionary is null.
   */
  std::vector<std::string> get_keys() const;

  /**
   * @brief Returns the name of the AttributeDictionary.
   * @return The dictionary name as a string.
   */
  std::string get_name() const;

  /**
   * @brief Returns the name of the AttributeDictionary.
   * @return The dictionary name as a string.
   */
  std::string name() const;

  /**
   * @brief Checks whether the AttributeDictionary is invalid (null).
   * @return true if the underlying ref is invalid.
   */
  bool operator!() const;

};

} /* namespace CW */

#endif /* AttributeDictionary_hpp */
