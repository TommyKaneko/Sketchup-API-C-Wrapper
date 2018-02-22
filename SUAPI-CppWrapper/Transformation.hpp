//
//  Transformation.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
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

#ifndef Transformation_hpp
#define Transformation_hpp

#include <stdio.h>
#include <array>

#include <SketchUpAPI/geometry/transformation.h>

#include "SUAPI-CppWrapper/Geometry.hpp"

namespace CW {

class Axes;
class Face;

class Transformation {
  private:
  SUTransformation m_transformation;
  constexpr static double EPSILON = 0.001; // Sketchup Tolerance is 1/1000"
  
  /**
  * Multiplies 4x1 matrix by this transformation matrix
  * @param matrix4_1 array of size 4 to mulitply with this transformation.
  * @return double[4] array representing 4x1 matrix
  */
  std::array<double, 4> multiply4x1(std::array<double, 4> matrix4_1) const;
  
  /**
  * Returns the determinant of the matrix.
  */
  double determinant() const;
  
  public:
  /**
  * Construct a Transformation with a simple scale of 1 (no change).
  * @since SketchUp 2018, API v6.0
  */
  Transformation();
  
  /**
  * Construct a Transformation from native SUTransformation object.
  */
  Transformation(SUTransformation transformation);

  /**
  * Construct a Transformation by setting axes and translation.
  * @since SketchUp 2018, API v6.0
  */
  Transformation(const Axes& axes, const Vector3D& translation, double scalar = 1.0);

  /**
  * Construct a Transformation by setting axes and translation.
  * @since SketchUp 2018, API v6.0
  */
  Transformation(const Point3D& origin, const Vector3D& x_axis, const Vector3D& y_axis, const Vector3D& z_axis, double scalar = 1.0);
  
  /**
  * Construct a scale Transformation.
  * @since SketchUp 2018, API v6.0
  */
  explicit Transformation(double scalar);

  /**
  * Construct a non-uniform scale Transformation.
  * @since SketchUp 2018, API v6.0
  */
  Transformation(double x_scale, double y_scale, double z_scale);

  /**
  * Construct a translation Transformation.
  * @since SketchUp 2018, API v6.0
  */
  explicit Transformation(const Vector3D& translation);
  
  /**
  * Construct a Transformation from a point representing the translation, and a scale.
  * @since SketchUp 2018, API v6.0
  */
  Transformation(const Point3D& translation, double scalar);

  /**
  * Construct a Transformation from a point representing the origin (translation), and vector representing the Z-Axis.  The other two axes in the transformed space are computed using the "Arbitrary axis algorithm".
  * @since SketchUp 2017, API v5.0
  */
  Transformation(const Point3D& translation, const Vector3D& normal);
  
  /**
  * Construct a Transformation given an origin, vector of rotation, and angle.
  * @since SketchUp 2018, API v6.0
  * @param point - The point specifying the translation component of the transformation.
  * @param vector - The vector about which rotation will occur.
  * @param angle - The rotation in radians for the transformation.
  */
  Transformation(const Point3D& point, const Vector3D& vector, double angle);

  /**
  * Construct a Transformation object from an interpolation between two transformations. The weight determines the amount of interpolation. A weight of 0.0 would return a transformation of t1, while a weight of 1.0 would return a transformation of t2.
  * @since SketchUp 2018, API v6.0
  * @param transform1 - The first transformation object.
  * @param transform2 - The second transformation object.
  * @param weight - The weight determines the amount of interpolation from t1 to t2.
  */
  Transformation(const Transformation& transform1, const Transformation& transform2, double weight);

  /*
  * Allows access to the array of numbers in the SUTransformation struct.
  */
  double operator[](size_t i) const;
  double& operator[](size_t i);
  
  /*
  * Cast to SUTransformation struct
  */
  SUTransformation ref() const;
  operator SUTransformation() const;
  operator const SUTransformation*() const;
  
  /**
  * Returns true if this Transformation is identity (no change).
  */
  bool is_identity() const;
  
  /**
  * Return the inverse Transformation object (see inverse Transformation matrices)
  * @since SketchUp 2018, API v6.0
  */
  Transformation inverse() const;
  
  /**
  * Returns the X-axis of the rigid transformation.
  * @since SketchUp 2018, API v6.0
  */
  Vector3D x_axis() const;

  /**
  * Returns the Y-axis of the rigid transformation.
  * @since SketchUp 2018, API v6.0
  */
  Vector3D y_axis() const;

  /**
  * Returns the Z-axis of the rigid transformation.
  * @since SketchUp 2018, API v6.0
  */
  Vector3D z_axis() const;
  
  /**
  * Returns the rotation about the Z-axis in radians.
  * @since SketchUp 2018, API v6.0
  */
  double z_rotation() const;
  
  /**
  * Normalise the transformation, so that the bottom row of the 4x4 matrix reads (0,0,0,1)
  */
  Transformation& normalize();
  
  /**
  * Retrieves the origin of a rigid transformation.
  * @since SketchUp 2018, API v6.0
  */
  Point3D origin() const;
  
  /**
  * Retrieves the translation part of the transformation
  */
  Vector3D translation() const;
  
  /**
  * Mulitplication of Transformation matrices.
  * @since SketchUp 2018, API v6.0
  */
  Transformation operator*(Transformation transform);

  /**
  * Return transformed vectors.
  * @since SketchUp 2018, API v6.0
  */
  friend Vector3D operator*(const Transformation &lhs, const Vector3D &rhs);
  /** You can't technically multiply a 4x1 matrix by a 4x4 matrix (only the other way around), so return it flipped */
  friend Vector3D operator*(const Vector3D &lhs, const Transformation &rhs);

  /**
  * Return transformed point.
  * @since SketchUp 2018, API v6.0
  */
  friend Point3D operator*(const Transformation &lhs, const Point3D &rhs);
  /** You can't technically multiply a 4x1 matrix by a 4x4 matrix (only the other way around), so return it flipped */
  friend Point3D operator*(const Point3D &lhs, const Transformation &rhs);
  
  /**
  * Return transformed plane.
  * @since SketchUp 2018, API v6.0
  */
  friend Plane3D operator*(const Plane3D &lhs, const Transformation &rhs);
  friend Plane3D operator*(const Transformation &lhs, const Plane3D &rhs);

  /**
  * Return transformed face.
  */
  friend Face operator*(const Face &lhs, const Transformation &rhs);
  
  /**
  * Compare equality of tranformation objects.
  */
  bool equal(const Transformation transform, const double epsilon = EPSILON) const;
  bool operator==(const Transformation transform) const;
  
  /**
  * Returns a Transformation object that represents rotation of the given angle in radians about a line.  The rotation direction follows the right hand rule along the direction vector of the line.
  */
  static Transformation transformation_rotate_about_line(const double angle, const Line3D line);
};

} /* namespace CW */
#endif /* Transformation_hpp */
