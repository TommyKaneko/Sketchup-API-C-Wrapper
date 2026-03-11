//
//  ClassificationInfo.hpp
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

#ifndef ClassificationInfo_hpp
#define ClassificationInfo_hpp

#include <string>
#include <SketchUpAPI/model/classification_info.h>

namespace CW {

// Forward declarations
class String;
class ClassificationAttribute;

/**
 * @brief C++ wrapper for SUClassificationInfoRef.
 *
 * Contains names of schemas and schema types applied to a component instance,
 * along with their classification attributes. Created from a ComponentInstance
 * and must be released when no longer needed.
 *
 * This object is non-copyable and moveable. It owns the underlying ref
 * and releases it on destruction.
 *
 * @see SUClassificationInfoRef
 * @since SketchUp 2017, API 5.0
 */
class ClassificationInfo {
  private:
  SUClassificationInfoRef m_classification_info;

  public:
  /**
  * @brief Constructs a NULL ClassificationInfo.
  */
  ClassificationInfo();

  /**
  * @brief Constructs from an existing SUClassificationInfoRef. Takes ownership.
  * @param info - the SUClassificationInfoRef to wrap (will be released on destruction).
  */
  ClassificationInfo(SUClassificationInfoRef info);

  /** @brief Non-copyable. */
  ClassificationInfo(const ClassificationInfo&) = delete;
  ClassificationInfo& operator=(const ClassificationInfo&) = delete;

  /** @brief Move constructor. */
  ClassificationInfo(ClassificationInfo&& other) noexcept;

  /** @brief Move assignment. */
  ClassificationInfo& operator=(ClassificationInfo&& other) noexcept;

  /** @brief Destructor. Releases the classification info. */
  ~ClassificationInfo();

  /**
  * @brief Returns the underlying SUClassificationInfoRef.
  */
  SUClassificationInfoRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null classification info.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the number of schemas applied to the component.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the info is null.
  */
  size_t num_schemas() const;

  /**
  * @brief Retrieves the schema name at the given index.
  * @since SketchUp 2017, API 5.0
  * @param index - the schema index.
  * @throws std::logic_error if the info is null.
  */
  String schema_name(size_t index) const;

  /**
  * @brief Retrieves the schema type at the given index.
  * @since SketchUp 2017, API 5.0
  * @param index - the schema index.
  * @throws std::logic_error if the info is null.
  */
  String schema_type(size_t index) const;

  /**
  * @brief Retrieves the classification attribute at the given index.
  * @since SketchUp 2017, API 5.0
  * @param index - the schema index.
  * @throws std::logic_error if the info is null.
  */
  ClassificationAttribute schema_attribute(size_t index) const;

  /**
  * @brief Retrieves a classification attribute by its path.
  * @since SketchUp 2017, API 5.0
  * @param path - the attribute path string.
  * @throws std::logic_error if the info is null.
  */
  ClassificationAttribute schema_attribute_by_path(const String& path) const;
};

} /* namespace CW */

#endif /* ClassificationInfo_hpp */
