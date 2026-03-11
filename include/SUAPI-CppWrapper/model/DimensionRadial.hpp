//
//  DimensionRadial.hpp
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

#ifndef DimensionRadial_hpp
#define DimensionRadial_hpp

#include <array>
#include <SketchUpAPI/model/dimension_radial.h>

#include "SUAPI-CppWrapper/model/Dimension.hpp"

namespace CW {

// Forward declarations
class InstancePath;

/**
 * @brief C++ wrapper for SUDimensionRadialRef.
 *
 * A DimensionRadial represents a radial (radius or diameter) measurement
 * annotation in the SketchUp model. Radial dimensions measure the radius or
 * diameter of an arc curve entity.
 *
 * Radial dimensions extend the Dimension base class and inherit its common
 * properties (text, plane, 3D flag, arrow type, font).
 *
 * @note Creation of a DimensionRadial requires a valid InstancePath whose
 *   leaf entity is an arc curve.
 *
 * @see SUDimensionRadialRef
 * @since SketchUp 2017, API 5.0
 */
class DimensionRadial :public Dimension {
  public:
  /**
  * @brief Constructs a NULL DimensionRadial object.
  */
  DimensionRadial();

  /**
  * @brief Constructs a DimensionRadial from an existing SUDimensionRadialRef.
  * @since SketchUp 2017, API 5.0
  * @param dimension - SUDimensionRadialRef object.
  * @param attached - flag indicating whether the dimension is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  DimensionRadial(SUDimensionRadialRef dimension, bool attached = true);

  /** @brief Destructor */
  ~DimensionRadial();

  /**
  * @brief Returns the SUDimensionRadialRef object.
  */
  SUDimensionRadialRef radial_ref() const;

  /**
  * @brief Retrieves the instance path of the arc curve being measured.
  *
  * The returned InstancePath object must be managed by the caller.
  *
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  InstancePath curve_instance_path() const;

  /**
  * @brief Sets the arc curve instance to be measured. The instance path's
  *   leaf entity must be an arc curve.
  * @since SketchUp 2017, API 5.0
  * @param path - the instance path to the arc curve.
  * @throws std::logic_error if the dimension is null.
  * @throws std::invalid_argument if the path does not refer to a valid
  *   arc curve instance.
  */
  void curve_instance_path(const InstancePath& path);

  /**
  * @brief Retrieves whether the dimension measures diameter or radius.
  * @since SketchUp 2017, API 5.0
  * @return true if measuring diameter, false for radius.
  * @throws std::logic_error if the dimension is null.
  */
  bool is_diameter() const;

  /**
  * @brief Sets whether the dimension measures diameter or radius.
  * @since SketchUp 2017, API 5.0
  * @param diameter - true to measure diameter, false for radius.
  * @throws std::logic_error if the dimension is null.
  */
  void is_diameter(bool diameter);

  /**
  * @brief Retrieves the leader line break point. The leader line break point
  *   is the point where the leader line bends towards the dimension label.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  Point3D leader_break_point() const;

  /**
  * @brief Sets the leader line break point.
  * @since SketchUp 2017, API 5.0
  * @param point - the break point to set.
  * @throws std::logic_error if the dimension is null.
  */
  void leader_break_point(const Point3D& point);

  /**
  * @brief Retrieves the three leader points of the dimension.
  *
  * The three points are:
  *   - [0] the point at which the dimension's text touches the leader line
  *   - [1] the point at which the dimension's arrow attaches to the arc curve
  *   - [2] the point on the arc curve's full circle opposite of point [1]
  *
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the dimension is null.
  */
  std::array<Point3D, 3> leader_points() const;
};

} /* namespace CW */

#endif /* DimensionRadial_hpp */
