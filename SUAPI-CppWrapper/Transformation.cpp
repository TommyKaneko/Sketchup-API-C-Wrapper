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
#include <cmath>

#include "SUAPI-CppWrapper/Transformation.hpp"

#include <SketchUpAPI/geometry/vector3d.h>
#include <SketchUpAPI/geometry/point3d.h>
#include <SketchUpAPI/geometry/plane3d.h>

#include "SUAPI-CppWrapper/model/Axes.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/LoopInput.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"

namespace CW {

Transformation::Transformation():
  Transformation(1.0)
{}


Transformation::Transformation(SUTransformation transformation):
  m_transformation(transformation)
{}

Transformation::Transformation(const Axes& axes, const Vector3D& translation, double scalar):
  Transformation(Point3D(translation), axes.x_axis(), axes.y_axis(), axes.z_axis(), scalar)
{}

Transformation::  Transformation(const Point3D& origin, const Vector3D& x_axis, const Vector3D& y_axis, const Vector3D& z_axis, double scalar):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationSetFromPointAndAxes(&m_transformation, origin, x_axis, y_axis, z_axis);
  assert(res == SU_ERROR_NONE);
  if (scalar != 1.0) {
    // TODO:
    assert(false);
  }
}

Transformation::Transformation(double scalar):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationScale(&m_transformation, scalar);
  assert(res == SU_ERROR_NONE);
}


Transformation::Transformation(double x_scale, double y_scale, double z_scale):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationNonUniformScale(&m_transformation, x_scale, y_scale, z_scale);
  assert(res == SU_ERROR_NONE);
}


Transformation::Transformation(const Vector3D& translation):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationTranslation(&m_transformation, translation);
  assert(res == SU_ERROR_NONE);
}


Transformation::Transformation(const Point3D& translation, double scalar):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationScaleAboutPoint(&m_transformation, translation, scalar);
  assert(res == SU_ERROR_NONE);
}


Transformation::Transformation(const Point3D& translation, const Vector3D& normal):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationSetFromPointAndNormal(&m_transformation, translation, normal);
  assert(res == SU_ERROR_NONE);
}


Transformation::Transformation(const Point3D& point, const Vector3D& vector, double angle):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationRotation(&m_transformation, point, vector, angle);
  assert(res == SU_ERROR_NONE);
}


Transformation::Transformation(const Transformation& transform1, const Transformation& transform2, double weight):
  m_transformation(SU_INVALID)
{
  SUResult res = SUTransformationInterpolate(&m_transformation, transform1, transform2, weight);
  assert(res == SU_ERROR_NONE);
}


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


std::array<double, 4> Transformation::multiply4x1(std::array<double, 4> matrix4_1) const {
  std::array<double, 4> output;
  for (size_t i=0; i < 4; ++i) {
    // This one is for multiplying 1x4 by 4x4:
    //output[i] = matrix4_1[i] * (m_transformation.values[i] +
    //                            m_transformation.values[i+4] +
    //                            m_transformation.values[i+8] +
    //                            m_transformation.values[i+12]);
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

Transformation::operator const SUTransformation*() const {
  return &m_transformation;
}


bool Transformation::is_identity() const {
  bool is_identity;
  SUResult res = SUTransformationIsIdentity(&m_transformation, &is_identity);
  assert(res == SU_ERROR_NONE);
  return is_identity;
}


Transformation Transformation::inverse() const {
  SUTransformation inverse = SU_INVALID;
  SUResult res = SUTransformationGetInverse(&m_transformation, &inverse);
  assert(res == SU_ERROR_NONE);
  return Transformation(inverse);
}
  

Vector3D Transformation::x_axis() const {
  SUVector3D x_axis = SU_INVALID;
  SUResult res = SUTransformationGetXAxis(&m_transformation, &x_axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(x_axis);
}


Vector3D Transformation::y_axis() const {
  SUVector3D y_axis = SU_INVALID;
  SUResult res = SUTransformationGetYAxis(&m_transformation, &y_axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(y_axis);
}


Vector3D Transformation::z_axis() const {
  SUVector3D z_axis = SU_INVALID;
  SUResult res = SUTransformationGetZAxis(&m_transformation, &z_axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(z_axis);
}


double Transformation::z_rotation() const {
  double z_rotation;
  SUResult res = SUTransformationGetZRotation(&m_transformation, &z_rotation);
  assert(res == SU_ERROR_NONE);
  return z_rotation;
}


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


Point3D Transformation::origin() const {
  SUPoint3D origin = SU_INVALID;
  SUResult res = SUTransformationGetOrigin(&m_transformation, &origin);
  assert(res == SU_ERROR_NONE);
  return Point3D(origin);
}
  

Vector3D Transformation::translation() const {
  if (m_transformation.values[15] == 1.0) {
    return Vector3D( m_transformation.values[12], m_transformation.values[13], m_transformation.values[14]);
  }
  return Vector3D( m_transformation.values[12] / m_transformation.values[15], m_transformation.values[13] / m_transformation.values[15], m_transformation.values[14] / m_transformation.values[15]);
}

  
Transformation Transformation::operator*(Transformation transform) {
  SUTransformation out_transform = SU_INVALID;
  SUResult res = SUTransformationMultiply(&m_transformation, &transform.m_transformation, &out_transform);
  assert(res == SU_ERROR_NONE);
  return Transformation(out_transform);
}


/**
* Friend Functions of class Transformation
*/
Vector3D operator*(const Transformation &lhs, const Vector3D &rhs) {
  if (!rhs) {
    throw std::invalid_argument("CW::Transformation::operator*(const Vector3D &lhs, const Transformation &rhs): Vector3D given is null");
  }
  SUVector3D transformed = rhs;
  SUResult res = SUVector3DTransform(&lhs.m_transformation, &transformed);
  assert(res == SU_ERROR_NONE);
  return Vector3D(transformed);
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
  SUPoint3D transformed = rhs;
  SUResult res = SUPoint3DTransform(&lhs.m_transformation, &transformed);
  assert(res == SU_ERROR_NONE);
  return Vector3D(transformed);
}


Point3D operator*(const Point3D &lhs, const Transformation &rhs) {
  // Can't actually multiply a vector by transformation, so return the transforamtion multiplied by the vector
  return rhs * lhs;
}


/**
* Friend Functions of class Transformation
*/
Plane3D operator*(const Transformation &lhs, const Plane3D &rhs) {
  if (!rhs) {
    throw std::invalid_argument("CW::Transformation::operator*(const Transformation &lhs, const Plane3D &rhs): Plane3D given is null");
  }
  SUPlane3D transformed = rhs;
  SUResult res = SUPlane3DTransform(&lhs.m_transformation, &transformed);
  assert(res == SU_ERROR_NONE);
  return Plane3D(transformed);
}


Plane3D operator*(const Plane3D &lhs, const Transformation &rhs) {
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
  // TODO: create solution in terms of SU's native Transformation functions.
  
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
  transform.values[2] = (u * w * (1 - cos_angle)) - (v * sin_angle);
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
