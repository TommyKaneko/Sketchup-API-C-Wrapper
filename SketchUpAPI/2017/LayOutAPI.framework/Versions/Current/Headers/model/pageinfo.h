// Copyright 2015 Trimble Navigation Ltd. All rights reserved.
// This file is intended for public distribution.

#ifndef LAYOUT_MODEL_PAGEINFO_H_
#define LAYOUT_MODEL_PAGEINFO_H_

#include <LayOutAPI/common.h>

/**
@struct LOPageInfoRef
@brief References the page information for a document. This specifies the
       physical page dimensions and other page-related settings for a document.
*/
DEFINE_SU_TYPE(LOPageInfoRef)

/**
@enum LOImageResolution
@brief Defines the different settings available for image resolution.
*/
typedef enum {
  LOImageResolution_Low = 0, ///< Low resolution, high efficiency.
  LOImageResolution_Medium,  ///< Medium resolution, medium efficiency.
  LOImageResolution_High,    ///< High resolution, low efficiency.
  LONumImageResolutions
} LOImageResolution;

#include <SketchUpAPI/color.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
@brief Gets the page width.
@param[in]  pageinfo  The page info object.
@param[out] width     The page width.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if width is NULL
*/
LO_RESULT LOPageInfoGetWidth(LOPageInfoRef pageinfo, double* width);

/**
@brief Sets the page width.
@param[in] pageinfo The page info object.
@param[in] width    The new page width to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if width is less than 1 or greater than 200
*/
LO_RESULT LOPageInfoSetWidth(LOPageInfoRef pageinfo, double width);

/**
@brief Gets the page height.
@param[in]  pageinfo   The page info object.
@param[out] height     The page height.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if height is NULL
*/
LO_RESULT LOPageInfoGetHeight(LOPageInfoRef pageinfo, double* height);

/**
@brief Sets the page height.
@param[in] pageinfo The page info object.
@param[in] height   The new page height to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if height is less than 1 or greater than 200
*/
LO_RESULT LOPageInfoSetHeight(LOPageInfoRef pageinfo, double height);

/**
@brief Gets the left page margin.
@param[in]  pageinfo   The page info object.
@param[out] margin     The left page margin.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if margin is NULL
*/
LO_RESULT LOPageInfoGetLeftMargin(LOPageInfoRef pageinfo,
                                  double* margin);

/**
@brief Gets the right page margin.
@param[in]  pageinfo   The page info object.
@param[out] margin     The right page margin.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if margin is NULL
*/
LO_RESULT LOPageInfoGetRightMargin(LOPageInfoRef pageinfo,
                                   double* margin);

/**
@brief Gets the top page margin.
@param[in]  pageinfo   The page info object.
@param[out] margin     The top page margin.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if margin is NULL
*/
LO_RESULT LOPageInfoGetTopMargin(LOPageInfoRef pageinfo, double* margin);

/**
@brief Gets the bottom page margin.
@param[in]  pageinfo   The page info object.
@param[out] margin     The bottom page margin.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if margin is NULL
*/
LO_RESULT LOPageInfoGetBottomMargin(LOPageInfoRef pageinfo, double* margin);

/**
@brief Sets the left page margin.
@param[in] pageinfo The page info object.
@param[in] margin   The new left margin to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if margin is less than 0 or greater than the page
  width
*/
LO_RESULT LOPageInfoSetLeftMargin(LOPageInfoRef pageinfo, double margin);

/**
@brief Sets the right page margin.
@param[in] pageinfo The page info object.
@param[in] margin   The new right margin to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if margin is less than 0 or greater than the page
  width
*/
LO_RESULT LOPageInfoSetRightMargin(LOPageInfoRef pageinfo, double margin);

/**
@brief Sets the top page margin.
@param[in] pageinfo The page info object.
@param[in] margin   The new top margin to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if margin is less than 0 or greater than the page
  height
*/
LO_RESULT LOPageInfoSetTopMargin(LOPageInfoRef pageinfo, double margin);

/**
@brief Sets the bottom page margin.
@param[in] pageinfo The page info object.
@param[in] margin   The new bottom margin to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if margin is less than 0 or greater than the page
  height
*/
LO_RESULT LOPageInfoSetBottomMargin(LOPageInfoRef pageinfo, double margin);

/**
@brief Gets the page color.
@param[in]  pageinfo  The page info object.
@param[out] color     The page color.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if color is NULL
*/
LO_RESULT LOPageInfoGetPaperColor(LOPageInfoRef pageinfo,
                                  SUColor* color);

/**
@brief Sets the page color.
@param[in] pageinfo The page info object.
@param[in] color    The new page color.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if color is NULL
*/
LO_RESULT LOPageInfoSetPaperColor(LOPageInfoRef pageinfo, const SUColor* color);

/**
@brief Gets whether or not the page color should be printed.
@param[in]  pageinfo          The page info object.
@param[out] print_paper_color Whether or not the page color should be printed.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if print_paper_color is NULL
*/
LO_RESULT LOPageInfoGetPrintPaperColor(LOPageInfoRef pageinfo,
                                       bool* print_paper_color);

/**
@brief Sets whether or not the page color should be printed.
@param[in] pageinfo          The page info object.
@param[in] print_paper_color Whether or not the page color should be printed.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
*/
LO_RESULT LOPageInfoSetPrintPaperColor(LOPageInfoRef pageinfo,
                                       bool print_paper_color);

/**
@brief Gets the color of the margins.
@param[in]  pageinfo The page info object.
@param[out] color    The margin color.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if color is NULL
*/
LO_RESULT LOPageInfoGetMarginColor(LOPageInfoRef pageinfo,
                                   SUColor* color);

/**
@brief Sets the color of the margins.
@param[in] pageinfo The page info object.
@param[in] color    The new margin color to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
*/
LO_RESULT LOPageInfoSetMarginColor(LOPageInfoRef pageinfo, SUColor color);

/**
@brief Gets whether or not the margins are shown.
@param[in]  pageinfo     The page info object.
@param[out] show_margins Whether the margins are shown or not.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if show_margins is NULL
*/
LO_RESULT LOPageInfoGetShowMargins(LOPageInfoRef pageinfo,
                                   bool* show_margins);

/**
@brief Sets whether or not the margins are shown.
@param[in] pageinfo     The page info object.
@param[in] show_margins Whether the margins should be shown or not.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
*/
LO_RESULT LOPageInfoSetShowMargins(LOPageInfoRef pageinfo,
                                   bool show_margins);

/**
@brief Gets whether or not the margins should be printed.
@param[in]  pageinfo      The page info object.
@param[out] print_margins Whether the margins are printed or not.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if print_margins is NULL
*/
LO_RESULT LOPageInfoGetPrintMargins(LOPageInfoRef pageinfo,
                                    bool* print_margins);

/**
@brief Sets whether or not the margins should be printed.
@param[in] pageinfo      The page info object.
@param[in] print_margins Whether the margins should be printed or not.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
*/
LO_RESULT LOPageInfoSetPrintMargins(LOPageInfoRef pageinfo,
                                    bool print_margins);

/**
@brief Gets the value of the document's display resolution for images and raster
       models.
@param[in]  pageinfo   The page info object.
@param[out] resolution The resolution.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if resolution is NULL
*/
LO_RESULT LOPageInfoGetDisplayResolution(LOPageInfoRef pageinfo,
                                         LOImageResolution* resolution);

/**
@brief Sets the value of the document's display resolution for images and raster
       models.
@param[in] pageinfo   The page info object.
@param[in] resolution The new resolution to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if the value of resolution is invalid
*/
LO_RESULT LOPageInfoSetDisplayResolution(LOPageInfoRef pageinfo,
                                         LOImageResolution resolution);

/**
@brief Gets the value of the document's output resolution for images and raster
       models.
@param[in]  pageinfo   The page info object.
@param[out] resolution The resolution.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if resolution is NULL
*/
LO_RESULT LOPageInfoGetOutputResolution(LOPageInfoRef pageinfo,
                                        LOImageResolution* resolution);

/**
@brief Sets the value of the document's output resolution for images and raster
       models.
@param[in] pageinfo   The page info object.
@param[in] resolution The new resolution to use.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if pageinfo does not refer to a valid object
- \ref SU_ERROR_OUT_OF_RANGE if the value of resolution is invalid
*/
LO_RESULT LOPageInfoSetOutputResolution(LOPageInfoRef pageinfo,
                                        LOImageResolution resolution);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // LAYOUT_MODEL_PAGEINFO_H_