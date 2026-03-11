//
//  ClassificationAttribute.hpp
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

#ifndef ClassificationAttribute_hpp
#define ClassificationAttribute_hpp

#include <string>
#include <vector>
#include <SketchUpAPI/model/classification_attribute.h>

namespace CW {

// Forward declarations
class String;
class TypedValue;

/**
 * @brief C++ wrapper for SUClassificationAttributeRef.
 *
 * Represents attribute data about a classified component. Classification
 * attributes can form a tree structure with child attributes.
 *
 * @see SUClassificationAttributeRef
 * @since SketchUp 2017, API 5.0
 */
class ClassificationAttribute {
  private:
  SUClassificationAttributeRef m_attribute;

  public:
  /**
  * @brief Constructs a NULL ClassificationAttribute.
  */
  ClassificationAttribute();

  /**
  * @brief Constructs a ClassificationAttribute from an existing ref.
  * @param attribute - the SUClassificationAttributeRef to wrap.
  */
  ClassificationAttribute(SUClassificationAttributeRef attribute);

  /**
  * @brief Returns the underlying SUClassificationAttributeRef.
  */
  SUClassificationAttributeRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null attribute.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the value of the attribute.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the attribute is null.
  */
  TypedValue value() const;

  /**
  * @brief Retrieves the path of the attribute.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the attribute is null.
  */
  String path() const;

  /**
  * @brief Retrieves the number of child attributes.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the attribute is null.
  */
  size_t num_children() const;

  /**
  * @brief Retrieves the child attribute at the given index.
  * @since SketchUp 2017, API 5.0
  * @param index - the child index.
  * @throws std::logic_error if the attribute is null.
  */
  ClassificationAttribute child(size_t index) const;
};

} /* namespace CW */

#endif /* ClassificationAttribute_hpp */
