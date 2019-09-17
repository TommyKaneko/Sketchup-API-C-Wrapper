// Copyright 2018 Trimble Inc. All Rights Reserverd.

#ifndef SKETCHUP_SOURCE_SKORE_SKETCHUP_PUBLIC_MODEL_LINESTYLE_H_
#define SKETCHUP_SOURCE_SKORE_SKETCHUP_PUBLIC_MODEL_LINESTYLE_H_

#include <SketchUpAPI/common.h>
#include <SketchUpAPI/model/defs.h>

#pragma pack(push, 8)
#ifdef __cplusplus
extern "C" {
#endif

/**
@brief Retrieves the name of a line style object.
@since SketchUp 2019, API 7.0
@param[in]  line_style The line style object.
@param[out] name       The name retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if line_style is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if name is NULL
- \ref SU_ERROR_INVALID_OUTPUT if name does not point to a valid \ref
       SUStringRef object
*/
SU_RESULT SULineStyleGetName(SULineStyleRef line_style, SUStringRef* name);
#ifdef __cplusplus
}
#endif
#pragma pack(pop)

#endif // SKETCHUP_SOURCE_SKORE_SKETCHUP_PUBLIC_MODEL_LINESTYLE_H_
