//
//  Geometry.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 31/08/2016.
//  Copyright © 2016 SketchUp. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include <cassert>


#include <SketchUpAPI/model/vertex.h>

#include "Geometry.hpp"

//#include "Plane.h"
//#include "Line.h"
//#include "float3.h"
namespace CW {

/**
* Radians
*/
Radians::Radians(const double &rhs):
	m_val(rhs)
{ 
    if(rhs > (2*PI)) {
      int numPi = rhs / (2*PI); // Get number of times 2pi fits in to rhs
      m_val = rhs - (numPi*2*PI);
    }
    else if(rhs < 0.0) {
      int numPi = (rhs / (2*PI)) - 1; // Get number of times 2pi fits in to rhs
      m_val = rhs - (numPi*2*PI);
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
	return m_val * divider;
}

// Comparator TODO
bool Radians::closest(const Radians value) {
  return Radians(m_val - value);
}

/********
* Vector3D
*********/
Vector3D::Vector3D():
	m_vector(SUVector3D{ 0.0, 0.0, 0.0}),
  x(m_vector.x),
  y(m_vector.y),
  z(m_vector.z)
{}

Vector3D::Vector3D( SUVector3D su_vector):
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

Vector3D::Vector3D(const SUEdgeRef &su_edge):
	Vector3D(vector_from_edge(su_edge))
{}

Vector3D::Vector3D(const Vector3D &vector):
  m_vector(vector.m_vector),
  x(m_vector.x),
  y(m_vector.y),
  z(m_vector.z)
{}

Vector3D& Vector3D::operator=(const Vector3D &vector) {
  if (this == &vector)
    return *this;
  
  x = vector.x;
  y = vector.y;
  z = vector.z;
  return *this;
}

Vector3D& Vector3D::operator=(const SUVector3D &vector) {
  x = vector.x;
  y = vector.y;
  z = vector.z;
  return *this;
}

// Casting
Vector3D::operator SUVector3D() { return m_vector; }

Vector3D::operator Point3D() { return Point3D(m_vector); }

// Operator overloads
Vector3D Vector3D::operator+(const Vector3D &vector) {
  return Vector3D(m_vector.x + vector.x, m_vector.y + vector.y, m_vector.z + vector.z);
}
Vector3D Vector3D::operator-(const Vector3D &vector) {
  return Vector3D(m_vector.x - vector.x, m_vector.y - vector.y, m_vector.z - vector.z);
}
Vector3D Vector3D::operator*(const double &scalar) const {
	return Vector3D( x * scalar, y * scalar, z * scalar);
}
Vector3D Vector3D::operator/(const double &scalar) const {
	return Vector3D( x / scalar, y / scalar, z / scalar);
}

bool Vector3D::operator==(const Vector3D &vector) const {
	if (x == vector.x && y == vector.y && z == vector.z) {
  	return true;
  }
  else {
  	return false;
  }
}

bool Vector3D::operator!=(const Vector3D &vector) const {
	if (x != vector.x || y != vector.y || z != vector.z) {
  	return true;
  }
  else {
  	return false;
  }
}

double Vector3D::length() const {
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

Vector3D Vector3D::unit() const {
	return *this / length();
}

double Vector3D::angle(const Vector3D vector_b) const {
  return acos(unit().dot(vector_b.unit()));
}

double Vector3D::dot(const Vector3D vector2) const {
	return (x * vector2.x) + (y * vector2.y) + (z * vector2.z);
}

Vector3D Vector3D::cross(const Vector3D vector2) const {
  return Vector3D{y * vector2.z - z * vector2.y,
	              z * vector2.x - x * vector2.z,
	              x * vector2.y - y * vector2.x};
}

// Static method
SUVector3D Vector3D::vector_from_edge(const SUEdgeRef &su_edge) {
	SUVertexRef vertex_a, vertex_b;
  SU_RESULT res = SUEdgeGetStartVertex(su_edge, &vertex_a);
  assert(res == SU_ERROR_NONE);
  res = SUEdgeGetEndVertex(su_edge, &vertex_b);
  assert(res == SU_ERROR_NONE);
  
  SUPoint3D point_a, point_b;
  res = SUVertexGetPosition(vertex_a, &point_a);
  assert(res == SU_ERROR_NONE);
  res = SUVertexGetPosition(vertex_b, &point_b);
  assert(res == SU_ERROR_NONE);
  
  
  Vector3D vector_b = Point3D(point_b) - Point3D(point_a);
  
  return vector_b;
}


/********
* Point3D
*********/
Point3D::Point3D( SUPoint3D su_point):
	Vector3D(su_point.x, su_point.y, su_point.z)
{}

Point3D::Point3D( SUVector3D su_vector):
	Vector3D(su_vector)
{}

Point3D& Point3D::operator=(const Point3D &point) {
  if (this == &point)
    return *this;
  
  x = point.x;
  y = point.y;
  z = point.z;
  return *this;
}

/********
* Plane3D
*********/
Plane3D::Plane3D(SUPlane3D plane):
	m_plane(plane),
  a(m_plane.a),
  b(m_plane.b),
  c(m_plane.c),
  d(m_plane.d)
{}

Plane3D::Plane3D(double a, double b, double c, double d):
	Plane3D(SUPlane3D{a,b,c,d})
{}

Plane3D::Plane3D(const SUFaceRef &face):
	Plane3D(get_plane(face))
{}

Plane3D::Plane3D(const Plane3D &plane):
  m_plane(plane.m_plane),
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
  return *this;
}

Vector3D Plane3D::intersection(const Plane3D plane2) const {
  // MathGeoLib's 'vec' type is vector of floats
  Plane lib_plane1 (vec(a, b, c), d); // Note: Type conversion from double to float
  Plane lib_plane2 (vec(plane2.a, plane2.b, plane2.c), plane2.d); // Note: Type conversion from double to float
  Line line;
  bool result = lib_plane1.Intersects(lib_plane2, &line);
  assert(result== true);
  return Vector3D{line.dir.x, line.dir.y, line.dir.z}; // Note: Type conversion from float to double
}

Vector3D Plane3D::normal() const {
	return Vector3D{a,b,c};
}

Plane3D Plane3D::offset(double offset_by){
	return Plane3D(a, b, c, (d + offset_by));
}


bool Plane3D::parallel(const Plane3D plane2) const {
  // MathGeoLib's 'vec' type is vector of floats
  Plane lib_plane1 (vec(a, b, c), d); // Note: Type conversion from double to float
  Plane lib_plane2 (vec(plane2.a, plane2.b, plane2.c), plane2.d); // Note: Type conversion from double to float
	
  return lib_plane1.IsParallel(lib_plane2);
}

Plane3D Plane3D::reverse(){
  return Plane3D(a*-1, b*-1, c*-1, d*-1);
}

double Plane3D::angle_with(const Plane3D plane2) const {
	return normal().dot(plane2.normal());
}

/**
* Static Function
*/
SUPlane3D Plane3D::get_plane(const SUFaceRef &face) {
	SUPlane3D plane;
	SU_RESULT res = SUFaceGetPlane(face, &plane);
  assert(res == SU_ERROR_NONE);
  return plane;
}


Plane3D::operator SUPlane3D() { return m_plane; }

} /* namespace CW */
