//
//  ArcCurve.hpp
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

#ifndef ArcCurve_hpp
#define ArcCurve_hpp

#include <SketchUpAPI/model/arccurve.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

/**
 * @brief C++ wrapper for SUArcCurveRef.
 *
 * An ArcCurve is a special type of Curve entity that represents an arc or
 * full circle. It is defined by a center point, start point, end point, and
 * a normal vector. If the start and end points are the same, a full circle
 * is generated.
 *
 * ArcCurve extends Entity (via SUArcCurveToEntity/SUArcCurveFromEntity).
 *
 * @note When released, the arc curve and its associated edge objects are
 *   also released.
 *
 * @see SUArcCurveRef
 * @since SketchUp 2016, API 4.0
 */
class ArcCurve :public Entity {
  private:
  static SUArcCurveRef create_arc_curve(
    const Point3D& center, const Point3D& start_point,
    const Point3D& end_point, const Vector3D& normal, size_t num_edges);

  static SUArcCurveRef copy_reference(const ArcCurve& other);

  public:
  /**
  * @brief Constructs a NULL ArcCurve object.
  */
  ArcCurve();

  /**
  * @brief Constructs a new ArcCurve.
  * @since SketchUp 2016, API 4.0
  * @param center - the center of the arc.
  * @param start_point - the start point of the arc on the circumference.
  * @param end_point - the end point of the arc on the circumference.
  *   If the same as start_point, a full circle is created.
  * @param normal - the normal vector of the arc plane.
  * @param num_edges - the number of edges to use for the arc.
  */
  ArcCurve(const Point3D& center, const Point3D& start_point,
           const Point3D& end_point, const Vector3D& normal,
           size_t num_edges);

  /**
  * @brief Constructs an ArcCurve from an existing SUArcCurveRef.
  * @since SketchUp 2016, API 4.0
  * @param arc_curve - the SUArcCurveRef to wrap.
  * @param attached - flag indicating whether the arc is owned by a parent.
  */
  ArcCurve(SUArcCurveRef arc_curve, bool attached = true);

  /**
  * @brief Copy constructor.
  */
  ArcCurve(const ArcCurve& other);

  /** @brief Destructor */
  ~ArcCurve();

  /**
  * @brief Copy assignment operator.
  */
  ArcCurve& operator=(const ArcCurve& other);

  /**
  * @brief Returns the SUArcCurveRef object.
  */
  SUArcCurveRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null arc curve.
  */
  bool operator!() const;

  /**
  * @brief Creates a detached copy of this arc curve.
  * @throws std::logic_error if the arc curve is null.
  * @return A new detached ArcCurve with the same geometry.
  */
  ArcCurve copy() const;

  /**
  * @brief Retrieves the number of edges in the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  size_t num_edges() const;

  /**
  * @brief Retrieves the radius of the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  double radius() const;

  /**
  * @brief Retrieves the start point of the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  Point3D start_point() const;

  /**
  * @brief Retrieves the end point of the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  Point3D end_point() const;

  /**
  * @brief Retrieves the x-axis of the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  Vector3D x_axis() const;

  /**
  * @brief Retrieves the y-axis of the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  Vector3D y_axis() const;

  /**
  * @brief Retrieves the center point of the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  Point3D center() const;

  /**
  * @brief Retrieves the normal vector of the arc curve.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  Vector3D normal() const;

  /**
  * @brief Retrieves the start angle of the arc curve (in radians).
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  double start_angle() const;

  /**
  * @brief Retrieves the end angle of the arc curve (in radians).
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  double end_angle() const;

  /**
  * @brief Retrieves whether the arc curve is a full circle.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the arc curve is null.
  */
  bool is_full_circle() const;
};

} /* namespace CW */

#endif /* ArcCurve_hpp */
