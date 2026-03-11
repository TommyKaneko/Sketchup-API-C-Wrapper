//
//  Schema.hpp
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

#ifndef Schema_hpp
#define Schema_hpp

#include <string>
#include <SketchUpAPI/model/schema.h>

namespace CW {

// Forward declarations
class String;

/**
 * @brief C++ wrapper for SUSchemaRef.
 *
 * A Schema represents a classification schema loaded into the model's
 * Classifications manager. It provides access to schema types by name
 * and the schema's own name.
 *
 * This object is not owned — it references data managed by the
 * Classifications object and must not be released.
 *
 * @see SUSchemaRef
 * @since SketchUp 2015, API 3.0
 */
class Schema {
  private:
  SUSchemaRef m_schema;

  public:
  /**
  * @brief Constructs a NULL Schema object.
  */
  Schema();

  /**
  * @brief Constructs from an existing SUSchemaRef.
  * @param schema - the SUSchemaRef obtained from Classifications.
  */
  Schema(SUSchemaRef schema);

  /**
  * @brief Returns the underlying SUSchemaRef.
  */
  SUSchemaRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null schema.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the name of this schema.
  * @since SketchUp 2022.0, API 10.0
  * @throws std::logic_error if the schema is null.
  */
  String name() const;

  /**
  * @brief Retrieves a schema type by name from this schema.
  * @since SketchUp 2015, API 3.0
  * @param schema_type_name - the name of the schema type.
  * @return SUSchemaTypeRef for the requested type (may be invalid if not found).
  * @throws std::logic_error if the schema is null.
  */
  SUSchemaTypeRef get_schema_type(const std::string& schema_type_name) const;
};

} /* namespace CW */

#endif /* Schema_hpp */
