// Copyright 2017 Trimble Inc. All Rights Reserved.
#ifndef SKETCHUP_MODEL_ENTITY_H_
#define SKETCHUP_MODEL_ENTITY_H_

#include <SketchUpAPI/common.h>
#include <SketchUpAPI/model/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
@struct SUEntityRef
@brief  References an entity, which is an abstract base type for most API types.
*/

/**
@brief Returns the concrete type of the given entity.
@param[in] entity The entity.
@return
- The concrete type of the given entity reference.
- \ref SURefType_Unknown if entity is not valid.
*/
SU_EXPORT enum SURefType SUEntityGetType(SUEntityRef entity);

/**
@brief Retrieves the id of the entity.
@param[in]  entity    The entity.
@param[out] entity_id The id retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if entity_id is NULL.
*/
SU_RESULT SUEntityGetID(SUEntityRef entity, int32_t* entity_id);

/**
@brief Retrieves the persistent id of the entity.

@note Only a subset of entity types support PIDs. Refer to the list
      below for which and when support was added.

SketchUp 2018
  - SUSceneRef
SketchUp 2017
  - SUAxesRef
  - SUComponentDefinitionRef
  - SUComponentInstanceRef
  - SUGuideLineRef
  - SUGuidePointRef
  - SUCurveRef
  - SUDimensionRef
  - SUEdgeRef
  - SUFaceRef
  - SUPolyline3dRef
  - SUSectionPlaneRef
  - SUTextRef
  - SUVertexRef

@param[in]  entity      The entity.
@param[out] entity_pid  The persistent id retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if entity_pid is NULL.
*/
SU_RESULT SUEntityGetPersistentID(SUEntityRef entity, int64_t* entity_pid);

/**
@brief Retrieves the number of attribute dictionaries of an entity.
@param[in]  entity The entity.
@param[out] count  The number of attribute dictionaries.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity is not a valid entity
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUEntityGetNumAttributeDictionaries(SUEntityRef entity,
                                              size_t* count);

/**
@brief Retrieves the attribute dictionaries of an entity.
@param[in]  entity       The entity.
@param[in]  len          The number of attribute dictionaries to retrieve.
@param[out] dictionaries The dictionaries retrieved.
@param[out] count        The number of dictionaries retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity is not a valid entity
- \ref SU_ERROR_NULL_POINTER_OUTPUT if dictionaries or count is NULL
*/
SU_RESULT SUEntityGetAttributeDictionaries(
    SUEntityRef entity,
    size_t len,
    SUAttributeDictionaryRef dictionaries[],
    size_t* count);

/**
@brief Adds the attribute dictionary to an entity.
@since SketchUp 2018 M0, API 6.0
@param[in] entity     The entity.
@param[in] dictionary The dictionary object to be added. If the function is 
                      successful, don't call SUAttributeDictionaryRelease on the
                      dictionary because the new entity will take ownership.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity or dictionary are not valid entities
- \ref SU_ERROR_DUPLICATE if another attribute already exists with the
       same name.
- \ref SU_ERROR_INVALID_ARGUMENT if dictionary's name is empty.
*/
SU_RESULT SUEntityAddAttributeDictionary(
    SUEntityRef entity,
    SUAttributeDictionaryRef dictionary);

/**
@brief Retrieves the attribute dictionary of an entity that has the given name.

If a dictionary with the given name does not exist, one is added to the entity.
@param[in]  entity The entity.
@param[in]  name   The name of the retrieved attribute dictionary.
                   Assumed to be UTF-8 encoded.
@param[out] dictionary The destination of the retrieved dictionary object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity is not a valid entity
- \ref SU_ERROR_NULL_POINTER_INPUT if name is NULL
- \ref SU_ERROR_NULL_POINTER_OUTPUT if dictionary is NULL
*/
SU_RESULT SUEntityGetAttributeDictionary(
    SUEntityRef entity,
    const char* name,
    SUAttributeDictionaryRef* dictionary);

/**
@brief Retrieves the model object associated with the entity.
@since SketchUp 2018 M0, API 6.0
@param[in]  entity The entity.
@param[out] model  The model object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if model is NULL
- \ref SU_ERROR_NO_DATA if the entity is not associated with a model
*/
SU_RESULT SUEntityGetModel(SUEntityRef entity, SUModelRef* model);

/**
@brief Retrieves the entities object which contains the entity.
@since SketchUp 2018 M0, API 6.0
@param[in]  entity   The entity.
@param[out] entities The entities object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if entity is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if entities is NULL
- \ref SU_ERROR_NO_DATA if the entity is not contained by an entities object
*/
SU_RESULT SUEntityGetParentEntities(SUEntityRef entity,
    SUEntitiesRef* entities);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SKETCHUP_MODEL_ENTITY_H_
