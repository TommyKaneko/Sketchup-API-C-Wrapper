//
//  Transformation.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Transformation_hpp
#define Transformation_hpp

#include <stdio.h>

#include <SketchUpAPI/transformation.h>

#include "Axes.hpp"
#include "Geometry.hpp"

namespace CW {

class Transformation {
	private:
  SUTransformation m_transformation;
  
  public:
  Transformation(SUTransformation transformation);
  Transformation(Axes axes, Vector3D translation, double scalar);
  Transformation(Vector3D x_axis, Vector3D y_axis, Vector3D z_axis, Vector3D translation, double scalar);
  
  
  /*
  * Cast to SUTransformation object
  */
  operator SUTransformation();
  
  /**
  * Return the inverse Transformation object (see inverse Transformation matrices)
  */
  Transformation inverse();
  
  /**
  * Mulitplication of Transformation matrices.
  */
  Transformation operator*(Transformation transform);
  
  /**
  * Return transformed vectors.
  */
  Vector3D operator*(Vector3D vector);
  Point3D operator*(Point3D vector);
};

} /* namespace CW */
#endif /* Transformation_hpp */
