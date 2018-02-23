// Copyright 2013 Trimble Navigation Ltd. All Rights Reserved.
#ifndef SKETCHUP_MODEL_DRAWINGELEMENT_H_
#define SKETCHUP_MODEL_DRAWINGELEMENT_H_

#include <SketchUpAPI/common.h>
#include <SketchUpAPI/model/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
@struct SUDrawingElementRef
@brief  References a drawing element, which is an abstract base type for some
        API types.
*/

/**
@brief Converts from an SUDrawingElementRef to an SUEntityRef.
       This is essentially an upcast operation.
@param[in] elem The given drawing element reference.
@return
- The converted SUEntityRef if elem is a valid drawing element.
- If not, the returned reference will be invalid.
*/
SU_EXPORT SUEntityRef SUDrawingElementToEntity(SUDrawingElementRef elem);

/**
@brief Converts from an SUEntityRef to an SUDrawingElementRef.
       This is essentially a downcast operation so the given entity must be
       convertible to a drawing element.
@param[in] entity The given entity reference.
@return
- The converted SUDrawingElementRef if the downcast operation succeeds.
- If not, the returned reference will be invalid.
*/
SU_EXPORT SUDrawingElementRef SUDrawingElementFromEntity(SUEntityRef entity);

/**
@brief Returns the concrete type of the given drawing element.
@param[in] elem The drawing element.
@return
- The concrete type of the given drawing element reference.
- \ref SURefType_Unknown if entity is not a valid drawing element.
*/
SU_EXPORT enum SURefType SUDrawingElementGetType(SUDrawingElementRef elem);

/**
@brief Retrieves the bounding box of a drawing element.
@param[in]  elem The drawing element.
@param[out] bbox The bounding box retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if bbox is NULL
*/
SU_RESULT SUDrawingElementGetBoundingBox(SUDrawingElementRef elem,
                                         struct SUBoundingBox3D* bbox);

/**
@brief Retrieves the material object of a drawing element.

The material object must not be subsequently deallocated while still associated
with the drawing element.
@param[in]  elem     The drawing element.
@param[out] material The drawing element retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if material is NULL
- \ref SU_ERROR_NO_DATA if the drawing element does not have a front face
       material
*/
SU_RESULT SUDrawingElementGetMaterial(SUDrawingElementRef elem,
                                      SUMaterialRef* material);

/**
@brief Sets the material of a drawing element.

The material object must not be subsequently deallocated while associated with
the drawing element.
@param[in] elem     The drawing element.
@param[in] material The material object to set. If an invalid reference is
                    given, then the material of the element will be removed.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is invalid
*/
SU_RESULT SUDrawingElementSetMaterial(SUDrawingElementRef elem,
                                      SUMaterialRef material);

/**
@brief Retrieves the layer object associated with a drawing element.
@param[in]  elem  The drawing element.
@param[out] layer The layer object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if layer is NULL
*/
SU_RESULT SUDrawingElementGetLayer(SUDrawingElementRef elem, SULayerRef* layer);

/**
@brief Sets the layer object to be associated with a drawing element.
@param[in] elem  The drawing element.
@param[in] layer The layer object to set.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem or layer are invalid objects
*/
SU_RESULT SUDrawingElementSetLayer(SUDrawingElementRef elem, SULayerRef layer);

/**
@brief Sets the hide flag of a drawing element.
@param[in] elem      The drawing element.
@param[in] hide_flag The hide flag to set.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
*/
SU_RESULT SUDrawingElementSetHidden(SUDrawingElementRef elem,
                                    bool hide_flag);

/**
@brief Retrieves the hide flag of a drawing element.
@param[in]  elem      The drawing element.
@param[out] hide_flag The hide flag retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if hide_flag is NULL
*/
SU_RESULT SUDrawingElementGetHidden(SUDrawingElementRef elem,
                                    bool* hide_flag);

/**
@brief Sets the casts shadows flag of a drawing element.
@param[in] elem               The drawing element.
@param[in] casts_shadows_flag The casts shadows flag to set.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
*/
SU_RESULT SUDrawingElementSetCastsShadows(SUDrawingElementRef elem,
                                          bool casts_shadows_flag);

/**
@brief Retrieves the casts shadows flag of a drawing element.
@param[in]  elem               The drawing element.
@param[out] casts_shadows_flag The casts shadows flag retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if hide_flag is NULL
*/
SU_RESULT SUDrawingElementGetCastsShadows(SUDrawingElementRef elem,
                                          bool* casts_shadows_flag);


/**
@brief Sets the receives shadows flag of a drawing element.
@param[in] elem                  The drawing element.
@param[in] receives_shadows_flag The casts shadows flag to set.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
*/
SU_RESULT SUDrawingElementSetReceivesShadows(SUDrawingElementRef elem,
                                             bool receives_shadows_flag);

/**
@brief Retrieves the receives shadows flag of a drawing element.
@param[in]  elem                  The drawing element.
@param[out] receives_shadows_flag The casts shadows flag retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if elem is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if hide_flag is NULL
*/
SU_RESULT SUDrawingElementGetReceivesShadows(SUDrawingElementRef elem,
                                             bool* receives_shadows_flag);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SKETCHUP_MODEL_DRAWINGELEMENT_H_
