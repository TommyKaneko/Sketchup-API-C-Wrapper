//
//  DynamicComponentAttribute.hpp
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

#ifndef DynamicComponentAttribute_hpp
#define DynamicComponentAttribute_hpp

#include <SketchUpAPI/model/dynamic_component_attribute.h>

namespace CW {

// Forward declarations
class String;

/**
 * @brief C++ wrapper for SUDynamicComponentAttributeRef.
 *
 * Represents a single attribute of a dynamic component, providing access
 * to the attribute's name, display name, visibility, and display value.
 *
 * This object is not owned — it references data managed by the
 * DynamicComponentInfo object.
 *
 * @see SUDynamicComponentAttributeRef
 * @since SketchUp 2016, API 4.0
 */
class DynamicComponentAttribute {
  private:
  SUDynamicComponentAttributeRef m_attribute;

  public:
  /**
  * @brief Constructs a NULL DynamicComponentAttribute.
  */
  DynamicComponentAttribute();

  /**
  * @brief Constructs from an existing SUDynamicComponentAttributeRef.
  * @param attribute - the SUDynamicComponentAttributeRef to wrap.
  */
  DynamicComponentAttribute(SUDynamicComponentAttributeRef attribute);

  /**
  * @brief Returns the underlying SUDynamicComponentAttributeRef.
  */
  SUDynamicComponentAttributeRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null attribute.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the internal name of this attribute.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the attribute is null.
  */
  String name() const;

  /**
  * @brief Retrieves the display name of this attribute.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the attribute is null.
  */
  String display_name() const;

  /**
  * @brief Retrieves the visibility setting of this attribute.
  * @since SketchUp 2016, API 4.0
  * @return true if the attribute is visible to users.
  * @throws std::logic_error if the attribute is null.
  */
  bool visible() const;

  /**
  * @brief Retrieves the display value of this attribute.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the attribute is null.
  */
  String display_value() const;
};

} /* namespace CW */

#endif /* DynamicComponentAttribute_hpp */
