//
//  LineStyle.hpp
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

#ifndef LineStyle_hpp
#define LineStyle_hpp

#include <SketchUpAPI/model/line_style.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class String;
class Color;

/**
 * @brief C++ wrapper for SULineStyleRef.
 *
 * A LineStyle defines the visual appearance of lines (stipple pattern, width,
 * color, etc.). LineStyles are obtained from the model's LineStyles manager.
 * Built-in styles cannot be modified; only user-created copies can be
 * changed.
 *
 * @note There is no SULineStyleCreate function — new styles can only be
 *   created by copying an existing one via create_copy().
 *
 * @see SULineStyleRef
 * @since SketchUp 2019, API 7.0
 */
class LineStyle :public Entity {
  private:
  static SULineStyleRef copy_reference(const LineStyle& other);

  public:
  /**
  * @brief Constructs a NULL LineStyle object.
  */
  LineStyle();

  /**
  * @brief Constructs a LineStyle from an existing SULineStyleRef.
  * @param line_style - the SULineStyleRef to wrap.
  * @param attached - flag indicating whether the style is owned by a model.
  */
  LineStyle(SULineStyleRef line_style, bool attached = true);

  /**
  * @brief Copy constructor.
  */
  LineStyle(const LineStyle& other);

  /** @brief Destructor. */
  ~LineStyle();

  /**
  * @brief Copy assignment operator.
  */
  LineStyle& operator=(const LineStyle& other);

  /**
  * @brief Returns the SULineStyleRef object.
  */
  SULineStyleRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null line style.
  */
  bool operator!() const;

  /**
  * @brief Creates a copy of this line style with a new name.
  * @since SketchUp 2020.1, API 8.1
  * @param name - the name for the copied style.
  * @return A new LineStyle object (not attached to any model).
  * @throws std::logic_error if the line style is null.
  */
  LineStyle create_copy(const std::string& name) const;

  /**
  * @brief Retrieves the name of the line style.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the line style is null.
  */
  String name() const;

  /**
  * @brief Retrieves the width of the line style in pixels.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the line style is null.
  */
  double width_pixels() const;

  /**
  * @brief Sets the width of the line style in pixels.
  * @since SketchUp 2019, API 7.0
  * @param width - the new width (must be > 0).
  * @throws std::logic_error if the line style is null.
  * @throws std::invalid_argument if setting fails (e.g. built-in style).
  */
  void width_pixels(double width);

  /**
  * @brief Retrieves the length multiplier for the stipple pattern.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the line style is null.
  */
  double length_multiplier() const;

  /**
  * @brief Sets the length multiplier for the stipple pattern.
  * @since SketchUp 2019, API 7.0
  * @param multiplier - the new multiplier (must be non-zero).
  * @throws std::logic_error if the line style is null.
  * @throws std::invalid_argument if setting fails (e.g. built-in style).
  */
  void length_multiplier(double multiplier);

  /**
  * @brief Retrieves the color of the line style.
  * @since SketchUp 2020.1, API 8.1
  * @param has_color - set to true if the line style has a color, false
  *   if no color is set.
  * @return Color object (only meaningful if has_color is true).
  * @throws std::logic_error if the line style is null.
  */
  Color color(bool& has_color) const;

  /**
  * @brief Sets the color of the line style.
  * @since SketchUp 2020.1, API 8.1
  * @param color - the new color.
  * @throws std::logic_error if the line style is null.
  * @throws std::invalid_argument if setting fails (e.g. built-in style).
  */
  void color(const Color& color);

  /**
  * @brief Retrieves whether this is a user-created line style.
  * @since SketchUp 2020.1, API 8.1
  * @throws std::logic_error if the line style is null.
  */
  bool is_user_created() const;
};

} /* namespace CW */

#endif /* LineStyle_hpp */
