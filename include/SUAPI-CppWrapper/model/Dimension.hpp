//
//  Dimension.hpp
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

#ifndef Dimension_hpp
#define Dimension_hpp

#include <SketchUpAPI/model/dimension.h>
#include <SketchUpAPI/model/arrow_type.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

namespace CW {

// Forward declarations
class String;
class Font;
class DimensionLinear;
class DimensionRadial;

/**
 * @brief C++ wrapper for SUDimensionRef.
 *
 * A Dimension is the base class for linear and radial dimension entities.
 * Dimensions are drawing elements that display measurement annotations in
 * the model. This class provides access to the properties common to all
 * dimension types (text, plane, 3D flag, arrow type, font).
 *
 * @note The SketchUp C API does not provide SUEntitiesAddDimension(). Dimensions
 *   can be created as DimensionLinear or DimensionRadial and retrieved from
 *   entities, but there is no general add-dimension method.
 *
 * @see SUDimensionRef
 * @since SketchUp 2017, API 5.0
 */
class Dimension :public DrawingElement {
  public:
  /**
  * @brief Constructs a NULL Dimension object.
  */
  Dimension();

  /**
  * @brief Constructs a Dimension from an existing SUDimensionRef.
  * @since SketchUp 2017, API 5.0
  * @param dimension - SUDimensionRef object.
  * @param attached - flag indicating whether the dimension is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  Dimension(SUDimensionRef dimension, bool attached = true);

  /** @brief Copy constructor */
  Dimension(const Dimension& other);

  /** @brief Copy assignment operator */
  Dimension& operator=(const Dimension& other);

  /**
  * @brief Returns the SUDimensionRef object.
  */
  SUDimensionRef ref() const;
  operator SUDimensionRef() const;
  operator SUDimensionRef*();

  /**
  * @brief Returns true if this is an invalid/null dimension.
  */
  bool operator!() const;

  /**
   * @brief Returns the dimension as a DimensionLinear object. This is effectively an upcast operation. If the dimension is not linear, an exception is thrown.
   * @return DimensionLinear object with the same underlying SUDimensionRef as this dimension.
   * @throws std::logic_error if the dimension is null or not linear.
   */
  DimensionLinear as_linear() const;

  /**
   * @brief Returns the dimension as a DimensionRadial object. This is effectively an upcast operation. If the dimension is not radial, an exception is thrown.
   * @return DimensionRadial object with the same underlying SUDimensionRef as this dimension.
   * @throws std::logic_error if the dimension is null or not radial.
   */
  DimensionRadial as_radial() const;

  /**
  * @brief Retrieves the dimension type (linear or radial).
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  SUDimensionType type() const;

  /**
  * @brief Retrieves the text of the dimension.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  String text() const;

  /**
  * @brief Sets the text of the dimension. Assumed to be UTF-8 encoded.
  * @since SketchUp 2017, API 5.0
  * @param text - the text content to set.
  * @throws std::logic_error if the dimension is null.
  */
  void text(const String& text);
  void text(const std::string& text);

  /**
  * @brief Retrieves the plane of the dimension.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  Plane3D plane() const;

  /**
  * @brief Retrieves whether the dimension text is 3D (positioned in model
  *   space rather than screen space).
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  bool text_3d() const;

  /**
  * @brief Sets whether the dimension text is 3D.
  * @since SketchUp 2017, API 5.0
  * @param is_3d - true for 3D text, false for screen-space text.
  * @throws std::logic_error if the dimension is null.
  */
  void text_3d(bool is_3d);

  /**
  * @brief Retrieves the dimension's arrow type.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  SUArrowType arrow_type() const;

  /**
  * @brief Sets the dimension's arrow type.
  * @since SketchUp 2017, API 5.0
  * @param type - the arrow type to set.
  * @throws std::logic_error if the dimension is null.
  * @throws std::out_of_range if the arrow type is not valid.
  */
  void arrow_type(SUArrowType type);

  /**
  * @brief Retrieves the dimension's font.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the dimension is null.
  */
  Font font() const;

  /**
  * @brief Sets the dimension's font.
  * @since SketchUp 2019, API 7.0
  * @param font - the Font to set.
  * @throws std::logic_error if the dimension is null.
  */
  void font(const Font& font);
};

} /* namespace CW */

#endif /* Dimension_hpp */
