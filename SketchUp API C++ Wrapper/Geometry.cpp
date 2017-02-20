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


bool Radians::operator==(const Radians rhs) const {
  return ((*this) - rhs) < EPSILON;
}

bool Radians::operator==(const double rhs) const {
  return (m_val - rhs) < EPSILON;
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
	Vector3D(get_vector(su_edge))
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
Vector3D Vector3D::operator+(const Vector3D &vector) const {
  return Vector3D(m_vector.x + vector.x, m_vector.y + vector.y, m_vector.z + vector.z);
}
Vector3D Vector3D::operator-(const Vector3D &vector) const {
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

double Vector3D::dot(const Point3D point) const {
	return (x * point.x) + (y * point.y) + (z * point.z);
}


Vector3D Vector3D::cross(const Vector3D vector2) const {
  return Vector3D{y * vector2.z - z * vector2.y,
	              z * vector2.x - x * vector2.z,
	              x * vector2.y - y * vector2.x};
}


Vector3D Vector3D::rotate_about(double angle, Vector3D axis) const {
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


// Static method
SUVector3D Vector3D::get_vector(const SUEdgeRef &su_edge) {
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


Point3D::operator SUPoint3D() const { return SUPoint3D {m_point.x, m_point.y, m_point.z}; }

Point3D::operator SUPoint3D*() {
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

Point3D Point3D::operator-(const Point3D &point) const {
  return Point3D(m_point.x - point.x, m_point.y - point.y, m_point.z - point.z);
}
Point3D Point3D::operator-(const Vector3D &vector) const { return (*this) - vector;}
Point3D Point3D::operator-(const SUPoint3D &point) const  { return (*this) - Point3D(point);}

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


bool Plane3D::coplanar(const Plane3D test_plane) const {
	if (this->parallel(test_plane)) {
  	if ((this->normal() * this->d) == (test_plane.normal() * test_plane.d)) {
    	return true;
    }
  }
  return false;
}


Line3D Plane3D::intersection(const Plane3D plane2) const {
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

Vector3D Plane3D::normal() const {
	return Vector3D{a,b,c};
}

double Plane3D::distance(const Point3D point) const {
	return normal().dot(point) - d;
}

Plane3D Plane3D::offset(double offset_by) const {
	return Plane3D(a, b, c, (d + offset_by));
}


bool Plane3D::parallel(const Plane3D plane2) const {
	// The following assumes that the plane normals are unit vectors.
  if (this->normal() == plane2.normal() || this->normal() == (plane2.normal() * -1)) {
  	return true;
  }
  return false;
}

Plane3D& Plane3D::reverse(){
  a = a*-1;
  b = b*-1;
  c = c*-1;
  d = d*-1;
  return (*this);
}

double Plane3D::angle_with(const Plane3D plane2) const {
	// Solution taken form this: https://www.youtube.com/watch?v=C6ElQdRbHaE
  return acos(std::abs(normal().dot(plane2.normal())));
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


Point3D Line3D::intersection(const Line3D &other_line) {
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


Point3D Line3D::intersection(const Plane3D &plane) {
	// @see http://paulbourke.net/geometry/pointlineplane/
	double u = ((plane.a * this->point.x) + (plane.b * this->point.y) + (plane.c * this->point.z) + plane.d) /
  						(-(plane.a * this->direction.x) - (plane.b * this->direction.y) - (plane.c * this->direction.z));
  if (std::abs(u) < EPSILON) {
  	// The line is parallel to or on the plane
    return Point3D(false);
  }
  Point3D point_of_intersection = this->point + (u * this->direction);
	return point_of_intersection;
}


std::pair<Point3D, Point3D> Line3D::closest_points(const Line3D &other_line) const {
	// @see http://paulbourke.net/geometry/pointlineplane/
	double d1343,d4321,d1321,d4343,d2121;
  double numer,denom;
	// p13 is a vector betwee two points on different lines
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
    return std::pair<Point3D,Point3D> (Point3D(false), Point3D(false));
  }
  numer = d1343 * d4321 - d1321 * d4343;

  double mua = numer / denom;
  double mub = (d1343 + d4321 * mua) / d4343;
	Point3D pa = this->point + (this->direction * mua);
	Point3D pb = other_line.point + (other_line.direction * mub);
  return std::pair<Point3D,Point3D> (pa, pb);
}


bool Line3D::on_line(const Point3D test_point) const {
	// Get the factor with which to multiply the x value, and see if it is the same for y and z values too.
  double factor = (test_point.x - this->point.x) / this->direction.x;
  double y_test = this->point.y + (this->direction.y * factor);
  if (std::abs(y_test - test_point.y) > EPSILON) {
  	return false;
  }
  double z_test = this->point.z + (this->direction.z * factor);
  if (std::abs(z_test - test_point.z) > EPSILON) {
  	return false;
  }
  return true;
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
	Vector3D difference = this->direction.unit() - vector.unit();
  double length = difference.length();
  if (length < EPSILON || (length - 2.0) < EPSILON) {
  	return true;
  }
  return false;
}



} /* namespace CW */
