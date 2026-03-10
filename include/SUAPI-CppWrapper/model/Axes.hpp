//
//  Axes.hpp
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

#ifndef Axes_hpp
#define Axes_hpp

#include <vector>
#include <string>

#include <SketchUpAPI/model/axes.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"

namespace CW {

/**
 * @brief C++ wrapper for SUAxesRef.
 *
 * Represents a set of coordinate axes defined by an origin and three
 * orthogonal unit vectors (x, y, z).
 * @see SUAxesRef
 */
class Axes :public Entity {
  private:
  /** @brief Creates a new default SUAxesRef via SUAxesCreate(). */
  static SUAxesRef create_axes();

  /**
   * @brief Creates a custom SUAxesRef from an origin and three orthogonal axes.
   * @param origin  The origin point.
   * @param xaxis   The x-axis vector.
   * @param yaxis   The y-axis vector.
   * @param zaxis   The z-axis vector.
   * @return A valid SUAxesRef, or SU_INVALID if the vectors are not orthogonal.
   */
  static SUAxesRef create_custom_axes(const SUPoint3D& origin, const SUVector3D& xaxis, const SUVector3D& yaxis, const SUVector3D& zaxis);

  /** @brief Returns a copied SUAxesRef from the given Axes object. */
  static SUAxesRef copy_reference(const Axes& other);

  public:
  /** @brief Constructs a null Axes object. */
  Axes();

  /**
   * @brief Wraps an existing SUAxesRef.
   * @param axes      Existing SUAxesRef to wrap.
   * @param attached  true if the ref is already owned by a model.
   */
  Axes(SUAxesRef axes, bool attached = true);

  /**
   * @brief Constructs an Axes object from the given origin and axis vectors.
   *
   * The vectors must be orthogonal; otherwise a null object is created.
   * @param origin  The origin of the axes.
   * @param x_axis  Vector representing the x axis.
   * @param y_axis  Vector representing the y axis.
   * @param z_axis  Vector representing the z axis.
   */
  Axes(Point3D origin, Vector3D x_axis, Vector3D y_axis, Vector3D z_axis);

  /** @brief Copy constructor. */
  Axes(const Axes& other);

  /** @brief Destructor — releases the axes if not attached. */
  ~Axes();

  /** @brief Returns the underlying SUAxesRef. */
  SUAxesRef ref() const;

  /** @brief Copy assignment operator. */
  Axes& operator=(const Axes& other);

  /**
   * @brief Checks whether the Axes is invalid (null).
   * @return true if the underlying ref is invalid.
   */
  bool operator!() const;

  /**
   * @brief Retrieves the x-axis vector.
   * @return The x-axis as a Vector3D.
   * @throws std::logic_error if the Axes is null.
   */
  Vector3D x_axis() const;

  /**
   * @brief Retrieves the y-axis vector.
   * @return The y-axis as a Vector3D.
   * @throws std::logic_error if the Axes is null.
   */
  Vector3D y_axis() const;

  /**
   * @brief Retrieves the z-axis vector.
   * @return The z-axis as a Vector3D.
   * @throws std::logic_error if the Axes is null.
   */
  Vector3D z_axis() const;

  /**
   * @brief Retrieves the origin of the axes.
   * @return The origin as a Point3D.
   * @throws std::logic_error if the Axes is null.
   */
  Point3D origin() const;

  /**
   * @brief Retrieves the transformation of the axes.
   * @return A Transformation representing the axes' coordinate system.
   * @throws std::logic_error if the Axes is null.
   */
  Transformation transformation() const;
};

} /* namespace CW */
#endif /* Axes_hpp */
