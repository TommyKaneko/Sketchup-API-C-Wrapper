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

#include <SketchUpAPI/transformation.h>

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
  
  /**
  * Inverts the transformation matrix, regardless of the values on the bottom row of the matrix.
  */
  Transformation matrix_inverse() const;
  
  public:
  /**
  * Construct a Transformation object which would conduct no transformations.
  */
  Transformation();
  
  /**
  *
  */
  Transformation(SUTransformation transformation);
  Transformation(Axes axes, Vector3D translation, double scalar);
  Transformation(Vector3D x_axis, Vector3D y_axis, Vector3D z_axis, Vector3D translation, double scalar);  
  explicit Transformation(double scalar);
  explicit Transformation(Vector3D translation);
  explicit Transformation(Point3D translation);

  
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
  operator SUTransformation*();
  
  /**
  * Return the inverse Transformation object (see inverse Transformation matrices)
  */
  Transformation inverse() const;
  
  /**
  * Returns the axis of the rigid transformation.
  */
  Vector3D x_axis() const;
  Vector3D y_axis() const;
  Vector3D z_axis() const;
  
  /**
  * Normalise the transformation, so that the bottom row of the 4x4 matrix reads (0,0,0,1)
  */
  Transformation& normalize();
  
  /**
  * Retrieves the origin of a rigid transformation.
  */
  Point3D origin() const;
  
  /**
  * Retrieves the translation part of the transformation
  */
  Vector3D translation() const;
  
  /**
  * Mulitplication of Transformation matrices.
  */
  Transformation operator*(Transformation transform);

  /**
  * Return transformed vectors.
  */
  friend Vector3D operator*(const Transformation &lhs, const Vector3D &rhs);
  /** You can't technically multiply a 4x1 matrix by a 4x4 matrix (only the other way around), so return it flipped */
  friend Vector3D operator*(const Vector3D &lhs, const Transformation &rhs);

  /**
  * Return transformed point.
  */
  friend Point3D operator*(const Transformation &lhs, const Point3D &rhs);
  /** You can't technically multiply a 4x1 matrix by a 4x4 matrix (only the other way around), so return it flipped */
  friend Point3D operator*(const Point3D &lhs, const Transformation &rhs);
  
  /**
  * Return transformed plane.
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

/**
* Allows the multiplication operator to be on the other side of the vector.
*/
//Vector3D operator*(const Vector3D &lhs, const Transformation &rhs);
//Point3D operator*(const Point3D &lhs, const Transformation &rhs);
//Plane3D operator*(const Plane3D &lhs, const Transformation &rhs);

/*
static Vector3D operator*(const Vector3D &lhs, const Transformation &rhs)
{  return rhs * lhs;}

static Point3D operator*(const Point3D &lhs, const Transformation &rhs)
{  return rhs * lhs;}
*/

} /* namespace CW */
#endif /* Transformation_hpp */
