//
//  DimensionLinear.hpp
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

#ifndef DimensionLinear_hpp
#define DimensionLinear_hpp

#include <SketchUpAPI/model/dimension_linear.h>

#include "SUAPI-CppWrapper/model/Dimension.hpp"

namespace CW {

/**
 * @brief C++ wrapper for SUDimensionLinearRef.
 *
 * A DimensionLinear represents a linear (distance) measurement annotation
 * in the SketchUp model. Linear dimensions measure the distance between
 * two connection points.
 *
 * Linear dimensions extend the Dimension base class and inherit its common
 * properties (text, plane, 3D flag, arrow type, font).
 *
 * @see SUDimensionLinearRef
 * @since SketchUp 2017, API 5.0
 */
class DimensionLinear :public Dimension {
  private:
  static SUDimensionLinearRef create_dimension_linear(
    const Point3D& start, const Point3D& end, double offset);

  static SUDimensionLinearRef copy_reference(const DimensionLinear& other);

  public:
  /**
  * @brief Constructs a NULL DimensionLinear object.
  */
  DimensionLinear();

  /**
  * @brief Constructs a new DimensionLinear between two points with an offset.
  * @since SketchUp 2017, API 5.0
  * @param start - the start point.
  * @param end - the end point.
  * @param offset - the offset distance from the measured entities.
  */
  DimensionLinear(const Point3D& start, const Point3D& end, double offset);

  /**
  * @brief Constructs a DimensionLinear from an existing SUDimensionLinearRef.
  * @since SketchUp 2017, API 5.0
  * @param dimension - SUDimensionLinearRef object.
  * @param attached - flag indicating whether the dimension is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  DimensionLinear(SUDimensionLinearRef dimension, bool attached = true);

  /** @brief Copy constructor */
  DimensionLinear(const DimensionLinear& other);

  /** @brief Destructor */
  ~DimensionLinear();

  /** @brief Copy assignment operator */
  DimensionLinear& operator=(const DimensionLinear& other);

  /**
  * @brief Returns the SUDimensionLinearRef object.
  */
  SUDimensionLinearRef linear_ref() const;

  /**
  * @brief Retrieves the start point of the dimension.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  Point3D start_point() const;

  /**
  * @brief Sets the start connection point to an arbitrary position in space.
  * @since SketchUp 2017, API 5.0
  * @param point - the start point to set.
  * @throws std::logic_error if the dimension is null.
  */
  void start_point(const Point3D& point);

  /**
  * @brief Retrieves the end point of the dimension.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  Point3D end_point() const;

  /**
  * @brief Sets the end connection point to an arbitrary position in space.
  * @since SketchUp 2017, API 5.0
  * @param point - the end point to set.
  * @throws std::logic_error if the dimension is null.
  */
  void end_point(const Point3D& point);

  /**
  * @brief Retrieves the x-axis of the dimension (the axis along the length
  *   of the dimension).
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  Vector3D x_axis() const;

  /**
  * @brief Sets the x-axis of the dimension.
  * @since SketchUp 2017, API 5.0
  * @param axis - the x-axis vector to set.
  * @throws std::logic_error if the dimension is null.
  */
  void x_axis(const Vector3D& axis);

  /**
  * @brief Retrieves the normal vector of the dimension. The normal is a unit
  *   vector pointing out of the plane of the linear dimension.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  Vector3D normal() const;

  /**
  * @brief Sets the normal vector of the dimension.
  * @since SketchUp 2017, API 5.0
  * @param normal - the normal vector to set.
  * @throws std::logic_error if the dimension is null.
  */
  void normal(const Vector3D& normal);

  /**
  * @brief Retrieves the 2D position of the dimension text in the dimension's
  *   plane.
  * @since SketchUp 2017, API 5.0
  * @return a pair containing (x, y) position in the dimension's plane.
  * @throws std::logic_error if the dimension is null.
  */
  std::pair<double, double> position() const;

  /**
  * @brief Sets the 2D position of the dimension text.
  * @since SketchUp 2017, API 5.0
  * @param x - the x position in the dimension's plane.
  * @param y - the y position in the dimension's plane.
  * @throws std::logic_error if the dimension is null.
  */
  void position(double x, double y);

  /**
  * @brief Retrieves the vertical text alignment type.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  SUVerticalTextPositionType vertical_alignment() const;

  /**
  * @brief Sets the vertical text alignment type.
  * @since SketchUp 2017, API 5.0
  * @param alignment - the alignment type to set.
  * @throws std::logic_error if the dimension is null.
  */
  void vertical_alignment(SUVerticalTextPositionType alignment);

  /**
  * @brief Retrieves the horizontal text alignment type.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  SUHorizontalTextPositionType horizontal_alignment() const;

  /**
  * @brief Sets the horizontal text alignment type.
  * @since SketchUp 2017, API 5.0
  * @param alignment - the alignment type to set.
  * @throws std::logic_error if the dimension is null.
  */
  void horizontal_alignment(SUHorizontalTextPositionType alignment);

  /**
  * @brief Retrieves the linear dimension alignment type (aligned, vertical,
  *   or horizontal).
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the dimension is null.
  */
  SUDimensionLinearAlignmentType alignment() const;

  /**
  * @brief Retrieves the 3D position of the dimension text attachment point.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the dimension is null.
  */
  Point3D text_position() const;
};

} /* namespace CW */

#endif /* DimensionLinear_hpp */
