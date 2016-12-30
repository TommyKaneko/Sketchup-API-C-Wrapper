//
//  Entities.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//
#include <cassert>

#include "Entities.hpp"
#include "GeometryInput.hpp"
#include "Transformation.hpp"
#include "ComponentDefinition.hpp"
#include "ComponentInstance.hpp"
#include "Group.hpp"

namespace CW {

Entities::Entities(SUEntitiesRef entities):
	m_entities(entities)
{
}

SU_RESULT Entities::fill(GeometryInput &geom_input) {
	// For the indexes of the GeometryInputRef to make sense after we fill the Entities object with its contents, we need to know how many of each entitity currently exists in the Entities object
  size_t num_faces_before = 0;
  SU_RESULT res = SUEntitiesGetNumFaces(m_entities, &num_faces_before);
  assert(res == SU_ERROR_NONE);
  
  SU_RESULT fill_res = SUEntitiesFill(m_entities, geom_input.input_ref(), true);
  assert(res == SU_ERROR_NONE);
  
  // Now add other data that SUEntitiesFill cannot add to the entities.
  // Apply properties to Faces
  // TODO: there is an assumption that the faces added to an Entities object is added in sequence, according to the index number.  So that (num_faces_before + face_index_of_geom_input) correspond to the Face number in the Entities object. This needs to be tested.
  size_t num_faces_after = 0;
  res = SUEntitiesGetNumFaces(m_entities, &num_faces_after);
  assert(res == SU_ERROR_NONE);
  std::vector<Face> faces_to_add = geom_input.faces();
  // If all of the faces in the geom_input were not added, it will not be possible to find the added face by looking at its index.
  // TODO: it is clear that if two overlapping faces are put into GeometryInput object, when output into the Entities object, the result could be three or more faces.  The following code will not recognise this. This shortcoming needs to be overcome.
  if ((num_faces_after-num_faces_before) == faces_to_add.size()) {
  	SUFaceRef faces_after[num_faces_after];
  	res = SUEntitiesGetFaces(m_entities, num_faces_after, &faces_after[0], &num_faces_after);
  	assert(res == SU_ERROR_NONE);
    for (size_t i=0; i < faces_to_add.size(); ++i) {
    	Face new_face(faces_after[(num_faces_before+i-1)]);
      new_face.copy_properties_from(faces_to_add[i]);
      new_face.copy_attributes_from(faces_to_add[i]);
    	// Set attributes for the edges that bound the face.
      std::vector<Loop> loops = faces_to_add[i].loops();
      std::vector<Loop> new_loops = new_face.loops();
      
      for (size_t j=0; j < loops.size(); ++j) {
      	std::vector<Edge> old_edges = loops[j].edges();
      	std::vector<Edge> new_edges = new_loops[j].edges();
        // If there are more new edges, then it means that some edges have been split during a merging operation.  The trick to find these is to see if the end location of the new and old edges match.
        size_t new_edge_index = 0;
        size_t old_edge_index = 0;
        while( old_edge_index < old_edges.size()) {
          do {
            new_edges[new_edge_index].copy_properties_from(old_edges[old_edge_index]);
            // TODO: to save on a little performance, only a few of the DrawingElement properties need to be copied, as hidden and soft properties are already set from GeometryInput object.
            new_edges[new_edge_index].copy_attributes_from(old_edges[old_edge_index]);
            ++new_edge_index;
          }
          while ((old_edges.size() < new_edges.size()) &&
                 (new_edge_index < new_edges.size()) &&
                 (new_edges[new_edge_index-1].end().position() != old_edges[old_edge_index+1].end().position()));
          ++old_edge_index;
        }
      }
    }
  }
  return fill_res;
}

std::vector<Face> Entities::add_faces(std::vector<Face> faces) {
	size_t len = faces.size();
  SUFaceRef faces_array[len];
  for (size_t i = 0; i < len; ++i) {
  	faces_array[i] = faces[i].ref();
  }
  
  SU_RESULT res = SUEntitiesAddFaces(m_entities, len, &faces_array[0]);
	assert(res == SU_ERROR_NONE);
  return faces;
}

bool Entities::add_edges(const std::vector<Edge> edges) {
  SUEdgeRef edge_refs[edges.size()];
  for (size_t i=0; i < edges.size(); ++i) {
    edge_refs[i] = edges[i].ref();
  }
  SU_RESULT res = SUEntitiesAddEdges(m_entities, edges.size(), edge_refs);
  if(res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}

bool Entities::add_edge(const Edge edge) {
  return add_edges(std::vector<Edge>{edge});
}


ComponentInstance Entities::add_instance(const ComponentDefinition definition, const Transformation transformation, const String name){
  SUComponentInstanceRef instance = SU_INVALID;
  SU_RESULT res = SUComponentDefinitionCreateInstance(definition.ref(), &instance);
	assert(res == SU_ERROR_NONE);
  SUTransformation transform = transformation.ref();
  res = SUComponentInstanceSetTransform(instance, &transform);
  assert(res == SU_ERROR_NONE);
  if (name.empty()) {
    res = SUEntitiesAddInstance(m_entities, instance, NULL);
  }
  else {
    SUStringRef name_ref = name.ref();
    res = SUEntitiesAddInstance(m_entities, instance, &name_ref);
  }
	assert(res == SU_ERROR_NONE);
  return ComponentInstance(instance);
}


// TODO: add_group needs to be refined

Group Entities::add_group(ComponentDefinition definition, Transformation transformation) {
  // Groups cannot be created with a component definition and transformation objects.  Instead, the geometry must be copied in to a new Entities object in the group.
  Group new_group;
  Entities group_entities = new_group.entities();
  Entities def_entities = definition.entities();
  // Add geometry one by one to Geometry input.
  GeometryInput geom_input;
  std::vector<Face> def_faces = def_entities.faces();
  for (size_t i=0; i < def_faces.size(); ++i) {
  	geom_input.add_face(def_faces[i]);
  }
  std::vector<Edge> def_edges = def_entities.edges(true);
  for (size_t i=0; i < def_edges.size(); ++i) {
  	group_entities.add_edge(def_edges[i]);
  }
  group_entities.fill(geom_input);
  // Also add instances and groups
  std::vector<Group> def_groups = def_entities.groups();
  for (size_t i=0; i < def_groups.size(); ++i) {
  	group_entities.add_group(def_groups[i].definition(), def_groups[i].transformation());
  }
  std::vector<ComponentInstance> def_instances = def_entities.instances();
  for (size_t i=0; i < def_instances.size(); ++i) {
  	group_entities.add_instance(def_instances[i].definition(), def_instances[i].transformation());
  }
  
  // TODO: the way groups are implemented are a problem.  Come back to this.
  SU_RESULT res = SUComponentDefinitionCreateGroup(definition.ref(), &group);
	assert(res == SU_ERROR_NONE);
  SUTransformation transform = transformation.ref();
  res = SUComponentInstanceSetTransform(instance, &transform);
  SU_RESULT res = SUEntitiesAddGroup(m_entities, group );
}



Entities::operator SUEntitiesRef() {
	return m_entities;
}

} /* namespace CW */
