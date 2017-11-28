// Copyright 2013 Trimble Navigation Ltd. All Rights Reserved.

#ifndef LAYOUT_APPLICATION_COMMAND_H_
#define LAYOUT_APPLICATION_COMMAND_H_

#include <LayOutAPI/common.h>

/**
@struct LOCommandRef
@brief References a command in LayOut.
*/
DEFINE_SU_TYPE(LOCommandRef)

#ifdef __cplusplus
  extern "C" {
#endif  // __cplusplus

/**
@brief Creates a reference to a command with the given text.
@param[out] command  The command object.
@param[in]  cmd_text The name of the command.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if command is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *command already refers to a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if cmd_text is NULL
*/
LO_RESULT LOCommandCreate(LOCommandRef* command, const char* cmd_text);

/**
@brief Releases a command object. The object will be invalidated upon releasing.
@param[in] command The command object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_INPUT if command is NULL
- \ref SU_ERROR_INVALID_INPUT if *command does not refer to a valid object
*/
LO_RESULT LOCommandRelease(LOCommandRef* command);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandGetText(LOCommandRef command, SUStringRef* cmd_text);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandSetText(LOCommandRef command, const char* cmd_text);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandGetSmallIcon(LOCommandRef command, SUStringRef* file_path);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandSetSmallIcon(LOCommandRef command, const char* file_path);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandGetLargeIcon(LOCommandRef command, SUStringRef* file_path);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandSetLargeIcon(LOCommandRef command, const char* file_path);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandGetTooltip(LOCommandRef command, SUStringRef* tool_tip);

/**
 * @brief
 *
 *
 *
 *
 * @return
 * - SU_ERROR_NONE : Success
 */
LO_RESULT LOCommandSetTooltip(LOCommandRef command, const char* tool_tip);

#ifdef __cplusplus
}  // extern "C" {
#endif

#endif //LAYOUT_APPLICATION_COMMAND_H_
