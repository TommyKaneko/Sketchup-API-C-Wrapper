// Copyright 2014 Trimble Navigation Ltd. All Rights Reserved.

#ifndef LAYOUT_APPLICATION_APPLICATION_H_
#define LAYOUT_APPLICATION_APPLICATION_H_

#include <LayOutAPI/common.h>

/**
 * @struct LOApplicationRef
 * @brief Encapsulates a LayOut application.
 */

DEFINE_SU_TYPE(LOApplicationRef)

#include <LayOutAPI/model/document.h>

#pragma pack(push, 8)
#ifdef __cplusplus
  extern "C" {
#endif  // __cplusplus

/**
@struct LONamedScale
@brief Represents a scale and its display name.
*/
struct LONamedScale {
  double scale;
  SUStringRef name;
};


/**
@brief Gets the application instance.
@param[out] app The application instance.
@return
- \ref SU_ERROR_NO_DATA if we are not running inside an instance of the
  LayOut application
- \ref SU_ERROR_NONE on success
 */
LO_RESULT LOApplicationGetApplication(LOApplicationRef* app);

/**
@brief Gets the number of named scales.
@param[in] app The application instance.
@param[out] out_number_of_scales The number of named scales.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if out_number_of_scales is NULL
- \ref SU_ERROR_INVALID_INPUT if app is not a valid object
*/
LO_RESULT LOApplicationGetNumberOfNamedScales(LOApplicationRef app,
    size_t* out_number_of_scales);

/**
@brief Gets all the named scales.
@return
- \ref SU_ERROR_NONE on success.
- \ref SU_ERROR_INVALID_INPUT if app is not a valid object
- \ref SU_ERROR_OUT_OF_RANGE if scales_array_length must be at least 1
- \ref SU_ERROR_NULL_POINTER_OUTPUT if out_scales_array or
  out_number_of_scales_copied is NULL
- \ref SU_ERROR_OVERWRITE_VALID if out_scales_array contains one or more valid
objects
*/
LO_RESULT LOApplicationGetNamedScalesArray(LOApplicationRef app,
                                  size_t scales_array_length,
                                  LONamedScale* out_scales_array,
                                  size_t* out_number_of_scales_copied);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOApplicationGetActiveDocument(LOApplicationRef app,
                                         LODocumentRef* doc);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOApplicationGetName(LOApplicationRef app, SUStringRef* app_name);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOApplicationGetVersion(LOApplicationRef app, SUStringRef* version);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOApplicationGetVersionNumber(LOApplicationRef app,
                                        unsigned int* version);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOApplicationOpenFile(LOApplicationRef app, const char* file_path);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOApplicationQuit(LOApplicationRef app);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
    
/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOApplicationGetIsOnline(LOApplicationRef app, bool* is_online);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */

#ifdef __cplusplus
}  // extern "C" {
#endif
#pragma pack(pop)

#endif // LAYOUT_APPLICATION_APPLICATION_H_
