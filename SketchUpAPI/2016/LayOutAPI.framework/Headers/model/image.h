// Copyright 2015 Trimble Navigation Ltd. All rights reserved.
// This file is intended for public distribution.

#ifndef LAYOUT_MODEL_IMAGE_H_
#define LAYOUT_MODEL_IMAGE_H_

#include <LayOutAPI/common.h>

/**
@struct LOImageRef
@brief References a raster image entity.
*/
DEFINE_SU_TYPE(LOImageRef);

#include <LayOutAPI/model/entity.h>
#include <LayOutAPI/model/imagerep.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
@brief Creates a new image object from an image file on disk.
@param[out] image     The image object.
@param[in]  bounds    The bounding box for the image.
@param[in]  file_path The file path to the image on disk.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if image is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *image already refers to a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if bounds is NULL
- \ref SU_ERROR_NULL_POINTER_INPUT if file_path is NULL
- \ref SU_ERROR_OUT_OF_RANGE if bounds has a width or height of zero
- \ref SU_ERROR_SERIALIZATION if there was an error reading the image file or
  allocating the memory for the image
*/
LO_RESULT LOImageCreateFromFile(LOImageRef* image,
                                const LOAxisAlignedRect2D* bounds,
                                const char* file_path);

/**
@brief Adds a reference to an image object.
@param[in] image The image object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if image does not refer to a valid object
*/
LO_RESULT LOImageAddReference(LOImageRef image);

/**
@brief Releases an image object. The object will be invalidated if
       releasing the last reference.
@param[in] image The image object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_INPUT if image is NULL
- \ref SU_ERROR_INVALID_INPUT if *image does not refer to a valid object
*/
LO_RESULT LOImageRelease(LOImageRef* image);

/**
@brief Converts from a \ref LOEntityRef to a \ref LOImageRef.
       This is essentially a downcast operation so the given \ref LOEntityRef
       must be convertible to a \ref LOImageRef.
@param[in] entity The entity object.
@return
- The converted \ref LOImageRef if the downcast operation succeeds
- If not, the returned reference will be invalid
*/
LO_EXPORT LOImageRef LOImageFromEntity(LOEntityRef entity);

/**
@brief Converts from a \ref LOImageRef to a \ref LOEntityRef.
       This is essentially an upcast operation.
@param[in] image The image object.
@return
- The converted \ref LOEntityRef if image is a valid object
- If not, the returned reference will be invalid
*/
LO_EXPORT LOEntityRef LOImageToEntity(LOImageRef image);

/**
@brief Gets an image object's image representation.
@param[in]  image    The image object.
@param[out] imagerep The image representation object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if image does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if imagerep is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *imagerep already refers to a valid object
*/
LO_RESULT LOImageGetImageRep(LOImageRef image, LOImageRepRef* imagerep);

#ifdef __cplusplus
}  // end extern "C"
#endif  // __cplusplus
#endif  // LAYOUT_MODEL_IMAGE_H_
