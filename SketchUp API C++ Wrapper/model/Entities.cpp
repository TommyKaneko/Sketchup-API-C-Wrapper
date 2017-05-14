//
//  Entities.cpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <cassert>

#include "Entities.hpp"

#include "GeometryInput.hpp"
#include "Vertex.hpp"
#include "Loop.hpp"
#include "Transformation.hpp"
#include "ComponentDefinition.hpp"
#include "ComponentInstance.hpp"
#include "Group.hpp"
#include "Face.hpp"
#include "Edge.hpp"

namespace CW {


Entities::Entities(SUEntitiesRef entities):
	m_entities(entities)
{
}

Entities::Entities():
	m_entities(SU_INVALID)
{}


std::vector<Face> Entities::faces() const {
	size_t count = 0;
	SU_RESULT res = SUEntitiesGetNumFaces(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  SUFaceRef face_refs[count];
	res = SUEntitiesGetFaces(m_entities, count, &face_refs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Face> faces;
  faces.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	faces.push_back(Face(face_refs[i]));
  }
  return faces;
}


std::vector<Edge> Entities::edges(bool stray_only) const {
	size_t count = 0;
	SU_RESULT res = SUEntitiesGetNumEdges(m_entities, stray_only, &count);
  assert(res == SU_ERROR_NONE);
  SUEdgeRef edge_refs[count];
	res = SUEntitiesGetEdges(m_entities, stray_only, count, &edge_refs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Edge> edges;
  edges.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	edges.push_back(Edge(edge_refs[i]));
  }
  return edges;
}


std::vector<ComponentInstance> Entities::instances() const {
	size_t count = 0;
	SU_RESULT res = SUEntitiesGetNumInstances(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  SUComponentInstanceRef instance_refs[count];
	res = SUEntitiesGetInstances(m_entities, count, &instance_refs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<ComponentInstance> instances;
  instances.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	instances.push_back(ComponentInstance(instance_refs[i]));
  }
  return instances;
}


std::vector<Group> Entities::groups() const {
	size_t count = 0;
	SU_RESULT res = SUEntitiesGetNumGroups(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Group>(0);
  }
  std::vector<SUGroupRef> group_refs(count);
	res = SUEntitiesGetGroups(m_entities, count, &group_refs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Group> groups;
  groups.reserve(count);
  for (size_t i=0; i < group_refs.size(); ++i) {
  	groups.push_back(Group(group_refs[i], true));
  }
  return groups;
}


size_t Entities::size() const {
	size_t total_count = 0;
  size_t count = 0;
	SU_RESULT res = SUEntitiesGetNumFaces(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  total_count += count;
  count = 0;
	res = SUEntitiesGetNumEdges(m_entities, true, &count);
  assert(res == SU_ERROR_NONE);
  total_count += count;
  count = 0;
	res = SUEntitiesGetNumInstances(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  total_count += count;
  count = 0;
	res = SUEntitiesGetNumGroups(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  total_count += count;
	return total_count;
}


void Entities::add(const Entities& other) {
	GeometryInput geom_input;
  geom_input.add_faces(other.faces());
  geom_input.add_edges(other.edges());
	this->fill(geom_input);
  std::vector<ComponentInstance> instances = other.instances();
  for (size_t i=0; i < instances.size(); ++i) {
  	this->add_instance(instances[i].definition(), instances[i].transformation(), instances[i].name());
  }
  std::vector<Group> other_groups = other.groups();
  for (size_t i=0; i < other_groups.size(); ++i) {
  	Group new_group = this->add_group();
    new_group.entities().add(other_groups[i].entities());
    new_group.transformation(other_groups[i].transformation());
    new_group.name(other_groups[i].name());
  }
  // TODO: other geometry types need to be added.
}



SU_RESULT Entities::fill(GeometryInput &geom_input) {
	// Check geom_input is not empty.
  if (geom_input.empty()) {
  	return SU_ERROR_NONE;
  }
  
  // For the indexes of the GeometryInputRef to make sense after we fill the Entities object with its contents, we need to know how many of each entitity currently exists in the Entities object
  size_t num_faces_before = 0;
  SU_RESULT res = SUEntitiesGetNumFaces(m_entities, &num_faces_before);
  assert(res == SU_ERROR_NONE);
  
  SU_RESULT fill_res = SUEntitiesFill(m_entities, geom_input.m_geometry_input, true);
  assert(fill_res == SU_ERROR_NONE);
  
  // Now add other data that SUEntitiesFill cannot add to the entities.
  // Apply properties to Faces
  // TODO: there is an assumption that the faces added to an Entities object is added in sequence, according to the index number.  So that (num_faces_before + face_index_of_geom_input) correspond to the Face number in the Entities object. This needs to be tested.
  size_t num_faces_after = 0;
  res = SUEntitiesGetNumFaces(m_entities, &num_faces_after);
  assert(res == SU_ERROR_NONE);
  /*
  std::vector<std::pair<size_t, Face>> faces_to_add = geom_input.faces();
  // If all of the faces in the geom_input were not added, it will not be possible to find the added face by looking at its index.
  // TODO: it is clear that if two overlapping faces are put into GeometryInput object, when output into the Entities object, the result could be three or more faces.  The following code will not recognise this. This shortcoming needs to be overcome.
  if ((num_faces_after-num_faces_before) == geom_input.num_faces()) {
  	
    SUFaceRef faces_after[num_faces_after];
    res = SUEntitiesGetFaces(m_entities, num_faces_after, &faces_after[0], &num_faces_after);
    assert(res == SU_ERROR_NONE);
    for (size_t i=0; i < faces_to_add.size(); ++i) {
      size_t new_face_index = faces_to_add[i].first;
      Face new_face(faces_after[(num_faces_before+new_face_index-1)]);
      new_face.copy_properties_from(faces_to_add[i].second);
      new_face.copy_attributes_from(faces_to_add[i].second);
      // Set attributes for the edges that bound the face.
      std::vector<Loop> loops = faces_to_add[i].second.loops();
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
  */
  return fill_res;
}

std::vector<Face> Entities::add_faces(std::vector<Face>& faces) {
  SU_RESULT res = SUEntitiesAddFaces(m_entities, faces.size(), faces[0]);
	assert(res == SU_ERROR_NONE);
  return faces;
}

std::vector<Edge> Entities::add_edges(std::vector<Edge>& edges) {
  SU_RESULT res = SUEntitiesAddEdges(m_entities, edges.size(), edges[0]);
  assert(res == SU_ERROR_NONE);
  return edges;
}


Edge Entities::add_edge(Edge& edge) {
	SUEdgeRef edge_ref = edge.ref();
  SU_RESULT res = SUEntitiesAddEdges(m_entities, 1, &edge_ref);
  assert(res == SU_ERROR_NONE);
  edge.attached(true);
  return edge;
}


void Entities::add_instance(ComponentInstance& instance) {
  SU_RESULT res = SUEntitiesAddInstance(m_entities, instance, nullptr);
	assert(res == SU_ERROR_NONE);
  instance.attached(true);
}

  
ComponentInstance Entities::add_instance(const ComponentDefinition& definition, const Transformation& transformation, const String& name){
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
  return ComponentInstance(instance, true);
}


// TODO: add_group needs to be refined

Group Entities::add_group(const ComponentDefinition& definition, const Transformation& transformation) {
  // Groups cannot be created with a component definition and transformation objects.  Instead, the geometry must be copied in to a new Entities object in the group.
  Group new_group;
  // Add group to the entities object before populating it.
  SU_RESULT res = SUEntitiesAddGroup(m_entities, new_group);
	assert(res == SU_ERROR_NONE);
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
  	group_entities.add_group(def_groups[i].definition() , def_groups[i].transformation());
  }
  std::vector<ComponentInstance> def_instances = def_entities.instances();
  for (size_t i=0; i < def_instances.size(); ++i) {
  	group_entities.add_instance(def_instances[i].definition(), def_instances[i].transformation());
  }
  
  // TODO: add other entities to the group (construction lines, curves, etc)
  return new_group;
  
  // TODO: the way groups are implemented are a problem.  Come back to this.
  //SU_RESULT res = SUComponentDefinitionCreateGroup(definition.ref(), &group);
	//assert(res == SU_ERROR_NONE);
  //SUTransformation transform = transformation.ref();
  //res = SUComponentInstanceSetTransform(instance, &transform);
}

Group Entities::add_group() {
  Group new_group;
  // Add group to the entities object before populating it.
  SU_RESULT res = SUEntitiesAddGroup(m_entities, new_group.ref());
	assert(res == SU_ERROR_NONE);
  return new_group;
}


bool Entities::transform_entities(std::vector<Entity>& elems, const Transformation& transform) {
	SUTransformation trans_ref = transform.ref();
	SU_RESULT res = SUEntitiesTransform(m_entities, elems.size(), elems[0], &trans_ref);
  assert(res == SU_ERROR_NONE || res == SU_ERROR_GENERIC);
  if (SU_ERROR_GENERIC) {
  	return false;
  }
  return true;
}


bool Entities::transform_entities(std::vector<Entity>& elems, std::vector<Transformation>& transforms) {
  assert(elems.size() == transforms.size());
  SU_RESULT res = SUEntitiesTransformMultiple(m_entities, elems.size(), elems[0], transforms[0]);
  assert(res == SU_ERROR_NONE || res == SU_ERROR_GENERIC);
  if (SU_ERROR_GENERIC) {
  	return false;
  }
  return true;
}




Entities::operator SUEntitiesRef() {
	return m_entities;
}

} /* namespace CW */
