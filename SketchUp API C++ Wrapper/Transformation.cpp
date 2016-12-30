//
//  Transformation.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include <cassert>
#include <tgmath.h>

#include "Transformation.hpp"

#include "Axes.hpp"

namespace CW {

Transformation::Transformation():
	m_transformation(SUTransformation{1.0, 0.0, 0.0, 0.0, // col-1 (column-major order)
                                    0.0, 1.0, 0.0, 0.0, // col-2
                                    0.0, 0.0, 1.0, 0.0, // col-3
                                    0.0, 0.0, 0.0, 1.0}) // col-4
{}


Transformation::Transformation(SUTransformation transformation):
	m_transformation(transformation)
{}

Transformation::Transformation(Axes axes, Vector3D translation, double scalar):
	m_transformation(axes.transformation() * Transformation(translation) * Transformation(scalar))
{}

Transformation::Transformation(Vector3D x_axis, Vector3D y_axis, Vector3D z_axis, Vector3D translation, double scalar):
	m_transformation(Transformation())
{
	// TODO
}

Transformation::Transformation(double scalar):
	m_transformation(SUTransformation{1.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    0.0, 0.0, 0.0, 1.0/scalar})
{}

Transformation::Transformation(Vector3D translation):
	m_transformation(SUTransformation{1.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    translation.x, translation.y, translation.z, 1.0})
{}
Transformation::Transformation(Point3D translation):
	Transformation(Vector3D(translation))
{}

std::array<double, 4> Transformation::multiply4x1(std::array<double, 4> matrix4_1) const {
	std::array<double, 4> output;
  for (size_t i=0; i < 4; ++i) {
  	output[i] = matrix4_1[i] * (m_transformation.values[i] +
    														m_transformation.values[i+4] +
    														m_transformation.values[i+8] +
    														m_transformation.values[i+12]);
  }
  return output;
}



double Transformation::operator[](size_t i) const {
	assert(i < 16);
  return m_transformation.values[i];
}

double& Transformation::operator[](size_t i) {
	assert(i < 16);
  return m_transformation.values[i];
}


SUTransformation Transformation::ref() const {
	return m_transformation;
}

Transformation::operator SUTransformation() const {
  return ref();
}
  
Transformation Transformation::inverse() {
	assert( m_transformation.values[3] == 0.0 &&
  				m_transformation.values[7] == 0.0 &&
          m_transformation.values[11] == 0.0 &&
          m_transformation.values[15] == 1.0);

  SUTransformation inverted_t;
  // Method for transformation is illustrated here: http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform
  /*
   M = [ R T ]
   		 [ 0 W ]
  where M is 4x4, R is 3x3, T is 3x1, and the bottom row is (0,0,0,1), then
  inv(M) = [ inv(R)   -inv(R) * T ]
           [   0              1   ]
  // First invert the 3x3 sub matrix R (the rotation part)
  */
  // [ 0 4 8 ]
  // [ 1 5 9 ]
  // [ 2 6 10]
  // matrix value = ((1. matrix of minors) * 2. cofactor, 3. swap_diagonals) / 4. determinant
  double matrix_minors3x3[9];
  // 1. Get matrix of minors
  matrix_minors3x3[0] = ((m_transformation.values[5] * m_transformation.values[10]) - (m_transformation.values[6] * m_transformation.values[9]));
  matrix_minors3x3[1] = ((m_transformation.values[4] * m_transformation.values[10]) - (m_transformation.values[6] * m_transformation.values[8]));
  matrix_minors3x3[2] = ((m_transformation.values[4] * m_transformation.values[9]) - (m_transformation.values[5] * m_transformation.values[8]));
  matrix_minors3x3[3] = ((m_transformation.values[1] * m_transformation.values[10]) - (m_transformation.values[2] * m_transformation.values[9]));
  matrix_minors3x3[4] = ((m_transformation.values[0] * m_transformation.values[10]) - (m_transformation.values[2] * m_transformation.values[8]));
  matrix_minors3x3[5] = ((m_transformation.values[0] * m_transformation.values[9]) - (m_transformation.values[1] * m_transformation.values[8]));
  matrix_minors3x3[6] = ((m_transformation.values[1] * m_transformation.values[6]) - (m_transformation.values[2] * m_transformation.values[5]));
  matrix_minors3x3[7] = ((m_transformation.values[0] * m_transformation.values[6]) - (m_transformation.values[2] * m_transformation.values[4]));
  matrix_minors3x3[8] = ((m_transformation.values[1] * m_transformation.values[5]) - (m_transformation.values[1] * m_transformation.values[4]));
	// 4. find determinant
  double determinant = (m_transformation.values[0] * matrix_minors3x3[0]) -
                       (m_transformation.values[4] * matrix_minors3x3[3]) +
                       (m_transformation.values[8] * matrix_minors3x3[6]);
  assert(determinant != 0.0);
  // 2. 3., 4. - create matrix of cofactors, adjudicate and divide by determinant.
  inverted_t.values[0] = matrix_minors3x3[0] / determinant;
  inverted_t.values[1] = matrix_minors3x3[3] * (-1.0) / determinant; // swapped with 4
  inverted_t.values[2] = matrix_minors3x3[6] / determinant; //swapped with 8
  inverted_t.values[4] = matrix_minors3x3[1] * (-1.0) / determinant; // swapped with 1
  inverted_t.values[5] = matrix_minors3x3[4] / determinant;
  inverted_t.values[6] = matrix_minors3x3[7] * (-1.0) / determinant; // swapped with 9
  inverted_t.values[8] = matrix_minors3x3[2] / determinant; // swaped with 2
  inverted_t.values[9] = matrix_minors3x3[5] * (-1.0) / determinant; // swapped with 6
  inverted_t.values[10] = matrix_minors3x3[8] / determinant;
  // Second, calculate inverted sub matrix T (the translation part)  (-inv(R) * T)
  // 				   [ 12 ]
  // -inv(R) * [ 13 ]
  // 				   [ 14 ]
  // matrix cell = column(val1) * row_val1 + ... +column(val3) * row_val3
	inverted_t.values[12] = (-inverted_t.values[0]) * m_transformation.values[12] +
                          (-inverted_t.values[4]) * m_transformation.values[13] +
                          (-inverted_t.values[8]) * m_transformation.values[14];
  inverted_t.values[13] = (-inverted_t.values[1]) * m_transformation.values[12] +
                          (-inverted_t.values[5]) * m_transformation.values[13] +
                          (-inverted_t.values[9]) * m_transformation.values[14];
  inverted_t.values[14] = (-inverted_t.values[2]) * m_transformation.values[12] +
                          (-inverted_t.values[6]) * m_transformation.values[13] +
                          (-inverted_t.values[10]) * m_transformation.values[14];
  // Fill in the fixed values:
  inverted_t.values[3] = 0;
  inverted_t.values[7] = 0;
  inverted_t.values[11] = 0;
  inverted_t.values[15] = 1.0;
  return Transformation(inverted_t);
}
  
  /**
  * Returns the axis of the rigid transformation.
  */
  Vector3D x_axis();
  Vector3D y_axis();
  Vector3D z_axis();


Transformation& Transformation::normalize() {
	if (m_transformation.values[15] == 1.0) {
  	// No normalisation necessary (is already normal)
    return (*this);
  }
  assert(m_transformation.values[15] != 0.0);
  for (size_t i=0; i < 15; ++i) {
  	m_transformation.values[i] = m_transformation.values[i] / m_transformation.values[15];
  }
  m_transformation.values[15] = 1.0;
  return (*this);
}



  /**
  * Retrieves the origin of a rigid transformation.
	*/
Point3D Transformation::origin() const {
	// TODO
  assert(false);
}
  

Vector3D Transformation::translation() const {
	if (m_transformation.values[15] == 1.0) {
  	return Vector3D( m_transformation.values[12], m_transformation.values[13], m_transformation.values[14]);
  }
  return Vector3D( m_transformation.values[12] / m_transformation.values[15], m_transformation.values[13] / m_transformation.values[15], m_transformation.values[14] / m_transformation.values[15]);
}

  
Transformation Transformation::operator*(Transformation transform) {
	double new_matrix[4][4];
  for (size_t col=0; col < 4; ++col) {
  	for (size_t row=0; row < 4; ++row) {
  		new_matrix[col][row] = 0.0;
      for (size_t i=0; i < 4; ++i) {
      	new_matrix[col][row] += ((*this)[(col*4)+i] * transform[(i*4)+row]);
      }
    }
  }
  SUTransformation array_vals = {{new_matrix[0][0], new_matrix[0][1], new_matrix[0][2], new_matrix[0][3],
                                  new_matrix[1][0], new_matrix[1][1], new_matrix[1][2], new_matrix[1][3],
                                  new_matrix[2][0], new_matrix[2][1], new_matrix[2][2], new_matrix[2][3],
                                  new_matrix[3][0], new_matrix[3][1], new_matrix[3][2], new_matrix[3][3]}};
  return Transformation(array_vals);
}
  
/**
* Friend Functions of class Transformation
*/
Vector3D operator*(const Vector3D &lhs, const Transformation &rhs) {
	// More info about multiplying vectors here: http://www.euclideanspace.com/maths/geometry/affine/matrix4x4/index.htm
  std::array<double, 4> matrix4x1{lhs.x, lhs.y, lhs.z, 0.0}; // Set w value to 0 (unaffected by translation)
  std::array<double, 4> return4x1 = rhs.multiply4x1(matrix4x1);
  return Vector3D(return4x1[0], return4x1[1], return4x1[2]);
}

/**
* Friend Functions of class Transformation
*/
Point3D operator*(const Point3D &lhs, const Transformation &rhs) {
	// More info about multiplying vectors here: http://www.euclideanspace.com/maths/geometry/affine/matrix4x4/index.htm
  std::array<double, 4> matrix4x1{lhs.x, lhs.y, lhs.z, 1.0}; // Set w value to 1 (affected by translation)
  std::array<double, 4> return4x1 = rhs.multiply4x1(matrix4x1);
  if (return4x1[3] == 1.0) {
    return Point3D(return4x1[0], return4x1[1], return4x1[2]);
  }
  else {
    return Vector3D(return4x1[0] / return4x1[3], return4x1[1] / return4x1[3], return4x1[2] / return4x1[3]);
  }
}

/**
* Friend Functions of class Transformation
*/
Plane3D operator*(const Plane3D &lhs, const Transformation &rhs) {
	Vector3D plane_normal = lhs.normal();
  Point3D plane_point = plane_normal * lhs.d;
  std::array<double, 4> trans_plane_point = rhs.multiply4x1({plane_point.x, plane_point.y, plane_point.z, 1.0});
  std::array<double, 4> trans_plane_normal = rhs.multiply4x1({plane_normal.x, plane_normal.y, plane_normal.z, 0.0});
  return Plane3D(Point3D(trans_plane_point[0], trans_plane_point[1], trans_plane_point[2]), Vector3D(trans_plane_normal[0], trans_plane_normal[1], trans_plane_normal[2]));
}

  

bool Transformation::equal(const Transformation transform, const double epsilon) const {
	for (size_t i=0; i < 16; ++i) {
  	// Skip bottom row, except last
    if (i == 3 || i == 7 || i == 11) {
    }
    else {
    	if (!(fabs((*this)[i] - transform[i]) < epsilon)) {
      	return false;
      }
    }
  }
  return true;
}

bool Transformation::operator==(const Transformation transform) const {
	return this->equal(transform);
}
  
  
} /* namespace CW */
