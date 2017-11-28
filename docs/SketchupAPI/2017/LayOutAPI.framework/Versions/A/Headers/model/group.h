// Copyright 2015 Trimble Navigation Ltd. All rights reserved.
// This file is intended for public distribution.

#ifndef LAYOUT_MODEL_GROUP_H_
#define LAYOUT_MODEL_GROUP_H_

#include <LayOutAPI/common.h>

/**
@struct LOGroupRef
@brief References a group entity. A group is a special type of entity that
       does not belong to a layer and contains other entities as children. A
       group's children may include other groups, allowing for a hierarchical
       tree structure of entities. A group must contain at least two children
       and will be automatically collapsed if an operation is performed that
       results in the group containing less than two children.
*/
DEFINE_SU_TYPE(LOGroupRef)

#include <LayOutAPI/model/entity.h>
#include <LayOutAPI/model/entitylist.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


/**
@brief Creates a new group object, populating it with the entities in the given
       \ref LOEntityListRef. It is possible to create a group from entities
       that are already in a document, as well as from entities that are not
       yet in a document. For entities that are already in a document, they can
       only be grouped if they all belong to shared layers, or all belong to
       non-shared layers on the same page. If the entities are in a document,
       then the new group will be added to the document at the top of the group
       hierarchy.
@param[out] group       The group object.
@param[in]  entity_list The list of entities to add to the group.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if group is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *group already refers to a valid object
- \ref SU_ERROR_INVALID_INPUT if entity_list does not refer to a valid object
- \ref SU_ERROR_GENERIC if entity_list contains fewer than two entities
- \ref SU_ERROR_GENERIC if entity_list contains entities that belong to
  different documents
- \ref SU_ERROR_GENERIC if entity_list contains a mix of entities that belong
  to a document and entities that don't belong to a document
- \ref SU_ERROR_GENERIC if entity_list contains entities on both shared
  and non-shared layers, or on non-shared layers belonging to different pages.
- \ref SU_ERROR_GENERIC if entity_list contains the same entity more than
  once.
*/
LO_RESULT LOGroupCreate(LOGroupRef* group, LOEntityListRef entity_list);

/**
@brief Adds a reference to a group object.
@param[in] group The group object.
@return
SU_ERROR_NONE on success
SU_ERROR_INVALID_INPUT if group does not refer to a valid object
*/
LO_RESULT LOGroupAddReference(LOGroupRef group);


/**
@brief Releases a group object. The object will be invalidated if
       releasing the last reference.
@param[in] group The group object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_INPUT if group is NULL
- \ref SU_ERROR_INVALID_INPUT if *group does not refer to a valid object
*/
LO_RESULT LOGroupRelease(LOGroupRef* group);

/**
@brief Converts from a \ref LOEntityRef to a \ref LOGroupRef.
       This is essentially a downcast operation so the given \ref LOEntityRef
       must be convertible to a \ref LOGroupRef.
@param[in] entity The entity object.
@return
- The converted \ref LOGroupRef if the downcast operation succeeds
- If not, the returned reference will be invalid
*/
LO_EXPORT LOGroupRef LOGroupFromEntity(LOEntityRef entity);

/**
@brief Converts from a \ref LOGroupRef to a \ref LOEntityRef.
       This is essentially an upcast operation.
@param[in] group The group object.
@return
- The converted \ref LOEntityRef if group is a valid object
- If not, the returned reference will be invalid
*/
LO_EXPORT LOEntityRef LOGroupToEntity(LOGroupRef group);

/**
@brief Removes all the entities from a group and removes the empty group. The
       entities will become children of the group's parent. *group will be set
       to invalid by this function.
@param[in] group The group object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_INPUT if group is NULL
- \ref SU_ERROR_INVALID_INPUT if *group does not refer to a valid object
*/
LO_RESULT LOGroupUngroup(LOGroupRef* group);

/**
@brief Populates a \ref LOEntityListRef object with all the children of a group.
@param[in] group       The group object.
@param[in] entity_list The entity list object to populate with the group's
                       children.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if group does not refer to a valid object
- \ref SU_ERROR_INVALID_INPUT if entity_list does not refer to a valid object
*/
LO_RESULT LOGroupGetEntities(LOGroupRef group,
                             LOEntityListRef entity_list);

/**
@brief Gets the number of children within a group.
@param[in]  group        The group object.
@param[out] num_entities The number of children in the group.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if group does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if num_entities is NULL
*/
LO_RESULT LOGroupGetNumberOfEntities(LOGroupRef group, size_t* num_entities);

/**
@brief Gets the child at the given index from within a group.
@param[in]  group        The group object.
@param[in]  index        The index of the child entity to get.
@param[out] child_entity The child entity object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if group does not refer to a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if child_entity is NULL
- \ref SU_ERROR_OVERWRITE_VALID if *child_entity already refers to a valid
  object
*/
LO_RESULT LOGroupGetEntityAtIndex(LOGroupRef group,
                                  size_t index,
                                  LOEntityRef* child_entity);

#ifdef __cplusplus
}  // end extern C
#endif  // __cplusplus

#endif  // LAYOUT_MODEL_GROUP_H_
