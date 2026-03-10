//
//  GuideLine.hpp
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

#ifndef GuideLine_hpp
#define GuideLine_hpp

#include <stdio.h>

#include <SketchUpAPI/model/guide_line.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

namespace CW {

class GuideLine :public DrawingElement {
  private:
  static SUGuideLineRef create_guide_line_finite(const Point3D& start, const Point3D& end);
  static SUGuideLineRef create_guide_line_infinite(const Point3D& point, const Vector3D& direction);
  static SUGuideLineRef copy_reference(const GuideLine& other);

  public:
  /**
  * @brief Constructs a NULL GuideLine object.
  */
  GuideLine();

  /**
  * @brief Constructs a finite GuideLine from start and end points.
  * @since SketchUp 2016, API 4.0
  * @param start - the start position.
  * @param end - the end position.
  */
  GuideLine(const Point3D& start, const Point3D& end);

  /**
  * @brief Constructs a GuideLine from an existing SUGuideLineRef.
  * @param guide_line - SUGuideLineRef object.
  * @param attached - flag indicating whether the guide line is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  GuideLine(SUGuideLineRef guide_line, bool attached = true);

  /** @brief Copy Constructor */
  GuideLine(const GuideLine& other);

  /** @brief Destructor */
  ~GuideLine();

  /** @brief Copy assignment operator */
  GuideLine& operator=(const GuideLine& other);

  /**
  * @brief Creates a finite guide line object. The generated line will be a
  *   segment with start and end points. The end point can be obtained by
  *   adding the direction vector to the start point.
  * @since SketchUp 2016, API 4.0
  * @param start - the guide line start position.
  * @param end - the guide line end position.
  */
  static GuideLine create_finite(const Point3D& start, const Point3D& end);

  /**
  * @brief Creates an infinite guide line object. The generated line will be
  *   infinite, defined with a point along the line and a direction vector.
  * @since SketchUp 2016, API 4.0
  * @param point - a point on the guide line.
  * @param direction - the guide line direction vector.
  */
  static GuideLine create_infinite(const Point3D& point, const Vector3D& direction);

  /**
  * @brief Returns the SUGuideLineRef object.
  */
  SUGuideLineRef ref() const;
  operator SUGuideLineRef() const;
  operator SUGuideLineRef*();

  /**
  * @brief Returns true if this is an invalid/null guide line.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the data defining the line (a point, a direction vector,
  *   and a boolean flagging if the line is infinite). For finite lines, start
  *   is the start point, and the end point can be obtained by adding the
  *   direction vector to the start point. For infinite lines, start is simply
  *   a point on the guide line, and direction is always a unit vector.
  * @since SketchUp 2016, API 4.0
  * @param[out] start - a point on the guide line.
  * @param[out] direction - the guide line direction.
  * @return true if the line is infinite, false if finite.
  */
  bool get_data(Point3D& start, Vector3D& direction) const;

  /**
  * @brief Returns whether the guide line is infinite.
  * @since SketchUp 2016, API 4.0
  */
  bool is_infinite() const;

  /**
  * @brief Retrieves the start point of the guide line. For infinite lines,
  *   this is simply a point that lies on the line.
  * @since SketchUp 2016, API 4.0
  */
  Point3D start() const;

  /**
  * @brief Retrieves the direction vector of the guide line. For infinite
  *   lines this is always a unit vector. For finite lines this is the vector
  *   from start to end.
  * @since SketchUp 2016, API 4.0
  */
  Vector3D direction() const;

  /**
  * @brief For a finite guide line, returns the end point (start + direction).
  *   For infinite lines, the result is undefined.
  * @since SketchUp 2016, API 4.0
  */
  Point3D end() const;

  /**
  * @brief Creates a copy of this guide line object, duplicating all properties.
  * @return a new unattached GuideLine object with the same properties.
  * @throws std::logic_error if this guide line is null.
  */
  GuideLine copy() const;
};

} /* namespace CW */

#endif /* GuideLine_hpp */
