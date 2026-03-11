//
//  DimensionStyle.hpp
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

#ifndef DimensionStyle_hpp
#define DimensionStyle_hpp

#include <SketchUpAPI/model/dimension_style.h>
#include <SketchUpAPI/model/arrow_type.h>
#include <SketchUpAPI/color.h>

namespace CW {

// Forward declarations
class Font;
class Color;

/**
 * @brief C++ wrapper for SUDimensionStyleRef.
 *
 * A DimensionStyle defines the default appearance of dimension entities
 * in a model. There is one DimensionStyle per model, accessible via
 * Model::dimension_style().
 *
 * This is a read-only class — all properties are getters only.
 * DimensionStyle has no Create/Release functions; it is owned by the model.
 *
 * @see SUDimensionStyleRef
 * @since SketchUp 2017, API 5.0
 */
class DimensionStyle {
  private:
  SUDimensionStyleRef m_style;

  public:
  /**
  * @brief Constructs a NULL DimensionStyle object.
  */
  DimensionStyle();

  /**
  * @brief Constructs a DimensionStyle from an existing SUDimensionStyleRef.
  * @since SketchUp 2017, API 5.0
  * @param style - SUDimensionStyleRef object.
  */
  DimensionStyle(SUDimensionStyleRef style);

  /**
  * @brief Returns the SUDimensionStyleRef object.
  */
  SUDimensionStyleRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null dimension style.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the font of the dimension style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  Font font() const;

  /**
  * @brief Retrieves whether the dimension style has 3D text.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  bool has_3d() const;

  /**
  * @brief Retrieves the arrow type specified by the dimension style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  SUArrowType arrow_type() const;

  /**
  * @brief Retrieves the arrow size specified by the dimension style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  size_t arrow_size() const;

  /**
  * @brief Retrieves the color specified by the dimension style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  Color color() const;

  /**
  * @brief Retrieves the text color specified by the dimension style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  Color text_color() const;

  /**
  * @brief Retrieves the extension line offset. The offset specifies the gap
  *   between the connection points' locations and the dimension leader lines.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  size_t extension_line_offset() const;

  /**
  * @brief Retrieves the extension line overshoot. The overshoot specifies
  *   the distance that leader lines extend past the connection arrows.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  size_t extension_line_overshoot() const;

  /**
  * @brief Retrieves the line weight specified by the dimension style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  size_t line_weight() const;

  /**
  * @brief Retrieves whether non-associative dimensions should be highlighted.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  bool highlight_non_associative() const;

  /**
  * @brief Retrieves the highlight color for non-associative dimensions.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  Color highlight_non_associative_color() const;

  /**
  * @brief Retrieves whether radius/diameter prefixes are shown on radial
  *   dimensions.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  bool show_radial_prefix() const;

  /**
  * @brief Retrieves whether out-of-plane dimensions are hidden.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  bool hide_out_of_plane() const;

  /**
  * @brief Retrieves the out-of-plane tolerance for hiding dimensions.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  double hide_out_of_plane_value() const;

  /**
  * @brief Retrieves whether small dimensions are hidden.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  bool hide_small() const;

  /**
  * @brief Retrieves the minimum size under which dimensions will be hidden
  *   if hide_small() returns true.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the style is null.
  */
  double hide_small_value() const;
};

} /* namespace CW */

#endif /* DimensionStyle_hpp */
