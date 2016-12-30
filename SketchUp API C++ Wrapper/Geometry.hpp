//
//  Geometry.h
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 31/08/2016.
//  Copyright © 2016 SketchUp. All rights reserved.
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
  operator SUVector3D();
  
  /*
  * Cast to Point3D object
  */
  operator Point3D();
  
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
  Vector3D operator+(const Vector3D &vector);
  Vector3D operator+(const Point3D &point);
  Vector3D operator+(const SUVector3D &vector) {return *this + Vector3D(vector);}
  Vector3D operator-(const Vector3D &vector);
  Vector3D operator-(const SUVector3D &vector) {return *this - Vector3D(vector);}
  Vector3D operator*(const double &scalar) const;
  Vector3D operator/(const double &scalar) const;
	
  /**
  * Comparator operator overloads
  */
  bool operator==(const Vector3D &vector) const;

  bool operator!=(const Vector3D &vector) const;

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
	Vector3D rotate_about(double angle, Vector3D axis);
  
  /**
  * Returns the vector of the Edge object
  */
  static SUVector3D vector_from_edge(const SUEdgeRef &su_edge);
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

	public:
  double &x;
  double &y;
  double &z;

  Point3D();
  Point3D( SUPoint3D su_point);
  Point3D( SUVector3D su_vector);
  Point3D(double x, double y, double z);

  /**
  * Invaid, or NULL Point3D objects can be simulated with this constructor.
  */
  Point3D(bool invalid);
  
  // Copy assignment operator
  Point3D &operator=(const Point3D &vector);

  /*
  * Cast to SUPoint3D struct
  */
  operator SUPoint3D();
  operator SUPoint3D*();
  
	/*
  * Cast to Vector3D
  */
  operator Vector3D();
  
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
  * Comparative operators
  */
  bool operator!() const;
  
  /**
  * Returns the normal of the plane
  */
  Vector3D normal() const;
  
  /**
  * Returns line of intersection between two planes
  */
  Line3D intersection(const Plane3D plane2) const;
  
	double angle_with(const Plane3D plane2) const;
	double angle(const Plane3D plane2) const { return angle_with(plane2);};
	
  /**
  * Returns the distance of a point from the plane.
  */
  double distance(const Point3D point) const;
  
  /**
  * Returns a plane moved along normal by given amount.
  */
  Plane3D offset(double offset_by);
  
  /**
  * Checks if the plane is parallel with another.
  */
	bool parallel(const Plane3D plane2) const;
	
  /**
  * Returns a Plane with normals reversed.
  */
  Plane3D reverse();
  
  /**
  * Returns SUPlane3D of SUFaceRef object
  */
  static SUPlane3D get_plane(const SUFaceRef &face);
  
  /**
  * Implicit conversion to SUPlane3D
  */
  operator SUPlane3D();
  
};

class BoundingBox3D {
	private:
  SUBoundingBox3D m_bounding_box;
  const bool null = false; // Invalid flag

  public:
  BoundingBox3D();
  BoundingBox3D(SUBoundingBox3D bounding_box);

  /**
  * Invaid, or NULL BoundingBox3D objects can be simulated with this constructor.
  */
  BoundingBox3D(bool invalid);
 
  /**
  * Comparative operators
  */
  bool operator!() const;
  
  /**
  * Returns the point where x,y and z are at their minimum
  */
  Point3D min();
  
  /**
  * Returns the point where x,y and z are at their maximum
  */
  Point3D max();

};


/**
* A Line3D represents a point and a vector, which carries on into infinity in two directions.
*/
class Line3D {
	private:
  Point3D m_point;
  Vector3D m_direction;
  const bool null = false; // Invalid flag

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
  
  Point3D intersection(const Line3D &line);
  Point3D intersection(const Plane3D &plane);
  
  /**
  * Returns true if the Line or vector given is parallel to this line.
  */
  bool parallel(const Line3D &line);
  bool parallel(const Vector3D &vector);
  
};

} /* namespace CW */
#endif /* Geometry_h */
