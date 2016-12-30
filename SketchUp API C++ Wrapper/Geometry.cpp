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

Vector3D::Vector3D(bool invalid):
  null(true),
	m_vector(SUVector3D{0.0,0.0,0.0}),
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


Vector3D::Vector3D( const Point3D point):
	Vector3D(SUVector3D{point.x, point.y, point.z})
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
Point3D::Point3D():
	m_point(),
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


Point3D::Point3D(bool invalid):
  null(true),
  m_point(SUPoint3D{0.0,0.0,0.0}),
  x(m_point.x),
  y(m_point.y),
  z(m_point.z)
{}


Point3D& Point3D::operator=(const Point3D &point) {
  if (this == &point)
    return *this;
  
  x = point.x;
  y = point.y;
  z = point.z;
  return *this;
}


Point3D::operator SUPoint3D() { return SUPoint3D {m_point.x, m_point.y, m_point.z}; }

Point3D::operator SUPoint3D*() {
  m_point = SUPoint3D{m_point.x, m_point.y, m_point.z};
  return &m_point;
}

Point3D::operator Vector3D() { return Vector3D(m_point.x, m_point.y, m_point.z); }

// Operator overloads
Point3D Point3D::operator+(const Point3D &point) const {
  return Point3D(m_point.x + point.x, m_point.y + point.y, m_point.z + point.z);
}
Point3D Point3D::operator+(const Vector3D &vector) const { return *this + vector;}
Point3D Point3D::operator+(const SUPoint3D &point) const { return *this + Point3D(point);}

Point3D Point3D::operator-(const Point3D &point) const {
  return Point3D(m_point.x - point.x, m_point.y - point.y, m_point.z - point.z);
}
Point3D Point3D::operator-(const Vector3D &vector) const { return *this - vector;}
Point3D Point3D::operator-(const SUPoint3D &point) const  { return *this - Point3D(point);}

Point3D Point3D::operator*(const double &scalar) const {
  return Point3D(m_point.x * scalar, m_point.y * scalar, m_point.z * scalar);
}

Point3D Point3D::operator/(const double &scalar) const {
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


Plane3D::Plane3D(const Vector3D normal, const Point3D point):
	Plane3D(SUPlane3D{normal.unit().x, normal.unit().y, normal.unit().z, normal.dot(point)})
{}
Plane3D::Plane3D(const Point3D point, const Vector3D normal):
  Plane3D(normal, point)
{}


Plane3D::Plane3D(bool invalid):
	null(true),
  m_plane(SUPlane3D{0.0,0.0,0.0,0.0}),
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

bool Plane3D::operator!() const {
	if (null) {
  	return true;
  }
  return false;
}


Line3D Plane3D::intersection(const Plane3D plane2) const {
  // TODO: check for parallel planes?
  const Vector3D line_vector = this->normal().cross(plane2.normal());
  const double determinant = pow(line_vector.length(), 2);
  
  if (determinant == 0.0) {
    return Line3D(false);
  }
  Point3D line_point = ((line_vector.cross(plane2.normal()) * this->d) +
             (this->normal().cross(line_vector) * plane2.d)) / determinant;
  return Line3D(line_point, line_vector);
}

Vector3D Plane3D::normal() const {
	return Vector3D{a,b,c};
}

double Plane3D::distance(const Point3D point) const {
	return normal().dot(point) - d;
}

Plane3D Plane3D::offset(double offset_by){
	return Plane3D(a, b, c, (d + offset_by));
}


bool Plane3D::parallel(const Plane3D plane2) const {
  return this->normal() == plane2.normal();
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


/**
* BoundingBox3D
*/

BoundingBox3D::BoundingBox3D():
	BoundingBox3D(SUBoundingBox3D{Point3D(), Point3D()})
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


Point3D BoundingBox3D::min() {
	return Point3D(m_bounding_box.min_point);
}

Point3D BoundingBox3D::max() {
	return Point3D(m_bounding_box.max_point);
}


/**
* Line3D
*/

Line3D::Line3D():
	Line3D(Point3D(), Vector3D())
{}

Line3D::Line3D(const Point3D point, const Vector3D direction):
	m_point(point),
  m_direction(direction.unit()),
  point(m_point),
  direction(m_direction)
{}

Line3D::Line3D(bool invalid):
	m_point(Point3D(true)),
  m_direction(Vector3D(true)),
  point(m_point),
  direction(m_direction),
  null(true)
{}


Line3D& Line3D::operator=(const Line3D &line) {
  if (this == &line)
    return *this;
  
  m_point = line.point;
  m_direction = line.direction;
  return *this;
}

bool Line3D::operator!() const {
	if (null) {
  	return true;
  }
  return false;
}

Point3D Line3D::intersection(const Line3D &line) {
	// TODO
}

Point3D Line3D::intersection(const Plane3D &plane) {
	// TODO
}

/**
* Returns true if the Line or vector given is parallel to this line.
*/
bool Line3D::parallel(const Line3D &line) {
	if (line.direction == direction) {
  	return true;
  }
	else if (line.direction == (direction * -1)) {
  	return true;
  }
  return false;
}
bool Line3D::parallel(const Vector3D &vector) {
	// TODO
}



} /* namespace CW */
