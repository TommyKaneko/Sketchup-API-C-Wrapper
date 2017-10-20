//
//  Transformation.cpp
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

#include <cassert>
#include <tgmath.h>

#include "SUAPI-CppWrapper/Transformation.hpp"

#include "SUAPI-CppWrapper/model/Axes.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/LoopInput.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"

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


double Transformation::determinant() const {
  // From: http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
  // Note this is for general 4x4 matrix, and it is excessive for a 3x3 matrix.
  double value =
    m_transformation.values[12] * m_transformation.values[9] * m_transformation.values[6] * m_transformation.values[3]-m_transformation.values[8] * m_transformation.values[13] * m_transformation.values[6] * m_transformation.values[3]-m_transformation.values[12] * m_transformation.values[5] * m_transformation.values[10] * m_transformation.values[3]+m_transformation.values[4] * m_transformation.values[13] * m_transformation.values[10] * m_transformation.values[3]+
    m_transformation.values[8] * m_transformation.values[5] * m_transformation.values[14] * m_transformation.values[3]-m_transformation.values[4] * m_transformation.values[9] * m_transformation.values[14] * m_transformation.values[3]-m_transformation.values[12] * m_transformation.values[9] * m_transformation.values[2] * m_transformation.values[7]+m_transformation.values[8] * m_transformation.values[13] * m_transformation.values[2] * m_transformation.values[7]+
    m_transformation.values[12] * m_transformation.values[1] * m_transformation.values[10] * m_transformation.values[7]-m_transformation.values[0] * m_transformation.values[13] * m_transformation.values[10] * m_transformation.values[7]-m_transformation.values[8] * m_transformation.values[1] * m_transformation.values[14] * m_transformation.values[7]+m_transformation.values[0] * m_transformation.values[9] * m_transformation.values[14] * m_transformation.values[7]+
    m_transformation.values[12] * m_transformation.values[5] * m_transformation.values[2] * m_transformation.values[11]-m_transformation.values[4] * m_transformation.values[13] * m_transformation.values[2] * m_transformation.values[11]-m_transformation.values[12] * m_transformation.values[1] * m_transformation.values[6] * m_transformation.values[11]+m_transformation.values[0] * m_transformation.values[13] * m_transformation.values[6] * m_transformation.values[11]+
    m_transformation.values[4] * m_transformation.values[1] * m_transformation.values[14] * m_transformation.values[11]-m_transformation.values[0] * m_transformation.values[5] * m_transformation.values[14] * m_transformation.values[11]-m_transformation.values[8] * m_transformation.values[5] * m_transformation.values[2] * m_transformation.values[15]+m_transformation.values[4] * m_transformation.values[9] * m_transformation.values[2] * m_transformation.values[15]+
    m_transformation.values[8] * m_transformation.values[1] * m_transformation.values[6] * m_transformation.values[15]-m_transformation.values[0] * m_transformation.values[9] * m_transformation.values[6] * m_transformation.values[15]-m_transformation.values[4] * m_transformation.values[1] * m_transformation.values[10] * m_transformation.values[15]+m_transformation.values[0] * m_transformation.values[5] * m_transformation.values[10] * m_transformation.values[15];
 return value;
}


Transformation Transformation::matrix_inverse() const {
  // Method for transformation is illustrated here: http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform
  const double& i00 = m_transformation.values[0];
  const double& i01 = m_transformation.values[1];
  const double& i02 = m_transformation.values[2];
  const double& i03 = m_transformation.values[3];
  const double& i10 = m_transformation.values[4];
  const double& i11 = m_transformation.values[5];
  const double& i12 = m_transformation.values[6];
  const double& i13 = m_transformation.values[7];
  const double& i20 = m_transformation.values[8];
  const double& i21 = m_transformation.values[9];
  const double& i22 = m_transformation.values[10];
  const double& i23 = m_transformation.values[11];
  const double& i30 = m_transformation.values[12];
  const double& i31 = m_transformation.values[13];
  const double& i32 = m_transformation.values[14];
  const double& i33 = m_transformation.values[15];
  
  double s0 = i00 * i11 - i10 * i01;
  double s1 = i00 * i12 - i10 * i02;
  double s2 = i00 * i13 - i10 * i03;
  double s3 = i01 * i12 - i11 * i02;
  double s4 = i01 * i13 - i11 * i03;
  double s5 = i02 * i13 - i12 * i03;

  double c5 = i22 * i33 - i32 * i23;
  double c4 = i21 * i33 - i31 * i23;
  double c3 = i21 * i32 - i31 * i22;
  double c2 = i20 * i33 - i30 * i23;
  double c1 = i20 * i32 - i30 * i22;
  double c0 = i20 * i31 - i30 * i21;

	double det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
  assert(det != 0.0);
  double invdet = 1 / det;
  
  SUTransformation inverted_t;
  inverted_t.values[0] = (i11 * c5 - i12 * c4 + i13 * c3) * invdet;
  inverted_t.values[1] = (-i01 * c5 + i02 * c4 - i03 * c3) * invdet;
  inverted_t.values[2] = (i31 * s5 - i32 * s4 + i33 * s3) * invdet;
  inverted_t.values[3] = (-i21 * s5 + i22 * s4 - i23 * s3) * invdet;

  inverted_t.values[4] = (-i10 * c5 + i12 * c2 - i13 * c1) * invdet;
  inverted_t.values[5] = (i00 * c5 - i02 * c2 + i03 * c1) * invdet;
  inverted_t.values[6] = (-i30 * s5 + i32 * s2 - i33 * s1) * invdet;
  inverted_t.values[7] = (i20 * s5 - i22 * s2 + i23 * s1) * invdet;

  inverted_t.values[8] = (i10 * c4 - i11 * c2 + i13 * c0) * invdet;
  inverted_t.values[9] = (-i00 * c4 + i01 * c2 - i03 * c0) * invdet;
  inverted_t.values[10] = (i30 * s4 - i31 * s2 + i33 * s0) * invdet;
  inverted_t.values[11] = (-i20 * s4 + i21 * s2 - i23 * s0) * invdet;

  inverted_t.values[12] = (-i10 * c3 + i11 * c1 - i12 * c0) * invdet;
  inverted_t.values[13] = (i00 * c3 - i01 * c1 + i02 * c0) * invdet;
  inverted_t.values[14] = (-i30 * s3 + i31 * s1 - i32 * s0) * invdet;
  inverted_t.values[15] = (i20 * s3 - i21 * s1 + i22 * s0) * invdet;

  return Transformation(inverted_t);
}


std::array<double, 4> Transformation::multiply4x1(std::array<double, 4> matrix4_1) const {
	std::array<double, 4> output;
  for (size_t i=0; i < 4; ++i) {
  	// This one is for multiplying 1x4 by 4x4:
    //output[i] = matrix4_1[i] * (m_transformation.values[i] +
    //														m_transformation.values[i+4] +
    //														m_transformation.values[i+8] +
    //														m_transformation.values[i+12]);
  	// This one is for multiplying 4x4 by 4x1:
    output[i] = (m_transformation.values[i] * matrix4_1[0]) +
    						(m_transformation.values[i+4] * matrix4_1[1]) +
    						(m_transformation.values[i+8] * matrix4_1[2]) +
    						(m_transformation.values[i+12] * matrix4_1[3]);

  }
  return output;
}



double Transformation::operator[](size_t i) const {
  if (i > 15) {
  	throw std::out_of_range("CW::Transformation::operator[](): index range is between 0 and 15");
  }
  return m_transformation.values[i];
}

double& Transformation::operator[](size_t i) {
  if (i > 15) {
  	throw std::out_of_range("CW::Transformation::operator[](): index range is between 0 and 15");
  }
  return m_transformation.values[i];
}


SUTransformation Transformation::ref() const {
	return m_transformation;
}

Transformation::operator SUTransformation() const {
  return ref();
}

Transformation::operator SUTransformation*() {
	return &m_transformation;
}


Transformation Transformation::inverse() const {
	if (m_transformation.values[3] != 0.0 ||
  				m_transformation.values[7] != 0.0 ||
          m_transformation.values[11] != 0.0 ||
          m_transformation.values[15] != 1.0) {
  	return this->matrix_inverse();
  }

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
  /*
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
  b[2, 2] = ( values[12]a[3, 0] * s4 - a[3, 1] * s2 + a[3, 3] * s0) * invdet;
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
  */
  const double& i00 = m_transformation.values[0];
  const double& i01 = m_transformation.values[1];
  const double& i02 = m_transformation.values[2];
  //const double& i03 = m_transformation.values[3];
  const double& i10 = m_transformation.values[4];
  const double& i11 = m_transformation.values[5];
  const double& i12 = m_transformation.values[6];
  //const double& i13 = m_transformation.values[7];
  const double& i20 = m_transformation.values[8];
  const double& i21 = m_transformation.values[9];
  const double& i22 = m_transformation.values[10];
  //const double& i23 = m_transformation.values[11];
  const double& i30 = m_transformation.values[12];
  const double& i31 = m_transformation.values[13];
  const double& i32 = m_transformation.values[14];
  //const double& i33 = m_transformation.values[15];
  
  double s0 = i00 * i11 - i10 * i01;
  double s1 = i00 * i12 - i10 * i02;
  //double s2 = 0.0;
  double s3 = i01 * i12 - i11 * i02;
  //double s4 = 0.0;
  //double s5 = 0.0;

  //double c5 = i22;
  //double c4 = i21;
  double c3 = i21 * i32 - i31 * i22;
  //double c2 = i20;
  double c1 = i20 * i32 - i30 * i22;
  double c0 = i20 * i31 - i30 * i21;

	double det = s0 * i22 - s1 * i21 + s3 * i20;
  assert(det != 0.0);
  double invdet = 1 / det;

  inverted_t.values[0] = (i11 * i22 - i12 * i21) * invdet;
  inverted_t.values[1] = (-i01 * i22 + i02 * i21) * invdet;
  inverted_t.values[2] = s3 * invdet;
  inverted_t.values[3] = 0.0;

  inverted_t.values[4] = (-i10 * i22 + i12 * i20) * invdet;
  inverted_t.values[5] = (i00 * i22 - i02 * i20) * invdet;
  inverted_t.values[6] = -s1 * invdet;
  inverted_t.values[7] = 0.0;

  inverted_t.values[8] = (i10 * i21 - i11 * i20) * invdet;
  inverted_t.values[9] = (-i00 * i21 + i01 * i20) * invdet;
  inverted_t.values[10] = s0 * invdet;
  inverted_t.values[11] = 0.0;

  inverted_t.values[12] = (-i10 * c3 + i11 * c1 - i12 * c0) * invdet;
  inverted_t.values[13] = (i00 * c3 - i01 * c1 + i02 * c0) * invdet;
  inverted_t.values[14] = (-i30 * s3 + i31 * s1 - i32 * s0) * invdet;
  inverted_t.values[15] = 1.0;

  return Transformation(inverted_t);
}
  
  /**
  * Returns the axis of the rigid transformation.
  */
  //Vector3D x_axis();
  //Vector3D y_axis();
  //Vector3D z_axis();


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
      	new_matrix[col][row] += ((*this)[(i*4)+row] * transform[(col*4)+i]);
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
Vector3D operator*(const Transformation &lhs, const Vector3D &rhs) {
  if (!rhs) {
  	throw std::invalid_argument("CW::Transformation::operator*(const Vector3D &lhs, const Transformation &rhs): Vector3D given is null");
  }
  // More info about multiplying vectors here: http://www.euclideanspace.com/maths/geometry/affine/matrix4x4/index.htm
  std::array<double, 4> matrix4x1{rhs.x, rhs.y, rhs.z, 0.0}; // Set w value to 0 (unaffected by translation)
  std::array<double, 4> return4x1 = lhs.multiply4x1(matrix4x1);
  return Vector3D(return4x1[0], return4x1[1], return4x1[2]);
}

Vector3D operator*(const Vector3D &lhs, const Transformation &rhs) {
	// Can't actually multiply a vector by transformation, so return the transforamtion multiplied by the vector
  return rhs * lhs;
}

/**
* Friend Functions of class Transformation
*/
Point3D operator*(const Transformation &lhs, const Point3D &rhs) {
  if (!rhs) {
  	throw std::invalid_argument("CW::Transformation::operator*(const Point3D &lhs, const Transformation &rhs): Point3D given is null");
  }
	// More info about multiplying vectors here: http://www.euclideanspace.com/maths/geometry/affine/matrix4x4/index.htm
  std::array<double, 4> matrix4x1{rhs.x, rhs.y, rhs.z, 1.0}; // Set w value to 1 (affected by translation)
  std::array<double, 4> return4x1 = lhs.multiply4x1(matrix4x1);
  if (return4x1[3] == 1.0) {
    return Point3D(return4x1[0], return4x1[1], return4x1[2]);
  }
  else {
    return Point3D(return4x1[0] / return4x1[3], return4x1[1] / return4x1[3], return4x1[2] / return4x1[3]);
  }
}
Point3D operator*(const Point3D &lhs, const Transformation &rhs) {
	// Can't actually multiply a vector by transformation, so return the transforamtion multiplied by the vector
	return rhs * lhs;
}

/**
* Friend Functions of class Transformation
*/
Plane3D operator*(const Plane3D &lhs, const Transformation &rhs) {
  if (!lhs) {
  	throw std::invalid_argument("CW::Transformation::operator*(const Plane3D &lhs, const Transformation &rhs): Plane3D given is null");
  }
	Vector3D plane_normal = lhs.normal();
  Point3D plane_point = plane_normal * -lhs.d;
  std::array<double, 4> trans_plane_point = rhs.multiply4x1({plane_point.x, plane_point.y, plane_point.z, 1.0});
  std::array<double, 4> trans_plane_normal = rhs.multiply4x1({plane_normal.x, plane_normal.y, plane_normal.z, 0.0});
  return Plane3D(Point3D(trans_plane_point[0], trans_plane_point[1], trans_plane_point[2]), Vector3D(trans_plane_normal[0], trans_plane_normal[1], trans_plane_normal[2]));
}

Plane3D operator*(const Transformation &lhs, const Plane3D &rhs) {
	return rhs * lhs;
}

/**
* Friend Functions of class Transformation
*/
Face operator*(const Face &lhs, const Transformation &rhs) {
  if (!lhs) {
  	throw std::invalid_argument("CW::Transformation::operator*(const Face &lhs, const Transformation &rhs): Face given is null");
  }
	// We transform all the vertices then create loops
  std::vector<Vertex> outer_vertices = lhs.outer_loop().vertices();
  std::vector<Point3D> trans_outer_points;
  trans_outer_points.reserve(outer_vertices.size());
  for (size_t i=0; i < outer_vertices.size(); ++i) {
  	trans_outer_points.push_back(outer_vertices[i].position() * rhs);
  }
  LoopInput outer_loop_input = lhs.outer_loop().loop_input();
  Face trans_face(trans_outer_points, outer_loop_input);
  // Now add inner loops
  std::vector<Loop> inner_loops = lhs.inner_loops();
  for (size_t j=0; j < inner_loops.size(); ++j) {
    std::vector<Vertex> inner_vertices = inner_loops[j].vertices();
    std::vector<Point3D> trans_inner_points;
    trans_inner_points.reserve(inner_vertices.size());
    for (size_t i=0; i < inner_vertices.size(); ++i) {
      trans_inner_points.push_back(inner_vertices[i].position() * rhs);
    }
    LoopInput inner_loop_input = inner_loops[j].loop_input();
    trans_face.add_inner_loop(trans_inner_points, inner_loop_input);
  }
  if (!!lhs.material()) {
  	trans_face.material(lhs.material());
  }
  trans_face.copy_attributes_from(lhs);
  return trans_face;
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


/**
* Publc Static Methods
*/
Transformation Transformation::transformation_rotate_about_line(const double angle, const Line3D line){
  if (!line) {
  	throw std::invalid_argument("CW::Transformation::transformation_rotate_about_line(): Line3D given is null");
  }
  // Solution derived from this article: http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
  SUTransformation transform;
  double u = line.direction.x;
  double v = line.direction.y;
  double w = line.direction.z;
  double a = line.point.x;
  double b = line.point.y;
  double c = line.point.z;
  double cos_angle = cos(angle);
  double sin_angle = sin(angle);
  double u2 = pow(u,2);
  double v2 = pow(v,2);
  double w2 = pow(w,2);
  double au = a * u;
  double av = a * v;
  double aw = a * w;
  double bu = b * u;
  double bv = b * v;
  double bw = b * w;
  double cu = c * u;
  double cv = c * v;
  double cw = c * w;

  transform.values[0] = u2 + (cos_angle * (v2 + w2));
  transform.values[1] = (u * v * (1 - cos_angle)) + (w * sin_angle);
  transform.values[2] = (u * w * (1 - cos_angle)) + (v * sin_angle);
  transform.values[3] = 0;
  
  transform.values[4] = (u * v * (1 - cos_angle)) - (w * sin_angle);
  transform.values[5] = v2 + ((u2 + w2) * cos_angle);
  transform.values[6] = (v * w * (1 - cos_angle)) + (u * sin_angle);
  transform.values[7] = 0;
  
  transform.values[8] = (u * w * (1 - cos_angle)) + (v * sin_angle);
  transform.values[9] = (v * w * (1 - cos_angle)) - (u * sin_angle);
  transform.values[10] = w2 + ((u2 + v2) * cos_angle);
  transform.values[11] = 0;

  transform.values[12] = (( (a * (v2 + w2)) - (u * (bv + cw)) ) * (1 - cos_angle)) + ((bw - cv) * sin_angle);
  transform.values[13] = (( (b * (u2 + w2)) - (v * (au + cw)) ) * (1 - cos_angle)) + ((cu - aw) * sin_angle);
  transform.values[14] = (( (c * (u2 + v2)) - (w * (au + bv)) ) * (1 - cos_angle)) + ((av - bu) * sin_angle);
  transform.values[15] = 1;
  return Transformation(transform);
}

} /* namespace CW */
