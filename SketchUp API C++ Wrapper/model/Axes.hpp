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

#include "Geometry.hpp"
#include "Transformation.hpp"

namespace CW {

/*
* Entity object wrapper
*/
class Axes {
  private:
  SUAxesRef m_axes;
  
  public:
  Axes(SUAxesRef axes);
  
  Vector3D get_x_axis() const;
  Vector3D get_y_axis() const;
  Vector3D get_z_axis() const;
	
  Point3D get_origin() const;
  
  /**
  * Return the transformation object of the axes.
  */
  Transformation transformation();
};

} /* namespace CW */
#endif /* Axes_hpp */
