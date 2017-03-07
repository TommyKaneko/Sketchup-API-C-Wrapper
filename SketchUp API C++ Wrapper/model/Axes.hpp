//
//  Axes.hpp
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

#ifndef Axes_hpp
#define Axes_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include <SketchUpAPI/model/axes.h>

#include "DrawingElement.hpp"
#include "Geometry.hpp"
#include "Transformation.hpp"

namespace CW {

/*
* Entity object wrapper
*/
class Axes :public DrawingElement {
  private:
  SUAxesRef m_axes;
  
  /** Returns an empty SUAxesRef object using SUAxesCreate(). **/
  static SUAxesRef create_axes();

  /** Returns a custom SUAxesRef object using SUAxesCreateCustom(). **/
  static SUAxesRef create_custom_axes(const SUPoint3D& origin, const SUVector3D& xaxis, const SUVector3D& yaxis, const SUVector3D& zaxis);

  /** Returns a copied SUAxesRef object from the given Axes object **/
  static SUAxesRef copy_reference(const Axes& other);
  
  public:
	/**
  * Constructor for null object.
  */
  Axes();
  
  /**
  * Standard constructor from existing SUAxesRef object.
  * @param axes - existing SUAxesRef object to wrap.
  * @param attached - true if the SUAxesRef has been attached to the model.  Required for object release purposes.
  */
  Axes(SUAxesRef axes, bool attached = true);
	
  /**
  * Constructs Axes object from the given origin, x, y and z axes.  The vectors passed to the constructor must be orthogonal, otherwise, a null object will be created.
  * @param origin - origin of the Axes.
  * @param x_axes - vector representing the x axis.
  * @param y_axes - vector representing the y axis.
  * @param z_axes - vector representing the z axis.
  */
  Axes(Point3D origin, Vector3D x_axis, Vector3D y_axis, Vector3D z_axis);
  
  /** Copy constructor */
  Axes(const Axes& other);
  
  /** Destructor */
  ~Axes();
  
  /** Copy assignment operator override */
  Axes& operator=(const Axes& other);
	
  /**
  * Operator overload signifies if this a valid object.
  */
  bool operator!() const;
 	
  /**
  * Return the vectors representing the axes.
  */
  Vector3D x_axis() const;
  Vector3D y_axis() const;
  Vector3D z_axis() const;
	
  /**
  * Return the origin of the axes as a Point3D object.
  */
  Point3D origin() const;
  
  /**
  * Return the transformation object of the axes.
  */
  Transformation transformation() const;
};

} /* namespace CW */
#endif /* Axes_hpp */
