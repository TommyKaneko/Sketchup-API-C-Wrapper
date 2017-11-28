// Copyright 2015 Trimble Navigation Ltd. All rights reserved.
// This file is intended for public distribution.

#ifndef LAYOUT_MODEL_PATH_H_
#define LAYOUT_MODEL_PATH_H_

#include <LayOutAPI/common.h>

/**
@struct LOPathRef
@brief References a path entity. A path entity represents a continuous,
       multi-segment polyline or Bezier curve.
*/
DEFINE_SU_TYPE(LOPathRef)

/**
@enum LOPathPointType
@brief Defines how a point is to be interpreted in a path entity.
*/
typedef enum {
  LOPathPointType_MoveTo = 0,    ///< Reserved for the first point in a path.
  LOPathPointType_LineTo,        ///< Straight line segment to a given point.
  LOPathPointType_BezierTo,      ///< Bezier line segment to a given point.
  LOPathPointType_ArcCenter,     ///< Control point that defines the center of an arc.
  LOPathPointType_BezierControl, ///< Control point that defines Bezier curvature.
  LOPathPointType_Close,         ///< Close the path with a straight line segment back to the first point.
  LONumPathPointTypes
} LOPathPointType;

#include <LayOutAPI/model/ellipse.h>
#include <LayOutAPI/model/entity.h>
#include <LayOutAPI/model/rectangle.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
@brief Creates a new path object with a straight line between start_point and
       end_point.
@param[out] path        The path object.
@param[in]  start_point The starting point of the path.
@param[in]  end_point   The ending point of the path.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if path is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *path already refers to a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if start_point is NULL
- \ref SU_ERROR_NULL_POINTER_INPUT if end_point is NULL
- \ref SU_ERROR_GENERIC if the path would be zero length
*/
LO_RESULT LOPathCreate(LOPathRef* path,
                       const LOPoint2D* start_point,
                       const LOPoint2D* end_point);

/**
@brief Creates a new path object with a Bezier curve between start_point and
       end_point.
@param[out] path            The path object.
@param[in]  start_point     The starting point of the path.
@param[in]  control_point_1 The first control point.
@param[in]  control_point_2 The second control point.
@param[in]  end_point       The ending point of the path.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if path is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *path already refers to a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if start_point is NULL
- \ref SU_ERROR_NULL_POINTER_INPUT if control_point_1 is NULL
- \ref SU_ERROR_NULL_POINTER_INPUT if control_point_2 is NULL
- \ref SU_ERROR_NULL_POINTER_INPUT if end_point is NULL
- \ref SU_ERROR_GENERIC if the path would be zero length
*/
LO_RESULT LOPathCreateBezier(LOPathRef* path,
                             const LOPoint2D* start_point,
                             const LOPoint2D* control_point_1,
                             const LOPoint2D* control_point_2,
                             const LOPoint2D* end_point);

/**
@brief Creates a new path object representing the same shape as a rectangle
       object.
@param[out] path      The path object.
@param[in]  rectangle The rectangle object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if path is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *path already refers to a valid object
- \ref SU_ERROR_INVALID_INPUT if rectangle does not refer to a valid object
*/
LO_RESULT LOPathCreateFromRectangle(LOPathRef* path, LORectangleRef rectangle);

/**
@brief Creates a new path object representing the same shape as an ellipse
       object.
@param[out] path    The path object.
@param[in]  ellipse The ellipse object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if path is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *path already refers to a valid object
- \ref SU_ERROR_INVALID_INPUT if ellipse does not refer to a valid object
*/
LO_RESULT LOPathCreateFromEllipse(LOPathRef* path, LOEllipseRef ellipse);

/**
@brief Adds a reference to a path object.
@param[in] path The path object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
*/
LO_RESULT LOPathAddReference(LOPathRef path);

/**
@brief Releases a path object. The object will be invalidated if releasing the
       last reference.
@param[in] path The path object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_INPUT if path is NULL
- \ref SU_ERROR_INVALID_INPUT if *path does not refer to a valid object
*/
LO_RESULT LOPathRelease(LOPathRef* path);

/**
@brief Converts from a \ref LOEntityRef to a \ref LOPathRef.
       This is essentially a downcast operation so the given \ref LOEntityRef
       must be convertible to a \ref LOPathRef.
@param[in] entity The entity object.
@return
- The converted \ref LOPathRef if the downcast operation succeeds
- If not, the returned reference will be invalid
*/
LO_EXPORT LOPathRef LOPathFromEntity(LOEntityRef entity);

/**
@brief Converts from a \ref LOPathRef to a \ref LOEntityRef.
       This is essentially an upcast operation.
@param[in] path The path object.
@return
- The converted \ref LOEntityRef if path is a valid object
- If not, the returned reference will be invalid
*/
LO_EXPORT LOEntityRef LOPathToEntity(LOPathRef path);

/**
@brief Gets the number of points in the path.
@param[in]  path The path object.
@param[out] size The number of points in the path.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if size is NULL
*/
LO_RESULT LOPathGetNumberOfPoints(LOPathRef path, size_t* size);

/**
@brief Gets the points in the path.
@param[in]  path                    The path object.
@param[in]  points_size             The maximum number of points to copy.
@param[out] points                  The array that the points will be copied to.
@param[out] number_of_points_copied The number of points actually copied.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if points_size is less than 1
- \ref SU_ERROR_NULL_POINTER_OUTPUT if points is NULL
- \ref SU_ERROR_NULL_POINTER_OUTPUT if points_copied is NULL
*/
LO_RESULT LOPathGetPoints(LOPathRef path,
                          size_t points_size,
                          LOPoint2D points[],
                          size_t* number_of_points_copied);

/**
@brief Gets the point in the path at the given index.
@param[in]  path          The path object.
@param[in]  point_index   The index of the point to get.
@param[out] point         The point at the given index.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if point_index is out of range
- \ref SU_ERROR_NULL_POINTER_OUTPUT if point is NULL
*/
LO_RESULT LOPathGetPointAtIndex(LOPathRef path,
                                size_t point_index,
                                LOPoint2D* point);

/**
@brief Gets the type of each point in the path.
@param[in]  path                        The path object.
@param[in]  pointtypes_size             The maximum number of point types to
                                        copy.
@param[out] pointtypes                  The array that the point types will be
                                        copied to.
@param[out] number_of_pointtypes_copied The number of point types actually
                                        copied.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if pointtypes_size is less than 1
- \ref SU_ERROR_NULL_POINTER_OUTPUT if pointtypes is NULL
- \ref SU_ERROR_NULL_POINTER_OUTPUT if number_of_pointtypes_copied is NULL
*/
LO_RESULT LOPathGetPointTypes(LOPathRef path,
                              size_t pointtypes_size,
                              LOPathPointType pointtypes[],
                              size_t* number_of_pointtypes_copied);

/**
@brief Gets the type of the specified point in the path.
@param[in]  path        The path object.
@param[in]  point_index The index of the point to get type information of.
@param[out] pointtype   The point type.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if pointtype is NULL
*/
LO_RESULT LOPathGetPointTypeAtIndex(LOPathRef path,
                                    size_t point_index,
                                    LOPathPointType* pointtype);

/**
@brief Gets the start point for the path.
@param[in]  path  The path object.
@param[out] point The start point.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if point is NULL
*/
LO_RESULT LOPathGetStartPoint(LOPathRef path,
                              LOPoint2D* point);

/**
@brief Gets the end point for the path.
@param[in]  path  The path object.
@param[out] point The end point.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if point is NULL
*/
LO_RESULT LOPathGetEndPoint(LOPathRef path,
                            LOPoint2D* point);

/**
@brief Creates a new path object in the shape of the start arrow of the given
       path.
@param[out] arrow_path The start arrow as a path.
@param[in]  path       The path object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if arrow_path is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *arrow_path already refers to a valid object
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NO_DATA if path does not have a start arrow
*/
LO_RESULT LOPathCreateFromStartArrow(LOPathRef* arrow_path,
                                     LOPathRef path);

/**
@brief Creates a new path object in the shape of the end arrow of the given
       path.
@param[out] arrow_path The end arrow as a path.
@param[in]  path       The path object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if arrow_path is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *arrow_path already refers to a valid object
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NO_DATA if path does not have an end arrow
*/
LO_RESULT LOPathCreateFromEndArrow(LOPathRef* arrow_path,
                                   LOPathRef path);

/**
@brief Gets whether or not the path is a closed loop.
@param[in]  path      The path object.
@param[out] is_closed Whether the path is closed or not.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if is_closed is NULL
*/
LO_RESULT LOPathGetClosed(LOPathRef path,
                          bool* is_closed);

/**
@brief Gets the length of the path.
@param[in]  path              The path object.
@param[out] parametric_length The length of the path.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if parametric_length is NULL
*/
LO_RESULT LOPathGetParametricLength(LOPathRef path,
                                    double* parametric_length);

/**
@brief Gets the point at the given distance along the path.
@param[in]  path             The path object.
@param[in]  parametric_value The distance along the path to get the point.
@param[out] point            The point at the given distance along the path.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if point is NULL
- \ref SU_ERROR_OUT_OF_RANGE if parametric_value is less than 0 or greater than
  the path's parametric length
*/
LO_RESULT LOPathGetPointAtParameter(LOPathRef path,
                                    double parametric_value,
                                    LOPoint2D* point);

/**
@brief Gets the tangent at the given distance along the path.
@param[in]  path             The path object.
@param[in]  parametric_value The distance along the path to get the tangent.
@param[out] tangent          The tangent at the given distane along the path.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if tangent is NULL
- \ref SU_ERROR_OUT_OF_RANGE if parametric_value is less than 0 or greater than
  the path's parametric length
*/
LO_RESULT LOPathGetTangentAtParameter(LOPathRef path,
                                      double parametric_value,
                                      LOPoint2D* tangent);

/**
@brief Adds a point to the end of the path.
@param[in] path  The path object.
@param[in] point The point to append.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if point is NULL
- \ref SU_ERROR_LAYER_LOCKED if path is on a locked layer
*/
LO_RESULT LOPathAppendLineTo(LOPathRef path, const LOPoint2D* point);

/**
@brief Adds a Bezier point to the end of the path.
@param[in]  path            The path object.
@param[in]  control_point_1 The first control point.
@param[in]  control_point_2 The second control point.
@param[in]  point           The point to append.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if control_point_1 is NULL
- \ref SU_ERROR_NULL_POINTER_INPUT if control_point_2 is NULL
- \ref SU_ERROR_NULL_POINTER_INPUT if point is NULL
- \ref SU_ERROR_LAYER_LOCKED if path is on a locked layer
*/
LO_RESULT LOPathAppendBezierTo(LOPathRef path,
                               const LOPoint2D* control_point_1,
                               const LOPoint2D* control_point_2,
                               const LOPoint2D* point);

/**
@brief Finishes the path loop if the path is not closed.
@param[in] path The path object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if path does not refer to a valid object
- \ref SU_ERROR_GENERIC if path has already been closed.
- \ref SU_ERROR_LAYER_LOCKED if path is on a locked layer
*/
LO_RESULT LOPathClose(LOPathRef path);

#ifdef __cplusplus
}  // end extern "C"
#endif  // __cplusplus
#endif  // LAYOUT_MODEL_PATH_H_
