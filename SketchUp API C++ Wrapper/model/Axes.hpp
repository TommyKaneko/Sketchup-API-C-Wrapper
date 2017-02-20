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
  
  static SUAxesRef create_axes();
  
  static SUAxesRef create_custom_axes(const SUPoint3D& origin, const SUVector3D& xaxis, const SUVector3D& yaxis, const SUVector3D& zaxis);
  
  static SUAxesRef copy_reference(const Axes& other);
  
  public:
	Axes();
  
  Axes(SUAxesRef axes, bool attached = true);
 
  Axes(Point3D origin, Vector3D x_axis, Vector3D y_axis, Vector3D z_axis);
  
  Axes(const Axes& other);
  
  ~Axes();
  
  Axes& operator=(const Axes& other);
 
  Vector3D x_axis() const;
  Vector3D y_axis() const;
  Vector3D z_axis() const;
	
  Point3D origin() const;
  
  /**
  * Return the transformation object of the axes.
  */
  Transformation transformation() const;
};

} /* namespace CW */
#endif /* Axes_hpp */
