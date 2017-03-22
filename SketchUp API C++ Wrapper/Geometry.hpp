//
//  Geometry.hpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef Geometry_h
#define Geometry_h

#include <algorithm>
#include <vector>

#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/face.h>

namespace CW {
/**
* Radians class deals with calculating radians.
* Usage: Radians(double) - converts double into radians.  Negative and values more than 2*pi will be made into a value between 0 and 2*pi.
*/
class Radians
{
private:

public:
  double m_val;
	static constexpr double PI = 3.141592653589793;
  // Estimate the degree of correctness of angles (Sketchup Tolerance is 1/1000", so try to make some sort of guess - suggest discrepancies of 1/1000" over radians rotations over 30m (approx 1000")
  constexpr static double EPSILON = 0.0000000000005; // Sketchup Tolerance is 1/1000"
  
  Radians() {};
  Radians(const double &rhs);
	
  /**
  * Copy constructor
  */
  Radians(const Radians &radians);
  
  /**
  * Radians can be cast simply into a double without data loss.
  */
  operator double() const;
  
  /**
  * Overloaded assignment operator
  */
  Radians &operator=(const Radians &radians);
  
  /**
  *	Arithmetic operator overloads.  These work like doubles, but will always give a value between 0 and 2*pi
  */
  Radians operator+(const double value) const;
  Radians operator-(const double value) const;
  Radians operator*(const double multiplier) const;
  Radians operator/(const double divider) const;
  
  bool operator==(const Radians rhs) const;
  bool operator==(const double rhs) const;
  
  // TODO: below does not look right
 	bool closest(const Radians value);
};

class Point3D;

/*
* Vector3D class is analagous to SUVector3D struct, and holds the same member variables.
* 
* Class methods are included to allow easy vector mathematics.
* Initialisation:
* - Vector3D(SUVector3D vec)
* - Vector3D(double x, double y, double z)
*/
class Vector3D {

	protected:
  SUVector3D m_vector;
  const bool null = false; // Invalid flag
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"

  public:
  double &x;
  double &y;
  double &z;
  
  Vector3D();
  /**
  * SUVector3D objects are easily converted to Vector3D without data loss
  */
  Vector3D( SUVector3D su_vector);
  Vector3D( double x, double y, double z);
  
  /**
  * Invaid, or NULL Vector3D objects can be simulated with this constructor.
  */
  Vector3D(bool invalid);
  
  /**
  * Returns the vector between start and end points of an edge.
  */
  Vector3D( const SUEdgeRef &su_edge);
	
  /**
  * Allow conversion from Point3D.
  */
  explicit Vector3D( const Point3D point);
  
  /*
  * Cast to SUVector3D object
  */
  operator SUVector3D() const;
  
  /*
  * Cast to Point3D object
  */
  operator Point3D() const;
  
 	/*
  * Copy constructor
  */
  Vector3D(const Vector3D &vector);
  
  /**
  * Copy assignment operator
  */
  Vector3D &operator=(const Vector3D &vector);
  Vector3D &operator=(const SUVector3D &vector);
	
  /**
  * Arithmetic operator overloads
  */
  Vector3D operator+(const Vector3D &vector) const;
  Vector3D operator+(const Point3D &point) const;
  Vector3D operator+(const SUVector3D &vector) const {return *this + Vector3D(vector);}
  Vector3D operator-() const;
  Vector3D operator-(const Vector3D &vector) const;
  Vector3D operator-(const SUVector3D &vector) const {return *this - Vector3D(vector);}
  Vector3D operator*(const double &scalar) const;
  Vector3D operator/(const double &scalar) const;
	
  /**
  * Comparator operator overloads
  */
  friend bool operator==(const Vector3D& lhs, const Vector3D& rhs);

  friend bool operator!=(const Vector3D& lhs, const Vector3D& rhs);

	/**
  * Validty check
  */
  bool operator!() const;

  
  /*
  * Returns the length of the vector
  */
  double length() const;
  
  /*
  * Returns the unit vector
  */
  Vector3D unit() const;

  /*
  * Returns the angle between this vector and that of another.
  */
  double angle(const Vector3D vector_b) const;
  
  /**
  * Returns dot product with another vector
  */
  double dot(const Vector3D vector2) const;
  double dot(const Point3D point) const;
  
  /**
  * Returns cross product with another vector
  */
  Vector3D cross(const Vector3D vector2) const;
	
  /**
  * Returns a vector rotated about another vector, which is used as the axis.
  * @param angle in radians to rotate.
  * @param vector which will be used as the axis through which it will be rotated.
  */
	Vector3D rotate_about(double angle, Vector3D axis) const;
  
  /**
  * Returns the vector of the Edge object
  */
  static SUVector3D get_vector(const SUEdgeRef &edge);
};

/**
* Allows the multiplication operator to be on the other side of the vector.
*/
static Vector3D operator*(const double &lhs, const Vector3D &rhs)
{  return rhs * lhs;}


/*
* Point3D class is analagous to SUPoint3D struct, and holds the same variables.
* 
* Class methods are given to allow easy vector mathematics.
*
* Point3D inherits from Vector3D, as the concept of a point and a vector is interchangeable in vector mathematics.
*/
class Point3D {
	private:
  SUPoint3D m_point;
  const bool null = false; // Invalid flag
  //constexpr static double EPSILON = 0.001; // Sketchup Tolerance is 1/1000"

	public:
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"
  double &x;
  double &y;
  double &z;

  Point3D();
  Point3D( SUPoint3D su_point);
  Point3D( SUVector3D su_vector);
  Point3D(double x, double y, double z);

	/**
  * Allow conversion from Vecgtor3D
  */
  explicit Point3D( const Vector3D vector);

  /**
  * Invaid, or NULL Point3D objects can be simulated with this constructor.
  */
  Point3D(bool invalid);
  
  // Copy assignment operator
  Point3D &operator=(const Point3D &vector);

  /*
  * Cast to SUPoint3D struct
  */
  operator SUPoint3D() const;
  operator SUPoint3D*();
  
	/*
  * Cast to Vector3D
  */
  operator Vector3D() const;
  
  /**
  * Arithmetic operator overloads
  */
  Point3D operator+(const Point3D &point) const;
  Point3D operator+(const Vector3D &vector) const;
  Point3D operator+(const SUPoint3D &point) const;
  Point3D operator-(const Point3D &point) const;
  Point3D operator-(const Vector3D &vector) const;
  Point3D operator-(const SUPoint3D &point) const;
  Point3D operator*(const double &scalar) const;
  Point3D operator/(const double &scalar) const;
  
  /**
  * Comparative operators
  */
  bool operator!() const;
  
  friend bool operator==(const Point3D& lhs, const Point3D& rhs);
  friend bool operator!=(const Point3D& lhs, const Point3D& rhs);
  
};

// Forward declaration
class Line3D;
/*
* Plane3D class is analagous to SUPlane3D struct, and holds the same variables.
* 
* Class methods are included to allow easy vector mathematics.
*/
class Plane3D {
	private:
  SUPlane3D m_plane;
  const bool null = false; // Invalid flag
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"

  public:
  double &a;
  double &b;
  double &c;
  double &d;

  Plane3D();
  Plane3D(const SUPlane3D plane);
  Plane3D(double a, double b, double c, double d);
  Plane3D(const SUFaceRef &face);

  /**
  * Invaid, or NULL Plane3D objects can be simulated with this constructor.
  */
  Plane3D(bool invalid);
  
  /**
  * Create a plane using a point and a vector.
  */
  Plane3D(const Vector3D normal, const Point3D point);
  Plane3D(const Point3D point, const Vector3D normal);

 	// Copy constructor
  Plane3D(const Plane3D &plane);
  
  // Overload copy assignment operator
  Plane3D &operator=(const Plane3D &plane);
  
  /**
  * Implicit conversion to SUPlane3D
  */
  operator SUPlane3D() const;
  
  /**
  * Comparative operators
  */
  bool operator!() const;
	
  /**
  * Checks if geometry is on the plane
  */
  bool coplanar(const Plane3D test_plane) const;
    
  /**
  * Returns the normal of the plane
  */
  Vector3D normal() const;
  
  /**
  * Returns line of intersection between two planes
  */
  Line3D intersection(const Plane3D plane2) const;
  
  /**
  * Returns point where a line intersects this plane.
  */
  Point3D intersection(const Line3D &line) const;
  
	double angle_with(const Plane3D plane2) const;
	double angle(const Plane3D plane2) const { return angle_with(plane2);};
	
  /**
  * Returns the distance of a point from the plane.  It can be negative as the plane has a front and back side.
  */
  double distance(const Point3D point) const;
  
  /**
  * Returns a plane moved along normal by given amount.
  */
  Plane3D offset(double offset_by) const;
  
  /**
  * Checks if the plane is parallel with another.
  */
	bool parallel(const Plane3D plane2) const;

  
  /**
  * Returns a Plane with normals reversed.
  */
  Plane3D& reverse();
  
  /**
  * Returns SUPlane3D of SUFaceRef object
  */
  static SUPlane3D get_plane(const SUFaceRef &face);
  
};

class BoundingBox3D {
	private:
  SUBoundingBox3D m_bounding_box;
  const bool null = false; // Invalid flag

  public:
  BoundingBox3D();
  BoundingBox3D(SUBoundingBox3D bounding_box);

  /**
  * Invalid, or NULL BoundingBox3D objects can be simulated with this constructor.
  */
  BoundingBox3D(bool valid);
  
  /** Casting overload */
  operator SUBoundingBox3D() const;
  
  /**
  * Comparative operators
  */
  bool operator!() const;
  
  /**
  * Returns the point where x,y and z are at their minimum
  */
  Point3D min() const;
  
  /**
  * Returns the point where x,y and z are at their maximum
  */
  Point3D max() const;

};


/**
* A Line3D represents a point and a vector, which carries on into infinity in two directions.  There is no equivalent native object in the Sketchup C API.
*/
class Line3D {
	private:
  Point3D m_point;
  Vector3D m_direction;
  const bool null = false; // Invalid flag
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"

  public:
  Line3D();
  Line3D(const Point3D point, const Vector3D direction);

  /**
  * Invaid, or NULL Line3D objects can be simulated with this constructor.
  */
  Line3D(bool invalid);
  
  Point3D &point;
  Vector3D &direction;

  // Overload copy assignment operator
  Line3D &operator=(const Line3D &line);
  
  /**
  * Comparative operators
  */
  bool operator!() const;
  
  Point3D intersection(const Line3D &line) const;
  Point3D intersection(const Plane3D &plane) const;
  
  
  /**
  * Return the closest points on two lines.
  * @param other_line the line with which to find the shortest.
  * @return pair of Point3D objects representing the point (first) on this line, and the point (second) on the other line. If the lines are parallel, the point3D objects returned will be invalid.
  */
  std::pair<Point3D, Point3D> closest_points(const Line3D &line) const;
  
  
  /**
  * Check if point is on line.
  */
  bool on_line(const Point3D) const;
  
  /**
  * Returns true if the Line or vector given is parallel to this line.
  */
  bool parallel(const Line3D &line) const;
  bool parallel(const Vector3D &vector) const;
  
};

} /* namespace CW */
#endif /* Geometry_h */
