//
//  Entities.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//
#include <cassert>

#include "Entities.hpp"

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
          while (old_edges.size() < new_edges.size() &&
            new_edge_index < new_edges.size() &&
            Point3D(new_edges[new_edge_index-1].end()) != Point3D(old_edges[old_edge_index+1].end()));
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
  	faces_array[i] = faces[i].face_ref();
  }
  
  SU_RESULT res = SUEntitiesAddFaces(m_entities, len, &faces_array[0]);
	assert(res == SU_ERROR_NONE);
  return faces;
}



Entities::operator SUEntiesRef&() {
	return &m_entities;
}
Entities::operator SUEntiesRef() {
	return m_entities;
}

} /* namespace CW */
