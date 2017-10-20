//
//  Axes.hpp
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

#ifndef Axes_hpp
#define Axes_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include <SketchUpAPI/model/axes.h>

#include "SUAPI-CppWrapper/model/DrawingElement.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"

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
