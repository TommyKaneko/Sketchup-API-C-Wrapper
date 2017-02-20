//
//  Transformation.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Transformation_hpp
#define Transformation_hpp

#include <stdio.h>
#include <array>

#include <SketchUpAPI/transformation.h>

#include "Geometry.hpp"

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
  
  public:
  Transformation();
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
  * Cast to SUTransformation object
  */
  SUTransformation ref() const;
  operator SUTransformation() const;
  operator SUTransformation*();
  
  /**
  * Return the inverse Transformation object (see inverse Transformation matrices)
  */
  Transformation inverse();
  
  /**
  * Returns the axis of the rigid transformation.
  */
  Vector3D x_axis();
  Vector3D y_axis();
  Vector3D z_axis();
  
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
  friend Vector3D operator*(const Vector3D &lhs, const Transformation &rhs);

  /**
  * Return transformed point.
  */
  friend Point3D operator*(const Point3D &lhs, const Transformation &rhs);
  
  /**
  * Return transformed plane.
  */
  friend Plane3D operator*(const Plane3D &lhs, const Transformation &rhs);

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
  * Returns a Transformation object that represents rotation of the given angle in radians about a line.
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
