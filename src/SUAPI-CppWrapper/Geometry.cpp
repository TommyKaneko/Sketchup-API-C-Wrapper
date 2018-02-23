//
//  Geometry.cpp
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

#include <stdio.h>
#include <cmath>
#include <stdexcept>

#include <SketchUpAPI/model/vertex.h>

#include "SUAPI-CppWrapper/Geometry.hpp"

#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"

//#include "SUAPI-CppWrapper/Plane.h"
//#include "SUAPI-CppWrapper/Line.h"
//#include "SUAPI-CppWrapper/float3.h"
namespace CW {

/**
* Radians
*/
Radians::Radians(const double &rhs):
  m_val(rhs)
{
  if(rhs > PI2) {
    m_val = std::fmod(rhs, PI2);
  }
  else if(rhs < 0.0) {
    m_val = PI2 - std::fmod(std::fabs(rhs), PI2);
  }
}

// Copy constructor
Radians::Radians(const Radians &radians):
  m_val(radians.m_val)
{}

// Cast to double
Radians::operator double() const
{
  return m_val;
}

Radians& Radians::operator=(const Radians &radians) {
  if (this == &radians)
    return *this;
  
  m_val = radians.m_val;
  return *this;
}

// Add
Radians Radians::operator+(const double value) const {
  return Radians(m_val + value);
}

// Subtract
Radians Radians::operator-(const double value) const {
  return Radians(m_val - value);
}

// Multiply
Radians Radians::operator*(const double multiplier) const {
  return m_val * multiplier;
}

// Divide
Radians Radians::operator/(const double divider) const {
  if (std::abs(divider) < EPSILON) {
    throw std::invalid_argument("CW::Radians::operator/() cannot divide by zero");
  }
  return m_val * divider;
}

bool Radians::operator==(const Radians& rhs) const {
  return (fabs(static_cast<double>(*this) - static_cast<double>(rhs))) < EPSILON;
}

bool Radians::operator==(const double rhs) const {
  return (fabs(m_val - rhs)) < EPSILON;
}

double Radians::difference(const Radians& other) const {
  double diff = std::abs(m_val - other.m_val);
  if (diff > PI) {
    return (2*PI)-diff;
  }
  return diff;
}

// Comparator TODO
bool Radians::closest(const Radians& value) {
  return Radians(m_val - value);
}

/********
* Vector3D
*********/

constexpr double Vector3D::EPSILON;

Vector3D::Vector3D():
  Vector3D(false)
{}

Vector3D::Vector3D(SUVector3D su_vector):
  m_vector(su_vector),
  x(m_vector.x),
  y(m_vector.y),
  z(m_vector.z)
{}

Vector3D::Vector3D( double x, double y, double z):
  m_vector(SUVector3D{x,y,z}),
  x(m_vector.x),
  y(m_vector.y),
  z(m_vector.z)
{}

Vector3D::Vector3D(bool valid):
  null(!valid),
  m_vector(SUVector3D{0.0,0.0,0.0}),
  x(m_vector.x),
  y(m_vector.y),
  z(m_vector.z)
{}


Vector3D::Vector3D(const Edge &edge):
  Vector3D(edge.vector())
{}

Vector3D::Vector3D(const Vector3D &vector):
  m_vector(vector.m_vector),
  x(m_vector.x),
  y(m_vector.y),
  z(m_vector.z),
  null(vector.null)
{}


Vector3D::Vector3D(const Point3D& point):
  Vector3D(SUVector3D{point.x, point.y, point.z})
{
  null = !point;
}


Vector3D& Vector3D::operator=(const Vector3D &vector) {
  if (this == &vector) {
    return *this;
  }
  x = vector.x;
  y = vector.y;
  z = vector.z;
  null = vector.null;
  return *this;
}

Vector3D& Vector3D::operator=(const SUVector3D &vector) {
  x = vector.x;
  y = vector.y;
  z = vector.z;
  null = false;
  return *this;
}

// Casting
Vector3D::operator SUVector3D() const { return m_vector; }

Vector3D::operator const SUVector3D*() const { return &m_vector; }

Vector3D::operator Point3D() const { return Point3D(m_vector); }

// Operator overloads
Vector3D Vector3D::operator+(const Vector3D &vector) const {
  return Vector3D(m_vector.x + vector.x, m_vector.y + vector.y, m_vector.z + vector.z);
}

Point3D operator+(const Vector3D &lhs, const Point3D& rhs) {
  return rhs + lhs;
}

Vector3D Vector3D::operator-() const {
  return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator-(const Vector3D &vector) const {
  return Vector3D(m_vector.x - vector.x, m_vector.y - vector.y, m_vector.z - vector.z);
}
Vector3D Vector3D::operator*(const double &scalar) const {
  return Vector3D( x * scalar, y * scalar, z * scalar);
}
Vector3D Vector3D::operator/(const double &scalar) const {
  if (std::abs(scalar) < EPSILON) {
    throw std::invalid_argument("CW::Vector3D::operator/() - cannot divide by zero");
  }
  return Vector3D( x / scalar, y / scalar, z / scalar);
}

bool operator==(const Vector3D &lhs, const Vector3D &rhs) {
  if (!lhs && !rhs) {
    return true;
  }
  if (!!lhs && !!rhs &&
      std::abs(lhs.x - rhs.x) < Vector3D::EPSILON &&
      std::abs(lhs.y - rhs.y) < Vector3D::EPSILON &&
      std::abs(lhs.z - rhs.z) < Vector3D::EPSILON) {
    return true;
  }
  return false;
}


bool operator!=(const Vector3D &lhs, const Vector3D &rhs) {
  if (lhs == rhs) {
    return false;
  }
  return true;
}


bool Vector3D::operator!() const {
  if (this->null) {
    return true;
  }
  return false;
}


double Vector3D::length() const {
  return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

Vector3D Vector3D::unit() const {
  return *this / length();
}

double Vector3D::angle(const Vector3D& vector_b) const {
  return acos(unit().dot(vector_b.unit()));
}

double Vector3D::dot(const Vector3D& vector2) const {
  return (x * vector2.x) + (y * vector2.y) + (z * vector2.z);
}

double Vector3D::dot(const Point3D& point) const {
  return (x * point.x) + (y * point.y) + (z * point.z);
}


Vector3D Vector3D::cross(const Vector3D& vector2) const {
  return Vector3D{y * vector2.z - z * vector2.y,
                z * vector2.x - x * vector2.z,
                x * vector2.y - y * vector2.x};
}

Vector3D::Colinearity Vector3D::colinear(const Vector3D& vector_b) const {
  if (this->length() < EPSILON || vector_b.length() < EPSILON) {
    return Vector3D::Colinearity::UNDEFINED;
  }
  Vector3D combined = (*this) + vector_b;
  double combined_length = combined.length();
  double added_length = (*this).length() + vector_b.length();
  if (std::fabs(added_length - combined_length) < EPSILON) {
    return Vector3D::Colinearity::COLINEAR_PRO;
  }
  double subtracted_length = (*this).length() - vector_b.length();
  if (std::fabs(subtracted_length - combined_length) < EPSILON) {
    return Vector3D::Colinearity::COLINEAR_ANTI;
  }
  return Vector3D::Colinearity::NO;
}


Vector3D Vector3D::rotate_about(double angle, const Vector3D& axis) const {
  // This solution is derived from this page: http://math.stackexchange.com/questions/511370/how-to-rotate-one-vector-about-another
  double b_dot_b = axis.dot(axis);
  Vector3D a_component_b_dir = ((*this).dot(axis) * b_dot_b) * axis;
  Vector3D a_component_b_orth = (*this) - (((*this).dot(axis) / b_dot_b) * axis);
  Vector3D w = axis.cross(a_component_b_orth);
  double x1 = cos(angle) / a_component_b_orth.length();
  double x2 = sin(angle) / w.length();
  Vector3D a_component_b_orth_rot = a_component_b_orth.length() * ((x1 * a_component_b_orth) + (x2 * w));
  return a_component_b_orth_rot + a_component_b_dir;
}

Vector3D Vector3D::zero_vector() {
  return Vector3D(0.0, 0.0, 0.0);
}


/********
* Point3D
*********/
Point3D::Point3D():
  Point3D(false)
{}

Point3D::Point3D(bool valid):
  null(!valid),
  m_point(SUPoint3D{0.0,0.0,0.0}),
  x(m_point.x),
  y(m_point.y),
  z(m_point.z)
{}

Point3D::Point3D( SUPoint3D su_point):
  m_point(su_point),
  x(m_point.x),
  y(m_point.y),
  z(m_point.z)
{}

Point3D::Point3D( SUVector3D su_vector):
  Point3D(SUPoint3D{su_vector.x, su_vector.y, su_vector.z})
{}

Point3D::Point3D(double x, double y, double z):
  Point3D(SUPoint3D{x, y, z})
{}

Point3D::Point3D(const Point3D& other):
  m_point(other.m_point),
  x(m_point.x),
  y(m_point.y),
  z(m_point.z),
  null(other.null)
{}


Point3D::Point3D( const Vector3D& vector):
  Point3D(SUPoint3D{vector.x, vector.y, vector.z})
{}

Point3D& Point3D::operator=(const Point3D &point) {
  if (this == &point) {
    return *this;
  }
  x = point.x;
  y = point.y;
  z = point.z;
  null = point.null;
  return *this;
}


Point3D::operator SUPoint3D() const { return SUPoint3D {m_point.x, m_point.y, m_point.z}; }


Point3D::operator const SUPoint3D*() const{
  //m_point = SUPoint3D{m_point.x, m_point.y, m_point.z};
  return &m_point;
}

Point3D::operator Vector3D() const { return Vector3D(m_point.x, m_point.y, m_point.z); }

// Operator overloads
Point3D Point3D::operator+(const Point3D &point) const {
  return Point3D(m_point.x + point.x, m_point.y + point.y, m_point.z + point.z);
}
Point3D Point3D::operator+(const Vector3D &vector) const {
  return Point3D(m_point.x + vector.x, m_point.y + vector.y, m_point.z + vector.z);
}
Point3D Point3D::operator+(const SUPoint3D &point) const { return (*this) + Point3D(point);}

Vector3D Point3D::operator-(const Point3D &point) const {
  return Vector3D(m_point.x - point.x, m_point.y - point.y, m_point.z - point.z);
}
Point3D Point3D::operator-(const Vector3D &vector) const { return (*this) - static_cast<Point3D>(vector);}
Point3D Point3D::operator-(const SUPoint3D &point) const  { return (*this) - Point3D(point);}

Point3D Point3D::operator*(const double &scalar) const {
  return Point3D(m_point.x * scalar, m_point.y * scalar, m_point.z * scalar);
}

Point3D Point3D::operator/(const double &scalar) const {
  if (std::abs(scalar) < EPSILON) {
    throw std::invalid_argument("Point3D::operator/: cannot divide by zero");
  }
  return Point3D(m_point.x / scalar, m_point.y / scalar, m_point.z / scalar);
}

/**
* Comparative operators
*/
bool Point3D::operator!() const {
  if (null) {
    return true;
  }
  return false;
}

bool operator==(const Point3D &lhs, const Point3D &rhs) {
  if (!lhs && !rhs) {
    return true;
  }
  if (!!lhs && !!rhs &&
      std::abs(lhs.x - rhs.x) < Point3D::EPSILON &&
      std::abs(lhs.y - rhs.y) < Point3D::EPSILON &&
      std::abs(lhs.z - rhs.z) < Point3D::EPSILON) {
    return true;
  }
  return false;
}


bool operator!=(const Point3D &lhs, const Point3D &rhs) {
  if (lhs == rhs) {
    return false;
  }
  return true;
}


/**
* Static method
*/
Point3D Point3D::intersection_between_lines(const Point3D& point_a, const Vector3D& vector_a, const Point3D& point_b, const Vector3D& vector_b, bool return_colinear) {
  if (vector_a.length() < EPSILON || vector_b.length() < EPSILON) {
    throw std::invalid_argument("CW::Point3D::intersection_between_lines() - cannot find intersection of lines with zero length");
  }
  // Find the closest points according to this solution: http://paulbourke.net/geometry/pointlineplane/
  // And this: http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
  Vector3D a_to_b = point_b - point_a;
  Vector3D vec_a_cross_b = vector_a.unit().cross(vector_b.unit());
  Vector3D a_to_b_cross_vec_a = a_to_b.unit().cross(vector_a.unit());
  Vector3D zero_vector(0.0,0.0,0.0);
  // Check for collinearity
  if (vec_a_cross_b == zero_vector) {
    if (a_to_b_cross_vec_a == zero_vector) {
      // Lines are collinear, so there is no intersection
      if (!return_colinear) {
        return Point3D(false);
      }
      bool opposite_direction;
      if (vector_a.unit() == vector_b.unit())
        opposite_direction = false;
      else
        opposite_direction = true;
      double vec_a_factor_0 = a_to_b.dot(vector_a) / vector_a.dot(vector_a);
      double vec_a_factor_1 = vec_a_factor_0 + (vector_b.dot(vector_a) / vector_a.dot(vector_a));
      double vec_a_epsilon = Vector3D::EPSILON / vector_a.length(); // Note accuracy needs to be determined
      if ((!opposite_direction && vec_a_factor_0 < vec_a_epsilon && vec_a_factor_1 > -vec_a_epsilon) ||
          (opposite_direction && vec_a_factor_1 < vec_a_epsilon && vec_a_factor_0 > -vec_a_epsilon)) {
        // The intersection is at the start of line A
        return point_a;
      }
      if ((!opposite_direction && vec_a_factor_0 > -vec_a_epsilon && vec_a_factor_0 < (1 + vec_a_epsilon))) {
        // The intersection is at the start of line B
        return point_b;
      }
      if (opposite_direction && vec_a_factor_1 > -vec_a_epsilon && vec_a_factor_1 < (1 + vec_a_epsilon)) {
        // The intersection is at the end of line B
        return point_b + vector_b;
      }
      // Lines are disjoint
      return Point3D(false);
      
    }
    else {
      // Lines are parallel, and so there is no intersection.
      return Point3D(false);
    }
  }
  // We create lines and find the closest points between them.
  Line3D line_a(point_a, vector_a);
  Line3D line_b(point_b, vector_b);
  std::pair<Point3D, Point3D> closest_points = line_a.closest_points(line_b);
  // Check closest points are actually the same.
  if (closest_points.first != closest_points.second) {
    // No intersection
    return Point3D(false);
  }
  // Otherwise, we need to see if the intersection of the lines lie between the line segments
  Point3D intersection = closest_points.first;
  Vector3D a_to_int = intersection - point_a;
  Vector3D b_to_int = intersection - point_b;
  // Express the point of intersection as a scalar of the vectors from point A and point B
  double a_direction_factor = 0.0;
  if (a_to_int != zero_vector)
    a_direction_factor = vector_a.unit().dot(a_to_int.unit());
  double b_direction_factor = 0.0;
  if (b_to_int != zero_vector)
    b_direction_factor = vector_b.unit().dot(b_to_int.unit());
 
  double a_factor = a_direction_factor * a_to_int.length() / vector_a.length();
  double b_factor = b_direction_factor * b_to_int.length() / vector_b.length();
  double a_epsilon = Vector3D::EPSILON / vector_a.length();
  double b_epsilon = Vector3D::EPSILON / vector_b.length();
  if (a_factor > -a_epsilon && a_factor < 1.0 + a_epsilon &&
      b_factor > -b_epsilon && b_factor < 1.0 + b_epsilon) {
    return intersection;
  }
  return Point3D(false);
}


Point3D Point3D::ray_line_intersection(const Point3D& point_a, const Vector3D& vector_a, const Point3D& point_b, const Vector3D& ray_b, bool return_colinear) {
  // Solution to finding intersection between two line segments derived from this answer: http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
  Vector3D zero_vector(0.0,0.0,0.0);
  if (ray_b == zero_vector || vector_a == zero_vector) {
    throw std::invalid_argument("CW::Point3D::ray_line_intersection() - given ray/line have zero length");
  }
  Vector3D a_to_b = point_b - point_a;
  // Check if the start of the lines touch
  if (a_to_b == zero_vector) {
    return point_a;
  }
  Vector3D b_to_a = -a_to_b;
  Vector3D vec_a_cross_b_z_comp =  vector_a.unit().cross(ray_b.unit()); // Use unit vectors for easier comparisons with zero vector
  Vector3D a_to_b_cross_vec_a_z_comp = a_to_b.unit().cross(vector_a.unit()); // Use unit vectors for easier comparisons with zero vector
  // Check for collinearity
  if (vec_a_cross_b_z_comp == zero_vector) {
    if (a_to_b_cross_vec_a_z_comp == zero_vector) {
      // Lines are collinear, so there is no intersection
      if (!return_colinear) {
        return Point3D(false);
      }
      double b_to_a_factor = b_to_a.dot(ray_b) / ray_b.dot(ray_b);
      double b_to_a_end_factor = b_to_a_factor + (vector_a.dot(ray_b) / ray_b.dot(ray_b));
      if ((b_to_a_factor < 0.0 && b_to_a_end_factor > 0.0) || (b_to_a_factor > 0.0 && b_to_a_end_factor < 0.0)) {
        // The start of ray B is on line A
        return point_b;
      }
      if (b_to_a_factor > 0.0) {
        if (b_to_a_factor < b_to_a_end_factor) {
          // The intersection is at the start of line A
          return point_a;
        }
        else {
          // The intersection is at the end of line B
          return point_a + vector_a;
        }
      }
      // Lines are disjoint
      return Point3D(false);
      
    }
    else {
      // Lines are parallel, and so there is no intersection.
      return Point3D(false);
    }
  }
    // We create lines and find the closest points between them.
  Line3D line_a(point_a, vector_a);
  Line3D line_b(point_b, ray_b);
  std::pair<Point3D, Point3D> closest_points = line_a.closest_points(line_b);
  // Check closest points are actually the same.
  if (closest_points.first != closest_points.second) {
    // No intersection
    return Point3D(false);
  }
  // Otherwise, we need to see if the intersection of the lines lie between the line segments
  Point3D intersection = closest_points.first;
  Vector3D a_to_int = intersection - point_a;
  Vector3D b_to_int = intersection - point_b;
  // Express the point of intersection as a scalar of the vectors from point A and point B
  double a_direction_factor = 0.0;
  if (a_to_int != zero_vector)
    a_direction_factor = vector_a.unit().dot(a_to_int.unit());
  double b_direction_factor = 0.0;
  if (b_to_int != zero_vector)
    b_direction_factor = ray_b.unit().dot(b_to_int.unit());
 
  double a_factor = a_direction_factor * a_to_int.length() / vector_a.length();
  double b_factor = b_direction_factor * b_to_int.length() / ray_b.length();
  double a_epsilon = Vector3D::EPSILON / vector_a.length();
  double b_epsilon = Vector3D::EPSILON / ray_b.length();
  if (a_factor > -a_epsilon && a_factor < 1.0 + a_epsilon &&
      b_factor > -b_epsilon) {
    return intersection;
  }
  return Point3D(false);
  
  /*
  else if (a_to_b_cross_vec_a_z_comp != zero_vector) {
    Vector3D vec_a_cross_b = vector_a.cross(ray_b);
    double line_a_factor = a_to_b.cross(ray_b).length() / vec_a_cross_b.length();
    double a_epsilon = Vector3D::EPSILON / vector_a.length(); // Note accuracy needs to be determined
    double line_b_factor = b_to_a.cross(vector_a).length() / vec_a_cross_b.length();
    double b_epsilon = Vector3D::EPSILON / ray_b.length();
    if (-a_epsilon <= line_a_factor && line_a_factor <= (1.0 + a_epsilon) &&
        -b_epsilon <= line_b_factor) {
      Point3D intersection = point_a + (vector_a * line_a_factor);
      return intersection;
    }
  }
  // The lines do not intersect
  return Point3D(false);
  */
}


  
/********
* Plane3D
*********/
Plane3D::Plane3D():
  Plane3D(SUPlane3D{0.0,0.0,0.0,0.0})
{}

Plane3D::Plane3D(SUPlane3D plane):
  m_plane(plane),
  a(m_plane.a),
  b(m_plane.b),
  c(m_plane.c),
  d(m_plane.d),
  null((m_plane.a == 0.0 && m_plane.b == 0.0 && m_plane.c == 0.0))
{}


Plane3D::Plane3D(double a, double b, double c, double d):
  Plane3D(SUPlane3D{a,b,c,d})
{}


Plane3D::Plane3D(const Face &face):
  Plane3D(face.plane())
{}


Plane3D::Plane3D(const Plane3D &plane):
  null(plane.null),
  m_plane(plane.m_plane),
  a(m_plane.a),
  b(m_plane.b),
  c(m_plane.c),
  d(m_plane.d)
{}


Plane3D::Plane3D(const Vector3D& normal, const Point3D& point):
  Plane3D(SUPlane3D{normal.unit().x, normal.unit().y, normal.unit().z, -normal.unit().dot(point)})
{
}
Plane3D::Plane3D(const Point3D& point, const Vector3D& normal):
  Plane3D(normal, point)
{}


Plane3D::Plane3D(bool valid):
  null(!valid),
  m_plane(SUPlane3D{1.0,0.0,0.0,0.0}),
  a(m_plane.a),
  b(m_plane.b),
  c(m_plane.c),
  d(m_plane.d)
{}



Plane3D& Plane3D::operator=(const Plane3D &plane) {
  if (this == &plane)
    return *this;
  a = plane.a;
  b = plane.b;
  c = plane.c;
  d = plane.d;
  null = plane.null;
  return *this;
}

bool Plane3D::operator!() const {
  if (null) {
    return true;
  }
  return false;
}

/**
* Comparative operators
*/
bool operator==(const Plane3D &lhs, const Plane3D &rhs) {
  if (!lhs && !rhs) {
    return true;
  }
  if (!!lhs && !!rhs &&
      std::abs(lhs.a - rhs.a) < Plane3D::EPSILON &&
      std::abs(lhs.b - rhs.b) < Plane3D::EPSILON &&
      std::abs(lhs.c - rhs.c) < Plane3D::EPSILON &&
      std::abs(lhs.d - rhs.d) < Plane3D::EPSILON) {
    return true;
  }
  return false;
}

bool operator!=(const Plane3D &lhs, const Plane3D &rhs) {
  if (lhs == rhs) {
    return false;
  }
  return true;
}


bool Plane3D::coplanar(const Plane3D& test_plane) const {
  if (this->parallel(test_plane)) {
    if ((this->normal() * this->d) == (test_plane.normal() * test_plane.d)) {
      return true;
    }
  }
  return false;
}


Line3D Plane3D::intersection(const Plane3D& plane2) const {
  const Vector3D line_vector = (*this).normal().cross(plane2.normal());
  const double determinant = pow(line_vector.length(), 2);
  
  if (determinant < EPSILON) {
    // Parallel planes
    return Line3D(false);
  }
  Point3D line_point = ((line_vector.cross(plane2.normal()) * (*this).d) +
             ((*this).normal().cross(line_vector) * plane2.d)) / determinant;
  return Line3D(line_point, line_vector);
}

Point3D Plane3D::intersection(const Line3D &line) const {
  return line.intersection(*this);
}


Point3D Plane3D::intersection(const Point3D& start_point, const Vector3D& direction) const {
  // First check that an intersection exists, by checking if the ray is pointing towards the plane
  Vector3D unit_direction = direction.unit();
  double combined_length = Vector3D(this->normal() + unit_direction).length();
  double distance_from_plane = this->distance(start_point);
  if (combined_length > 1.0) {
    if (distance_from_plane > EPSILON) {
      return Point3D(false);
    }
    else {
      return this->intersection(Line3D(start_point, unit_direction));
    }
  }
  else {
    if (distance_from_plane < -EPSILON) {
      return Point3D(false);
    }
    else {
      return this->intersection(Line3D(start_point, unit_direction));
    }
  }
}


Point3D Plane3D::intersection_between(const Point3D& point_a, const Point3D& point_b) const {
  Line3D temp_line(point_a, Vector3D(point_b-point_a));
  Point3D intersection = this->intersection(temp_line);
  if (!!intersection && Line3D::on_line_segment(point_a, point_b, intersection)) {
    return intersection;
  }
  return Point3D(false);
}


Vector3D Plane3D::normal() const {
  return Vector3D{a,b,c};
}

double Plane3D::distance(const Point3D& point) const {
  return normal().dot(point) + d;
}

bool Plane3D::on_plane(const Point3D& point) const {
  double distance = this->distance(point);
  if (fabs(distance) < EPSILON) {
    return true;
  }
  return false;
}


Plane3D Plane3D::offset(double offset_by) const {
  return Plane3D(a, b, c, (d - offset_by));
}


bool Plane3D::parallel(const Plane3D& plane2) const {
  if (!plane2) {
    throw std::invalid_argument("CW::Plane3D::parallel(): given plane is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Plane3D::parallel(): plane is null");
  }
  // The following assumes that the plane normals are unit vectors.
  if (this->normal() == plane2.normal() || this->normal() == -plane2.normal()) {
    return true;
  }
  return false;
}

Plane3D Plane3D::inverse() const {
  if (!(*this)) {
    throw std::logic_error("CW::Plane3D::inverse(): plane is null");
  }
  return Plane3D(a*-1, b*-1, c*-1, d*-1);
}

double Plane3D::angle_with(const Plane3D& plane2) const {
  if (!plane2) {
    throw std::invalid_argument("CW::Plane3D::angle_with(): given plane is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Plane3D::angle_with(): plane is null");
  }
  // Solution taken form this: https://www.youtube.com/watch?v=C6ElQdRbHaE
  return acos(std::abs(normal().dot(plane2.normal())));
}


Plane3D::operator SUPlane3D() const { return m_plane; }


Plane3D Plane3D::plane_from_loop(const std::vector<Point3D>& loop_points) {
  if (loop_points.size() < 3) {
    throw std::invalid_argument("CW::Plane3D::plane_from_loop(): not enough points given for a valid loop");
  }
  /*
  if(loop_points.size() < 3) {
    // This is an invalid plane
    return Plane3D();
  }
  // Find 3 points which we can use to create this plane.
  Point3D point_a = loop_points[0];
  Point3D point_b = loop_points[1];
  Line3D ab_line(point_a, Vector3D(point_b - point_a).unit());
  Point3D point_c; // We first need to find a point that is not colinear with the other two points.
  for (size_t i=2; i < loop_points.size(); i++) {
    if (!ab_line.on_line(loop_points[i])) {
      point_c = loop_points[i];
      break;
    }
  }
  if (!point_c) {
    return Plane3D();
  }
  // Create a plane from the 3 points;
  Vector3D normal = Vector3D(point_b-point_a).cross(Vector3D(point_c-point_b));
  Plane3D temp_plane(point_a, normal);
  // Check that all points given are on the plane.
  for (size_t i=2; i < loop_points.size(); i++) {
    if (!temp_plane.on_plane(loop_points[i])) {
      return Plane3D();
    }
  }
  return temp_plane;
  */
  // Solution for finding the planes here: http://www.euclideanspace.com/maths/algebra/vectors/applications/normals/
  // We will use two adjacent edges of the the loop to create a plane from.  The vertex that is furthest from the centre of the loop is where we will start
  
  // Or this solution: https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
  Vector3D normal(0.0, 0.0, 0.0);
  for (size_t i=0; i < loop_points.size(); i++) {
    Point3D next;
    Point3D current = loop_points[i];
    if (i == loop_points.size() - 1) {
      next = loop_points[0];
    }
    else {
      next = loop_points[i+1];
    }
    normal.x += (current.y - next.y) * (current.z + next.z);
    normal.y += (current.z - next.z) * (current.x + next.x);
    normal.z += (current.x - next.x) * (current.y + next.y);
  }
  if (normal == Vector3D::zero_vector()) {
    return Plane3D(false);
  }
  return Plane3D (loop_points[0], normal.unit());
}



/**
* BoundingBox3D
*/

BoundingBox3D::BoundingBox3D():
  BoundingBox3D(SUBoundingBox3D{Point3D(), Point3D()})
{}

BoundingBox3D::BoundingBox3D(bool valid):
  null(!valid),
  m_bounding_box(SUBoundingBox3D{Point3D(), Point3D()})
{}

BoundingBox3D::BoundingBox3D(SUBoundingBox3D bounding_box):
  m_bounding_box(bounding_box)
{}

bool BoundingBox3D::operator!() const {
  if (null) {
    return true;
  }
  return false;
}

BoundingBox3D::operator SUBoundingBox3D() const {
  return m_bounding_box;
}

Point3D BoundingBox3D::min() const {
  return Point3D(m_bounding_box.min_point);
}

void BoundingBox3D::min_point(const Point3D& point) {
  m_bounding_box.min_point = point;
}

Point3D BoundingBox3D::max() const {
  return Point3D(m_bounding_box.max_point);
}
  
void BoundingBox3D::max_point(const Point3D& point) {
  m_bounding_box.max_point = point;
}


/**
* Line3D
*/

Line3D::Line3D():
  Line3D(false)
{}


Line3D::Line3D(const Point3D point, const Vector3D direction):
  m_point(point),
  m_direction(direction.unit()),
  point(m_point),
  direction(m_direction)
{}


Line3D::Line3D(const Vector3D direction, const Point3D point):
  Line3D(point, direction)
{}


Line3D::Line3D(bool valid):
  m_point(Point3D(valid)),
  m_direction(Vector3D(valid)),
  point(m_point),
  direction(m_direction),
  null(!valid)
{}


Line3D::Line3D(const Line3D& other):
  m_point(other.m_point),
  m_direction(other.m_direction),
  point(m_point),
  direction(m_direction),
  null(other.null)
{}


Line3D& Line3D::operator=(const Line3D &line) {
  if (this == &line)
    return *this;
  
   point = line.point;
  direction = line.direction;
  null = line.null;
  return *this;
}


bool Line3D::operator!() const {
  if (null) {
    return true;
  }
  return false;
}


Point3D Line3D::intersection(const Line3D &other_line) const {
  if (!other_line) {
    throw std::invalid_argument("CW::Line3D::intersection(): given line is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Line3D::intersection(): this line is null");
  }
  // @see http://paulbourke.net/geometry/pointlineplane/
  std::pair<Point3D, Point3D> close_points = this->closest_points(other_line);
  if (!close_points.first) {
    return close_points.first;
  }
  Vector3D vector_between = close_points.first - close_points.second;
  double distance = vector_between.length();
  if (distance > EPSILON) {
    // The lines do not intersect
    return Point3D(false);
  }
  return close_points.first;
}


Point3D Line3D::intersection(const Plane3D &plane) const {
  if (!plane) {
    throw std::invalid_argument("CW::Line3D::intersection(): given plane is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Line3D::intersection(): this line is null");
  }
  // @see http://paulbourke.net/geometry/pointlineplane/
  double numerator = (plane.a * this->point.x) + (plane.b * this->point.y) + (plane.c * this->point.z) + plane.d;
  double denominator = -(plane.a * this->direction.x) - (plane.b * this->direction.y) - (plane.c * this->direction.z);
  if (std::abs(denominator) < EPSILON) {
    // The line is parallel to or on the plane
    return Point3D(false);
  }
  double u = numerator / denominator;
  Point3D point_of_intersection = this->point + (u * this->direction);
  return point_of_intersection;
}


std::pair<Point3D, Point3D> Line3D::closest_points(const Line3D &other_line) const {
  if (!other_line) {
    throw std::invalid_argument("CW::Line3D::closest_points(): given line is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Line3D::closest_points(): this line is null");
  }
  
  // @see http://paulbourke.net/geometry/pointlineplane/
  double d1343,d4321,d1321,d4343,d2121;
  double numer,denom;
  // p13 is a vector between two points on different lines
  Vector3D p13 = this->point - other_line.point;
  Vector3D p43 = other_line.direction;
  if (std::abs(p43.x) < EPSILON && std::abs(p43.y) < EPSILON && std::abs(p43.z) < EPSILON) {
    // The two lines are along each other
    return std::pair<Point3D,Point3D> (Point3D(false), Point3D(false));
  }
  Vector3D p21 = this->direction;
  //if (std::abs(p21.x) < EPSILON/2 && std::abs(p21.y) < EPSILON/2 && std::abs(p21.z) < EPSILON/2) {
    // The vector is zero - should not happen
  //  return std::pair<Point3D,Point3D> (Point3D(false), Point3D(false));
  //}
  d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
  d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
  d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
  d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
  d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;
  
  denom = d2121 * d4343 - d4321 * d4321;
  if (std::abs(denom) < EPSILON) {
    // Lines are parallel
    return std::pair<Point3D,Point3D> (Point3D(false), Point3D(false));
  }
  numer = d1343 * d4321 - d1321 * d4343;

  double mua = numer / denom;
  double mub = (d1343 + d4321 * mua) / d4343;
  Point3D pa = this->point + (this->direction * mua);
  Point3D pb = other_line.point + (other_line.direction * mub);
  return std::pair<Point3D,Point3D> (pa, pb);
}


Point3D Line3D::closest_point(const Point3D& point) const {
  if (!point) {
    throw std::invalid_argument("CW::Line3D::closest_point(): given point is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Line3D::closest_point(): this line is null");
  }
  // @see http://paulbourke.net/geometry/pointlineplane/
  // [P3 - P1 - u(P2 - P1)] dot (P2 - P1) = 0 - where U is the factor
  Vector3D start_to_point = point - m_point;
  double factor = start_to_point.dot(m_direction);
  return m_point + (factor * m_direction);
}


double Line3D::distance(const Point3D& point) const {
  Point3D closest_point = this->closest_point(point);
  return Vector3D(point - closest_point).length();
}


bool Line3D::on_line(const Point3D& test_point) const {
  if (!test_point) {
    throw std::invalid_argument("CW::Line3D::on_line(): given point is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Line3D::on_line(): this line is null");
  }
  double factor;
  enum class OnLineFactorCoords {
    X,
    Y,
    Z
  };
  OnLineFactorCoords factor_val; // This is to save extra calculations.
  // To get the most accurate factor, use one of the larger values among x, y OR z to calculate the factor.  The smallest possible value for all three x, y, z to have (given that direction is a unit vector) is 1 / sqrt(3) = 0.577
  if (this->direction.x > 0.56) {
    // Get the factor with which to multiply the x value, and see if it is the same for y and z values too.
    factor = (test_point.x - this->point.x) / this->direction.x;
    factor_val = OnLineFactorCoords::X;
  }
  else if (this->direction.y > 0.56) {
    factor = (test_point.y - this->point.y) / this->direction.y;
    factor_val = OnLineFactorCoords::Y;
  }
  else {
    factor = (test_point.z - this->point.z) / this->direction.z;
    factor_val = OnLineFactorCoords::Z;
  }
  if (factor_val != OnLineFactorCoords::X) {
    double x_test = this->point.x + (this->direction.x * factor);
    if (std::abs(x_test - test_point.x) > EPSILON) {
      return false;
    }
  }
  if (factor_val != OnLineFactorCoords::Y) {
  double y_test = this->point.y + (this->direction.y * factor);
    if (std::abs(y_test - test_point.y) > EPSILON) {
      return false;
    }
  }
  if (factor_val != OnLineFactorCoords::Z) {
    double z_test = this->point.z + (this->direction.z * factor);
    if (std::abs(z_test - test_point.z) > EPSILON) {
      return false;
    }
  }
  return true;
}


bool Line3D::on_line_segment(const Point3D& point_a, const Point3D& point_b, const Point3D& test_point) {
  if (!point_a || !test_point || !point_b) {
    throw std::invalid_argument("CW::Line3D::on_line_segment(): given point is null");
  }
  if (point_a == point_b) {
    throw std::invalid_argument("CW::Line3D::on_line_segment(): given points are equal");
  }
  // From: https://stackoverflow.com/questions/328107/how-can-you-determine-a-point-is-between-two-other-points-on-a-line-segment
  Vector3D a_to_b = point_b - point_a;
  Vector3D a_to_c = test_point - point_a;
  if (a_to_c == Vector3D::zero_vector()) {
    return true;
  }
  Vector3D crossproduct = a_to_b.unit().cross(a_to_c.unit());
  if (fabs(crossproduct.length()) > EPSILON) {
    // Not aligned
    return false;
  }
  // Check that the distance matches.
  double dotproduct = a_to_b.dot(a_to_c);
  if (dotproduct < EPSILON) {
    return false;
  }

  double squaredlengthba = pow(a_to_b.length()+EPSILON, 2);
  if (dotproduct > squaredlengthba) {
    return false;
  }
  return true;
}


/**
* Returns true if the Line or vector given is parallel to this line.
*/
bool Line3D::parallel(const Line3D &line) const {
  if (!line) {
    throw std::invalid_argument("CW::Line3D::parallel(): given line is null");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Line3D::parallel(): this line is null");
  }
  if (line.direction == direction) {
    return true;
  }
  else if (line.direction == (direction * -1)) {
    return true;
  }
  return false;
}


bool Line3D::parallel(const Vector3D &vector) const {
  if (!vector) {
    throw std::invalid_argument("CW::Line3D::parallel(): given vector is null");
  }
  if (vector.length() < EPSILON) {
    throw std::invalid_argument("CW::Line3D::parallel(): given vector has zero length");
  }
  if (!(*this)) {
    throw std::logic_error("CW::Line3D::parallel(): this line is null");
  }
  Vector3D difference = this->direction.unit() - vector.unit();
  double length = difference.length();
  if (length < EPSILON || (length - 2.0) < EPSILON) {
    return true;
  }
  return false;
}


bool operator==(const Line3D& lhs, const Line3D& rhs) {
  if (lhs.parallel(rhs)) {
    // lines are parallel - check if they overlap
    if (lhs.m_point == rhs.m_point) {
      return true;
    }
    Vector3D l_to_r = rhs.m_point - lhs.m_point;
    if (l_to_r.colinear(lhs.m_direction) != Vector3D::Colinearity::NO) {
      return true;
    }
  }
  return false;
}


} /* namespace CW */
