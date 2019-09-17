// Copyright 2015 Trimble Navigation Ltd. All rights reserved.
// This file is intended for public distribution.

#ifndef LAYOUT_MODEL_GRID_H_
#define LAYOUT_MODEL_GRID_H_

#include <LayOutAPI/common.h>
#include <LayOutAPI/model/defs.h>

#include <SketchUpAPI/color.h>

/**
@struct LOGridRef
@brief References a document's grid settings.
*/

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
@brief Gets the major space size of the grid.
@param[in]  grid    The grid object.
@param[out] spacing The spacing of the major divisions of the grid.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if spacing is NULL
*/
LO_RESULT LOGridGetMajorSpacing(LOGridRef grid, double* spacing);

/**
@brief Gets the number of minor divisions of the grid.
@param[in]  grid      The grid object.
@param[out] divisions The minor division count.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if divisions is NULL
*/
LO_RESULT LOGridGetMinorDivisions(LOGridRef grid, int* divisions);

/**
@brief Gets the color for the major grid lines.
@param[in]  grid  The grid object.
@param[out] color The color of the major grid lines.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if color is NULL
*/
LO_RESULT LOGridGetMajorColor(LOGridRef grid, SUColor* color);

/**
@brief Gets the color for the minor grid lines.
@param[in]  grid  The grid object.
@param[out] color The color of the minor grid lines.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if color is NULL
*/
LO_RESULT LOGridGetMinorColor(LOGridRef grid, SUColor* color);

/**
@brief Gets whether or not the grid is visible.
@param[in]  grid The grid object.
@param[out] show Whether or not the grid is visible.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if show is NULL
*/
LO_RESULT LOGridGetShow(LOGridRef grid, bool* show);

/**
@brief Gets whether or not the major grid lines are visible.
@param[in]  grid The grid object.
@param[out] show Whether or not the major grid lines are visible.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if show is NULL
*/
LO_RESULT LOGridGetShowMajor(LOGridRef grid, bool* show);

/**
@brief Gets whether or not the minor grid lines are visible.
@param[in]  grid The grid object.
@param[out] show Whether or not the minor grid lines are visible.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if show is NULL
*/
LO_RESULT LOGridGetShowMinor(LOGridRef grid, bool* show);

/**
@brief Gets whether or not the grid will be printed.
@param[in]  grid The grid object.
@param[out] show Whether or not the grid will be printed.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if grid does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if show is NULL
*/
LO_RESULT LOGridGetPrint(LOGridRef grid, bool* show);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // LAYOUT_MODEL_GRID_H_
