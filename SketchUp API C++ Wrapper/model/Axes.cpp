//
//  Axes.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 01/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "Axes.hpp"

#include <cassert>

namespace CW {
  
Axes::Axes(SUAxesRef axes):
  m_axes(axes)
{}

Vector3D Axes::x_axis() const {
  SUVector3D axis;
  SU_RESULT res = SUAxesGetXAxis(m_axes, &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}

Vector3D Axes::y_axis() const {
  SUVector3D axis;
  SU_RESULT res = SUAxesGetYAxis(m_axes, &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}

Vector3D Axes::z_axis() const {
  SUVector3D axis;
  SU_RESULT res = SUAxesGetZAxis(m_axes, &axis);
  assert(res == SU_ERROR_NONE);
  return Vector3D(axis);
}


Point3D Axes::origin() const {
  SUPoint3D origin;
  SU_RESULT res = SUAxesGetOrigin(m_axes, &origin);
  assert(res == SU_ERROR_NONE);
  return Point3D(origin);
}


Transformation Axes::transformation() const {
  SUTransformation transform;
  SU_RESULT res = SUAxesGetTransform(m_axes, &transform);
  assert(res == SU_ERROR_NONE);
  return Transformation(transform);
}

} /* namespace CW */