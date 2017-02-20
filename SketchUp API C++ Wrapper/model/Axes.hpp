//
//  Axes.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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
