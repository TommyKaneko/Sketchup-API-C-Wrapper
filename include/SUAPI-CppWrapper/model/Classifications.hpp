//
//  Classifications.hpp
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

#ifndef Classifications_hpp
#define Classifications_hpp

#include <string>
#include <vector>
#include <SketchUpAPI/model/classifications.h>

namespace CW {

// Forward declarations
class Schema;

/**
 * @brief C++ wrapper for SUClassificationsRef.
 *
 * Classifications is a manager object that provides access to the
 * classification schemas loaded in a model. It is obtained from Model
 * and cannot be created or released independently.
 *
 * @see SUClassificationsRef
 * @since SketchUp 2015, API 3.0
 */
class Classifications {
  private:
  SUClassificationsRef m_classifications;

  public:
  /**
  * @brief Constructs a NULL Classifications object.
  */
  Classifications();

  /**
  * @brief Constructs from an existing SUClassificationsRef.
  * @param classifications - the SUClassificationsRef obtained from the model.
  */
  Classifications(SUClassificationsRef classifications);

  /**
  * @brief Returns the underlying SUClassificationsRef.
  */
  SUClassificationsRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null classifications manager.
  */
  bool operator!() const;

  /**
  * @brief Loads a schema into the classifications.
  * @since SketchUp 2015, API 3.0
  * @param schema_file_name - the full path of the schema file to load.
  * @throws std::logic_error if the classifications manager is null.
  * @throws std::invalid_argument if the schema file path is invalid.
  */
  void load_schema(const std::string& schema_file_name);

  /**
  * @brief Retrieves a schema by name.
  * @since SketchUp 2015, API 3.0
  * @param schema_name - the name of the schema.
  * @return Schema object (may be null if not found).
  * @throws std::logic_error if the classifications manager is null.
  */
  Schema get_schema(const std::string& schema_name) const;

  /**
  * @brief Retrieves the number of schemas loaded.
  * @since SketchUp 2022.0, API 10.0
  * @throws std::logic_error if the classifications manager is null.
  */
  size_t num_schemas() const;

  /**
  * @brief Retrieves all loaded schemas.
  * @since SketchUp 2022.0, API 10.0
  * @throws std::logic_error if the classifications manager is null.
  */
  std::vector<Schema> schemas() const;
};

} /* namespace CW */

#endif /* Classifications_hpp */
