// Copyright 2015 Trimble Navigation Ltd., All rights reserved.
// This file is intended for public distribution.

#ifndef LAYOUT_GEOMETRY_GEOMETRY_H_
#define LAYOUT_GEOMETRY_GEOMETRY_H_

#include <SketchUpAPI/geometry.h>

#pragma pack(push, 8)

/**
@typedef LOPoint2D
@brief Represents a point in 2-dimensional paper space.
*/
typedef SUPoint2D LOPoint2D;

/**
@typedef LOPoint3D
@brief Represents a point in 3-dimensional model space.
*/
typedef SUPoint3D LOPoint3D;


/**
@struct LOAxisAlignedRect2D
@brief Represents a 2D rectangle that is aligned with the X and Y axis of the
       coordinate system.
*/
struct LOAxisAlignedRect2D {
  LOPoint2D upper_left;
  LOPoint2D lower_right;
};

/**
@struct LOOrientedRect2D
@brief Represents a 2D rectangle that might be rotated. Each corner of the
       rectangle is represented by a separate 2D point.
*/
struct LOOrientedRect2D {
  LOPoint2D upper_left;
  LOPoint2D upper_right;
  LOPoint2D lower_right;
  LOPoint2D lower_left;
};

/**
@struct LOTransformMatrix2D
@brief 2x3 matrix for representing 2D transforms on LayOut entities. The matrix
       is stored in column-major format:
<pre>
 m11 m21 tx
 m12 m22 ty
</pre>
*/
struct LOTransformMatrix2D {
  double m11;
  double m12;
  double m21;
  double m22;
  double tx;
  double ty;
};

#pragma pack(pop)

#endif  // LAYOUT_GEOMETRY_GEOMETRY_H_
