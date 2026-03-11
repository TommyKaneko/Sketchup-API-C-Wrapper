//
//  Style.hpp
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

#ifndef Style_hpp
#define Style_hpp

#include <string>
#include <SketchUpAPI/model/style.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class String;
class TypedValue;

/**
 * @brief C++ wrapper for SUStyleRef.
 *
 * A Style defines the visual rendering settings of a SketchUp model,
 * including edge display, background color, watermarks, and other
 * rendering properties. Styles are managed by the Styles collection
 * in the model.
 *
 * @see SUStyleRef
 * @since SketchUp 2017, API 5.0
 */
class Style :public Entity {
  private:
  static SUStyleRef create_style();
  static SUStyleRef copy_reference(const Style& other);

  public:
  /**
  * @brief Constructs a NULL Style object.
  */
  Style();

  /**
  * @brief Constructs a new empty Style.
  * @since SketchUp 2017, API 5.0
  * @param name - the name of the style.
  */
  Style(const std::string& name);

  /**
  * @brief Constructs a Style from an existing SUStyleRef.
  * @param style - the SUStyleRef to wrap.
  * @param attached - flag indicating whether the style is owned by a model.
  */
  Style(SUStyleRef style, bool attached = true);

  /**
  * @brief Copy constructor.
  */
  Style(const Style& other);

  /** @brief Destructor. */
  ~Style();

  /**
  * @brief Copy assignment operator.
  */
  Style& operator=(const Style& other);

  /**
  * @brief Returns the SUStyleRef object.
  */
  SUStyleRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null style.
  */
  bool operator!() const;

  // TODO: Styles cannot be created independently of a model, so copying is not currently supported. To copy a style, save it to file and load it into the other model.
  // /**
  // * @brief Creates a copy of the style, with all of its properties duplicated. The copy is not attached to a model.
  // */
  // Style copy() const;

  /**
  * @brief Retrieves the name of the style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  String name() const;

  /**
  * @brief Sets the name of the style.
  * @since SketchUp 2017, API 5.0
  * @param name - the new name.
  * @throws std::logic_error if the style is null.
  */
  void name(const std::string& name);

  /**
  * @brief Retrieves the display name of the style. If the name starts
  *   with '*', this returns the name without the prefix.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  String display_name() const;

  /**
  * @brief Retrieves the description of the style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  String description() const;

  /**
  * @brief Sets the description of the style.
  * @since SketchUp 2017, API 5.0
  * @param description - the new description.
  * @throws std::logic_error if the style is null.
  */
  void description(const std::string& description);

  /**
  * @brief Retrieves the file path the style was loaded from.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  String path() const;

  /**
  * @brief Retrieves the GUID of the style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  String guid() const;

  /**
  * @brief Retrieves whether the style displays a watermark.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  bool displays_watermark() const;

  /**
  * @brief Saves the style to a file.
  * @since SketchUp 2017, API 5.0
  * @param file_path - the path to save to.
  * @throws std::logic_error if the style is null.
  */
  void save_to_file(const std::string& file_path) const;

  /**
  * @brief Gets a style property value.
  * @since SketchUp 2017, API 5.0
  * @param type - the property type (SUStylePropertyType).
  * @return TypedValue containing the property value.
  * @throws std::logic_error if the style is null.
  */
  TypedValue get_property(enum SUStylePropertyType type) const;

  /**
  * @brief Sets a style property value.
  * @since SketchUp 2017, API 5.0
  * @param type - the property type (SUStylePropertyType).
  * @param value - the value to set.
  * @throws std::logic_error if the style is null.
  */
  void set_property(enum SUStylePropertyType type, const TypedValue& value);
};

} /* namespace CW */

#endif /* Style_hpp */
