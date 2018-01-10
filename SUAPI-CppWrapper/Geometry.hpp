//
//  Geometry.hpp
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

// Remove Visual Studio min and max macros
#ifdef WIN32
  #undef min
  #undef max
#endif

#ifndef Geometry_h
#define Geometry_h

#include <algorithm>
#include <vector>

#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/face.h>

namespace CW {

// Forward declarations
class Face;
class Edge;

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
  
  bool operator==(const Radians& rhs) const;
  bool operator==(const double rhs) const;
	
	/**
	* Gives the difference between the two radians values as a positive double value.
	*/
  double difference(const Radians& other) const;
  
  // TODO: below does not look right
 	bool closest(const Radians& value);
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
  bool null = false; // Invalid flag

  public:
  double &x;
  double &y;
  double &z;
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"
  
  Vector3D();
  /**
  * SUVector3D objects are easily converted to Vector3D without data loss
  */
  Vector3D( SUVector3D su_vector);
  Vector3D( double x, double y, double z);
  
  /**
  * Invaid, or NULL Vector3D objects can be simulated with this constructor.
  */
  Vector3D(bool valid);
  
  /**
  * Returns the vector between start and end points of an edge.
  */
  Vector3D( const Edge &edge);
	
  /**
  * Allow conversion from Point3D.
  */
  explicit Vector3D( const Point3D& point);
  
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
  Vector3D operator+(const SUVector3D &vector) const {return *this + Vector3D(vector);}
  friend Point3D operator+(const Vector3D &lhs, const Point3D& rhs);

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
  double angle(const Vector3D& vector_b) const;
  
  /**
  * Returns dot product with another vector
  */
  double dot(const Vector3D& vector2) const;
  double dot(const Point3D& point) const;
  
  /**
  * Returns cross product with another vector
  */
  Vector3D cross(const Vector3D& vector2) const;
  
  enum class Colinearity {
  	UNDEFINED,
    COLINEAR_PRO,
    COLINEAR_ANTI,
    NO
  };
  /**
  * Returns whether the vector is colinear.
  */
  Colinearity colinear(const Vector3D& vector_b) const;
	
  /**
  * Returns a vector rotated about another vector, which is used as the axis.
  * @param angle in radians to rotate.
  * @param vector which will be used as the axis through which it will be rotated.
  */
	Vector3D rotate_about(double angle, const Vector3D& axis) const;
  
  /**
  * Returns a valid vector that has zero length.
  */
  static Vector3D zero_vector();
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
  bool null = false; // Invalid flag
  //constexpr static double EPSILON = 0.001; // Sketchup Tolerance is 1/1000"

	public:
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"
  double &x;
  double &y;
  double &z;

  /**
  * Invaid, or NULL Point3D objects can be simulated with this constructor.
  */
  Point3D();
  
  /**
  * Constructs a NULL object, or a point with zero values as coordinates.
  * @param valid - true for an object with zero values, or false for a null object.
  */
  Point3D(bool valid);
	
  /**
  * Constructs a Point3D object from a SUPoint3D object.
  * @param su_point - SUPoint3D object to be wrapped in this object.
  */
  Point3D(SUPoint3D su_point);
  
  /**
  * Constructs a Point3D object from a SUVector3D object.
  * @param su_vector - SUVector3D object to be converted to this object.
  */
  Point3D(SUVector3D su_vector);
  
  /**
  * Constructs a Point3D object from given x y z coordinates.
  * @param x - the X coordinate of the point.
  * @param y - the Y coordinate of the point.
  * @param z - the Z coordinate of the point.
  */
  Point3D(double x, double y, double z);
  
  /**
  * Copy Constructor
  */
  Point3D(const Point3D& other);

	/**
  * Allows conversion from Vecgtor3D
  */
  explicit Point3D( const Vector3D& vector);
  
  /**
  * Copy assignment operator
  */
  Point3D &operator=(const Point3D &point);

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
  Vector3D operator-(const Point3D &point) const;
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
  
  
  /**
  * Returns intersection between two line segments.  Each line segment is represented by a point and a vector.
  *	@param return_colinear - if line segments overlap, return the point at which line A first interacts. with line B.  If set to false, a null point will be returned for overlapping lines.
  * @return if intersection is found the point of intersection is returned.  If no intersection exists, a null Point3D object is returned.
  */
  static Point3D intersection_between_lines(const Point3D& point_a, const Vector3D& vector_a, const Point3D& point_b, const Vector3D& vector_b, bool return_colinear = false);

  /**
  * Returns the point of intersection between a line segment and a ray drawn from a point.
  * @param point_a - start point of the line segment to intersect
  * @param vector_a - vector to the end point of the line segment to intersect
  * @param point_b - start point of the ray that will be intersected with the line.
  * @param ray_b - the vector of the direction of the ray to intersect with the line.
  * @param return_colinear - (optional) if the line and ray is collinear, setting this flag to false will mean no intersection will be returned.  Otherwise, the first point at which the ray and line becomes colinear is returned.
  * @return point representing the intersection.  If there is no intersection, then a null point will be returned.
  */
  static Point3D ray_line_intersection(const Point3D& point_a, const Vector3D& vector_a, const Point3D& point_b, const Vector3D& ray_b, bool return_colinear = false);

  
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
  bool null = false; // Invalid flag
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"

  public:
  double &a;
  double &b;
  double &c;
  double &d;

  Plane3D();
  Plane3D(const SUPlane3D plane);
  Plane3D(double a, double b, double c, double d);
  Plane3D(const Face &face);

  /**
  * Invaid, or NULL Plane3D objects can be simulated with this constructor.
  */
  Plane3D(bool invalid);
  
  /**
  * Create a plane using a point and a vector.
  */
  Plane3D(const Vector3D& normal, const Point3D& point);
  Plane3D(const Point3D& point, const Vector3D& normal);

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

  friend bool operator==(const Plane3D& lhs, const Plane3D& rhs);
  friend bool operator!=(const Plane3D& lhs, const Plane3D& rhs);
  
  /**
  * Checks if geometry is on the plane
  */
  bool coplanar(const Plane3D& test_plane) const;
    
  /**
  * Returns the normal of the plane
  */
  Vector3D normal() const;
  
  /**
  * Returns line of intersection between two planes
  */
  Line3D intersection(const Plane3D& plane2) const;
  
  /**
  * Returns point where a line intersects this plane.
  */
  Point3D intersection(const Line3D &line) const;
	
  /**
  * Returns point where a ray drawn from the given point intersects this plane. Note that this is different from a line, which effectively draws a ray in both directions of the point.
  * @param start_point - the point from which the ray will be drawn from.
  * @param direction - vector representing the direction of the ray.
  * @return point of the intersection, or a null Point3D object if no intersection exists.
  */
  Point3D intersection(const Point3D& start_point, const Vector3D& direction) const;

  /**
  * Returns point at which a line segment drawn from point A to point B intersects the plane.
  * @param point_a - the start of the line segment.
  * @param point_b - the end of the line segment.
  * @return point of the intersection, or a null Point3D object if no intersection exists.
  */
  Point3D intersection_between(const Point3D& point_a, const Point3D& point_b) const;
  
	double angle_with(const Plane3D& plane2) const;
	double angle(const Plane3D& plane2) const { return angle_with(plane2);};
	
  /**
  * Returns the distance of a point from the plane.  It can be negative as the plane has a front and back side.
  */
  double distance(const Point3D& point) const;
  
  /**
  * Returns true if the point is on the plane, within SketchUp's tolerance.
  */
  bool on_plane(const Point3D& point) const;
  
  /**
  * Returns a plane moved along normal by given amount.
  */
  Plane3D offset(double offset_by) const;
  
  /**
  * Checks if the plane is parallel with another.
  */
	bool parallel(const Plane3D& plane2) const;

  
  /**
  * Returns a Plane with normals reversed.
  */
  Plane3D inverse() const;
	
  /**
  * Create a plane from a series of points (normally vertexes within a loop).
  *
  * Note that the normal of the returned plane (if successful) will point in the direction expected of an OUTER_LOOP.
  */
  static Plane3D plane_from_loop(const std::vector<Point3D>& loop_points);
};

class BoundingBox3D {
	private:
  SUBoundingBox3D m_bounding_box;
  bool null = false; // Invalid flag

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
  * Set the minimum point
  */
	void min_point(const Point3D& point);
	
  /**
  * Returns the point where x,y and z are at their maximum
  */
  Point3D max() const;
	
  /**
  * Set the maximum point
  */
	void max_point(const Point3D& point);
};


/**
* A Line3D represents a point and a vector, which carries on into infinity in two directions.  There is no equivalent native object in the Sketchup C API.
*/
class Line3D {
	private:
  Point3D m_point;
  Vector3D m_direction;
  bool null = false; // Invalid flag
  constexpr static double EPSILON = 0.0005; // Sketchup Tolerance is 1/1000"

  public:
  Line3D();
  Line3D(const Point3D point, const Vector3D direction);
  Line3D(const Vector3D direction, const Point3D point);

  /**
  * Invaid, or NULL Line3D objects can be simulated with this constructor.
  * @param valid - if true, a valid line will be created with a point and no direction.
  */
  Line3D(bool valid);
  
  /**
  * Copy constructor
  */
  Line3D(const Line3D& other);
  
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
  * Returns the intersection between this line and a line segment drawn between two points, given by a point and a vector from that point.
  * @param point - point representing the start of the line segment.
  * @param vector - vector representing the direction and length of the line segment from the start point.
  * @return point3D object, representing the point of intersection.  If there is no intersection between the line and the line segment, a null point3D object will be returned.
  */
  Point3D intersection(const Point3D& point, const Vector3D& vector) const;
  
  
  /**
  * Return the closest points on two lines.
  * @param other_line the line with which to find the shortest.
  * @return pair of Point3D objects representing the point (first) on this line, and the point (second) on the other line. If the lines are parallel, the point3D objects returned will be invalid.
  */
  std::pair<Point3D, Point3D> closest_points(const Line3D &line) const;

  /**
  * Return the closest point on the line to the point.
  * @param point - the point for which to find the shortest point on the line
  * @return Point3D representing the point on this line, where it is closest to the target point.
  */
  Point3D closest_point(const Point3D& point) const;
  
  /**
  * Return the closest distance between the line and the given point.
  * @param point - the point for which to find the shortest distance to the line
  * @return double distance between the point and line.
  */
  double distance(const Point3D& point) const;

  /**
  * Check if point is on line.
  */
  bool on_line(const Point3D& point) const;
  
  /**
  * Check if a point lies on a line segment.
  * @param point_a - the start point of the line segment.
  * @param point_b - the end point of the line segment.
  * @param test_point - the point to test.
  * @return true if the point lies between the line segment.
  */
  static bool on_line_segment(const Point3D& point_a, const Point3D& point_b, const Point3D& test_point);
  
  /**
  * Returns true if the Line or vector given is parallel to this line.
  */
  bool parallel(const Line3D &line) const;
  bool parallel(const Vector3D &vector) const;
  
  /**
  * Compares two lines if they represent the same line.
  */
  friend bool operator==(const Line3D& lhs, const Line3D& rhs);
  
};

} /* namespace CW */
#endif /* Geometry_h */
