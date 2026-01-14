//
//  Entities.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include <cassert>

#include "SUAPI-CppWrapper/model/Entities.hpp"

#include "SUAPI-CppWrapper/model/GeometryInput.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"
#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"
#include "SUAPI-CppWrapper/model/Group.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"

namespace CW {

#if SketchUpAPI_VERSION_MAJOR < 2021
Entities::Entities(SUEntitiesRef entities, const SUModelRef model):
  Entities(entities, Model(model, false))
{}

Entities::Entities(SUEntitiesRef entities, const Model model):
  m_entities(entities),
  m_model(model)
{}

Entities::Entities():
  m_entities(SU_INVALID),
  m_model(Model())
{}
#else
Entities::Entities(SUEntitiesRef entities):
  m_entities(entities)
{}

Entities::Entities():
  m_entities(SU_INVALID)
{}
#endif

std::vector<Face> Entities::faces() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::faces(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumFaces(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Face>(0);
  }
  std::vector<SUFaceRef> face_refs(count, SU_INVALID);
  res = SUEntitiesGetFaces(m_entities, count, face_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Face> faces(count);
  std::transform(face_refs.begin(), face_refs.end(), faces.begin(),
  [](const SUFaceRef& value) {
    return Face(value);
  });
  return faces;
}


std::vector<Edge> Entities::edges(bool stray_only) const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::edges(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumEdges(m_entities, stray_only, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Edge>(0);
  }
  std::vector<SUEdgeRef> edge_refs(count, SU_INVALID);
  res = SUEntitiesGetEdges(m_entities, stray_only, count, edge_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Edge> edges(count);
  std::transform(edge_refs.begin(), edge_refs.end(), edges.begin(),
  [](const SUEdgeRef& value) {
    return Edge(value);
  });
  return edges;
}


std::vector<ComponentInstance> Entities::instances() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::instances(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumInstances(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<ComponentInstance>{};
  }
  std::vector<SUComponentInstanceRef> instance_refs(count, SU_INVALID);
  res = SUEntitiesGetInstances(m_entities, count, instance_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<ComponentInstance> instances(count);
  std::transform(instance_refs.begin(), instance_refs.end(), instances.begin(),
  [](const SUComponentInstanceRef& value) {
    return ComponentInstance(value);
  });;
  return instances;
}


std::vector<Group> Entities::groups() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::groups(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumGroups(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Group>(0);
  }
  std::vector<SUGroupRef> group_refs(count);
  res = SUEntitiesGetGroups(m_entities, count, &group_refs[0], &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Group> groups;
  groups.reserve(count);
  for (size_t i=0; i < group_refs.size(); ++i) {
    groups.push_back(Group(group_refs[i], true));
  }
  return groups;
}


BoundingBox3D Entities::bounding_box() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::groups(): Entities is null");
  }
  SUBoundingBox3D box;
  SUResult res = SUEntitiesGetBoundingBox(m_entities, &box);
  assert(res == SU_ERROR_NONE); _unused(res);
  return BoundingBox3D(box);
}


size_t Entities::size() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::size(): Entities is null");
  }
  size_t total_count = 0;
  size_t count = 0;
  SUResult res = SUEntitiesGetNumFaces(m_entities, &count);
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
  assert(res == SU_ERROR_NONE); _unused(res);
  total_count += count;
  return total_count;
}


void Entities::add(const Entities& other) {
  if (!SUIsValid(m_entities) || !SUIsValid(other.m_entities)) {
    throw std::logic_error("CW::Entities::add(): Entities is null");
  }
  Model model = this->model();
  GeometryInput geom_input(&model);
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



SUResult Entities::fill(GeometryInput &geom_input) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::fill(): Entities is null");
  }
  // Check geom_input is not empty.
  if (geom_input.empty()) {
    return SU_ERROR_NONE;
  }
  // if (geom_input.m_attached) {
  //   throw std::logic_error("CW::Entities::fill(): GeometryInput has already been attached to a model, cannot fill Entities object.");
  // }
  Model model = this->model();
  if (geom_input.m_target_model->ref().ptr != model.ref().ptr) {
    throw std::logic_error("CW::Entities::fill(): GeometryInput is associated with a different model than the Entities object.");
  }
  // For the indexes of the GeometryInputRef to make sense after we fill the Entities object with its contents, we need to know how many of each entity currently exists in the Entities object
  //size_t num_faces_before = 0;
  //SUResult res = SUEntitiesGetNumFaces(m_entities, &num_faces_before);
  //assert(res == SU_ERROR_NONE); _unused(res);

  SUResult fill_res = SUEntitiesFill(m_entities, geom_input.m_geometry_input, true);
  assert(fill_res == SU_ERROR_NONE); _unused(fill_res);
  return fill_res;
}

std::vector<Face> Entities::add_faces(std::vector<Face>& faces) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_faces(): Entities is null");
  }
  std::vector<SUFaceRef> refs(faces.size(), SU_INVALID);
  std::transform(faces.begin(), faces.end(), refs.begin(), [](const CW::Face& face) {return face.ref(); });

  SUResult res = SUEntitiesAddFaces(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);

  // Transfer ownership of each face
  for (auto& face : faces)
    face.attached(true);

  return faces;
}

std::vector<Edge> Entities::add_edges(std::vector<Edge>& edges) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_edges(): Entities is null");
  }
  std::vector<Edge> existing_edges = this->edges();
  std::vector<Edge> edges_to_add; edges_to_add.reserve(edges.size());
  for (Edge& edge : edges) {
    if (!edge) {
      continue;
    }
    // Check that each edge is not attached to another entities object
    if (edge.attached()) {
      edges_to_add.push_back(edge.copy());
    }
    else {
      edges_to_add.push_back(edge);
    }
  }

  std::vector<SUEdgeRef> refs(edges_to_add.size(), SU_INVALID);
  std::transform(edges_to_add.begin(), edges_to_add.end(), refs.begin(), [](const CW::Edge& edge) {return edge.ref(); });

  SUResult res = SUEntitiesAddEdges(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  // Transfer ownership of each edge
  for (auto& edge : edges_to_add)
      edge.attached(true);
  return edges_to_add;
}


Edge Entities::add_edge(Edge& edge) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_edge(): Entities is null");
  }
  if (edge.attached()) {
    throw std::invalid_argument("CW::Entities::add_edge(): Edge is already attached to an Entities object.");
  }
  SUEdgeRef edge_ref = edge.ref();
  SUResult res = SUEntitiesAddEdges(m_entities, 1, &edge_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  edge.attached(true);
  return edge;
}


void Entities::add_instance(ComponentInstance& instance) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_instance(): Entities is null");
  }
  if (!instance) {
    throw std::invalid_argument("CW::Entities::add_instance(): ComponentInstance argument is invalid");
  }
  if (instance.attached()) {
    throw std::invalid_argument("CW::Entities::add_instance(): ComponentInstance has already been added to a model.");
  }
  SUResult res = SUEntitiesAddInstance(m_entities, instance, nullptr);
  assert(res == SU_ERROR_NONE); _unused(res);
  instance.attached(true);
}


ComponentInstance Entities::add_instance(const ComponentDefinition& definition, const Transformation& transformation, const String& name){
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_instance(): Entities is null");
  }
  if (!definition) {
    throw std::invalid_argument("CW::Entities::add_instance(): ComponentDefinition argument is invalid");
  }
  if (!definition.attached()) {
    throw std::invalid_argument("CW::Entities::add_instance(): ComponentDefinition must be attached to a model before creating an instance.");
  }
  // Check that the definition is attached to this model
  if (definition.model().ref().ptr != this->model().ref().ptr) {
    throw std::invalid_argument("CW::Entities::add_instance(): ComponentDefinition is attached to a different model than this Entities object.");
  }
  SUComponentInstanceRef instance = SU_INVALID;
  SUResult res = SUComponentDefinitionCreateInstance(definition.ref(), &instance);
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
  assert(res == SU_ERROR_NONE); _unused(res);
  return ComponentInstance(instance, true);
}


// TODO: add_group needs to be refined

Group Entities::add_group(const ComponentDefinition& definition, const Transformation& transformation) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_group(): Entities is null");
  }
  if (!definition) {
    throw std::invalid_argument("CW::Entities::add_group(): ComponentDefinition argument is invalid");
  }
  if (!definition.is_group()) {
    throw std::invalid_argument("CW::Entities::add_group(): ComponentDefinition given is not a group");
  }
  // Groups cannot be created with a component definition and transformation objects.  Instead, the geometry must be copied in to a new Entities object in the group.
  Group new_group = this->add_group();
  new_group.transformation(transformation);
  Entities group_entities = new_group.entities();
  Entities def_entities = definition.entities();
  // Add geometry one by one to Geometry input.
  Model model = this->model();
  GeometryInput geom_input(&model);
  // GeometryInput object must first load layers and materials from the definition's model.
  // TODO: this is unsatisfatory for performance - need a better way to handle this.
  geom_input.load_materials(definition.model().materials());
  geom_input.load_layers(definition.model().layers());
  std::vector<Face> def_faces = def_entities.faces();
  for (size_t i=0; i < def_faces.size(); ++i) {
    //geom_input.add_face(def_faces[i]);
    geom_input.add_face(def_faces[i], false);
  }
  std::vector<Edge> def_edges = def_entities.edges(true); // add stray edges only
  for (size_t i=0; i < def_edges.size(); ++i) {
    geom_input.add_edge(def_edges[i], def_edges[i].material(), def_edges[i].layer());
  }
  if((def_faces.size() + def_edges.size()) > 0) {
    group_entities.fill(geom_input);
  }
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
  //SUResult res = SUComponentDefinitionCreateGroup(definition.ref(), &group);
  //assert(res == SU_ERROR_NONE); _unused(res);
  //SUTransformation transform = transformation.ref();
  //res = SUComponentInstanceSetTransform(instance, &transform);
}

Group Entities::add_group() {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_group(): Entities is null");
  }
  SUGroupRef group = SU_INVALID;
  SUResult res = SUGroupCreate(&group);
  assert(res == SU_ERROR_NONE);
  // Add group to the entities object before populating it.
  res = SUEntitiesAddGroup(m_entities, group);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Group(group, true);
}


bool Entities::transform_entities(std::vector<Entity>& elems, const Transformation& transform) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::transform_entities(): Entities is null");
  }
  SUTransformation trans_ref = transform.ref();
  SUResult res = SUEntitiesTransform(m_entities, elems.size(), elems[0], &trans_ref);
  assert(res == SU_ERROR_NONE || res == SU_ERROR_GENERIC); _unused(res);
  if (res == SU_ERROR_UNSUPPORTED) {
    throw std::invalid_argument("CW::Entities::transform_entities(): One of the elements given in the Entity vector is not contained by this Entities object.");
  }
  else if (SU_ERROR_GENERIC) {
    return false;
  }
  return true;
}


bool Entities::transform_entities(std::vector<Entity>& elems, std::vector<Transformation>& transforms) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::transform_entities(): Entities is null");
  }
  if (elems.size() != transforms.size()) {
    throw std::invalid_argument("CW::Entities::transform_entities(): different number of elements to transformation objects given - the same number must be given.");
  }
  assert(elems.size() == transforms.size());
  SUResult res = SUEntitiesTransformMultiple(m_entities, elems.size(), elems[0], transforms[0]);
  if (res == SU_ERROR_UNSUPPORTED) {
    throw std::invalid_argument("CW::Entities::transform_entities(): One of the elements given in the Entity vector is not contained by this Entities object.");
  }
  else if (SU_ERROR_GENERIC) {
    return false;
  }
  return true;
}


#if SketchUpAPI_VERSION_MAJOR < 2021
Model Entities::model() const {
  return m_model;
}
#else
Model Entities::model() const {
  SUEntitiesParent parent;
  parent.model = SU_INVALID;
  parent.definition = SU_INVALID;
  SUResult res = SUEntitiesGetParent(m_entities, &parent);
  if (res != SU_ERROR_NONE) {
    throw std::logic_error("CW::Entities::model(): could not get parent of Entities object.");
  }
  if (SUIsValid(parent.model)) {
    return Model(parent.model, false);
  }
  else {
    ComponentDefinition def(parent.definition, true); // assume that it is attached?!
    return def.model();
  }
}
#endif


Entities::operator SUEntitiesRef() {
  return m_entities;
}

} /* namespace CW */
