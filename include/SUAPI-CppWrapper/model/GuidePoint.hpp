//
//  GuidePoint.hpp
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

#ifndef GuidePoint_hpp
#define GuidePoint_hpp

#include <SketchUpAPI/model/guide_point.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

namespace CW {

class GuidePoint :public DrawingElement {
  private:
  static SUGuidePointRef create_guide_point(const Point3D& position);
  static SUGuidePointRef copy_reference(const GuidePoint& other);

  public:
  /**
  * @brief Constructs a NULL GuidePoint object.
  */
  GuidePoint();

  /**
  * @brief Constructs a GuidePoint at the given position.
  * @since SketchUp 2014 M1, API 2.1
  * @param position - the 3D position of the guide point.
  */
  GuidePoint(const Point3D& position);

  /**
  * @brief Constructs a GuidePoint from an existing SUGuidePointRef.
  * @param guide_point - SUGuidePointRef object.
  * @param attached - flag indicating whether the guide point is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  GuidePoint(SUGuidePointRef guide_point, bool attached = true);

  /** @brief Copy Constructor */
  GuidePoint(const GuidePoint& other);

  /** @brief Destructor */
  ~GuidePoint();

  /** @brief Copy assignment operator */
  GuidePoint& operator=(const GuidePoint& other);

  /**
  * @brief Returns the SUGuidePointRef object.
  */
  SUGuidePointRef ref() const;
  operator SUGuidePointRef() const;
  operator SUGuidePointRef*();

  /**
  * @brief Returns true if this is an invalid/null guide point.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the position of the guide point object.
  * @since SketchUp 2014 M1, API 2.1
  */
  Point3D position() const;

  /**
  * @brief Retrieves the anchor (from) position of the guide point object.
  *   If the point was created in SketchUp then the anchor is the position
  *   that was first clicked during the point creation. If the point was
  *   created with SUGuidePointCreate the anchor is the origin.
  * @since SketchUp 2016, API 4.0
  */
  Point3D from_position() const;

  /**
  * @brief Retrieves the boolean indicating if the point should be displayed
  *   as a line.
  * @since SketchUp 2016, API 4.0
  */
  bool display_as_line() const;

  /**
  * @brief Creates a copy of this guide point object, duplicating all properties.
  * @return a new unattached GuidePoint object with the same properties.
  * @throws std::logic_error if this guide point is null.
  */
  GuidePoint copy() const;
};

} /* namespace CW */

#endif /* GuidePoint_hpp */
