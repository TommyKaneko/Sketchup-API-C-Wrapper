// Copyright 2016 Trimble Navigation Ltd. All Rights Reserved

#ifndef SKETCHUP_MODEL_CLASSIFICATION_INFO_H_
#define SKETCHUP_MODEL_CLASSIFICATION_INFO_H_

#include <SketchUpAPI/common.h>
#include <SketchUpAPI/unicodestring.h>
#include <SketchUpAPI/model/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
@struct SUClassificationInfoRef
@brief  References an object with classification information. Each
        SUClassificationInfoRef contains the names of the schemas and the schema
        types, and the types attributes. See \ref
        SUClassificationAttributeRef for details on the type attributes.
*/

/**
@brief Releases the classification info. Classification info objects are created
       from component instance using \ref
       SUComponentInstanceCreateClassificationInfo, and must be released using
       this function. This function also invalidates the given
       SUClassificationInfoRef.
@since SketchUp 2017, API 5.0
@param[in,out]  classification_info The classification info object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_INPUT if classification_info is NULL
- \ref SU_ERROR_INVALID_INPUT if classification_info is not a valid object
*/
SU_RESULT SUClassificationInfoRelease(
    SUClassificationInfoRef* classification_info);

/**
@brief Retrieves the number of schemas that have been applied to the
       component instance.
@since SketchUp 2017, API 5.0
@param[in]  classification_info The classification info object.
@param[out] count               The number of classifications.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if classification_info is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUClassificationInfoGetNumSchemas(
    SUClassificationInfoRef classification_info,
    size_t* count);

/**
@brief Retrieves the schema name for the classification at the given index.
@param[in]  classification_info The classification info object.
@param[in]  index               The classification index.
@param[out] schema_name         The name of the schema.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if classification_info is an invalid object
- \ref SU_ERROR_OUT_OF_RANGE if index is larger than the number of schemas
- \ref SU_ERROR_NULL_POINTER_OUTPUT if schema_name is NULL
- \ref SU_ERROR_INVALID_OUTPUT if *schema_name is not a valid object
*/
SU_RESULT SUClassificationInfoGetSchemaName(
    SUClassificationInfoRef classification_info,
    size_t index,
    SUStringRef* schema_name);

/**
@brief Retrieves the schema type for the classification at the given index.
@param[in]  classification_info The classification info object.
@param[in]  index               The classification index.
@param[out] schema_type         The applied type from the schema.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if classification_info is an invalid object
- \ref SU_ERROR_OUT_OF_RANGE if index is larger than the number of schemas
- \ref SU_ERROR_NULL_POINTER_OUTPUT if schema_type is NULL
- \ref SU_ERROR_INVALID_OUTPUT if *schema_type is not a valid object
*/
SU_RESULT SUClassificationInfoGetSchemaType(
    SUClassificationInfoRef classification_info,
    size_t index,
    SUStringRef* schema_type);

/**
@brief Retrieves the classification attribute for the classification at the
       given index.
@param[in]  classification_info The classification info object.
@param[in]  index               The classification index.
@param[out] attribute           The attribute retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if classification_info is an invalid object
- \ref SU_ERROR_OUT_OF_RANGE if index is larger than the number of schemas
- \ref SU_ERROR_NULL_POINTER_OUTPUT if attribute is NULL
*/
SU_RESULT SUClassificationInfoGetSchemaAttribute(
    SUClassificationInfoRef classification_info,
    size_t index,
    SUClassificationAttributeRef* attribute);

/**
@brief Retrieves the classification attribute with the given path.
@param[in]  classification_info The classification info object.
@param[in]  path                The path of the classification attribute to get.
@param[out] attribute           The attribute retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if classification_info is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if attribute is NULL
*/
SU_RESULT SUClassificationInfoGetSchemaAttributeByPath(
    SUClassificationInfoRef classification_info,
    SUStringRef path,
    SUClassificationAttributeRef* attribute);

#ifdef __cplusplus
}  //  extern "C" {
#endif

#endif // SKETCHUP_MODEL_CLASSIFICATION_INFO_H_
