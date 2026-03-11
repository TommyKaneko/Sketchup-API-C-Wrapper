//
//  Entities.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
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
#include "SUAPI-CppWrapper/model/GeometryInputHelper.hpp"
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
#include "SUAPI-CppWrapper/model/GuideLine.hpp"
#include "SUAPI-CppWrapper/model/GuidePoint.hpp"
#include "SUAPI-CppWrapper/model/SectionPlane.hpp"
#include "SUAPI-CppWrapper/model/Image.hpp"
#include "SUAPI-CppWrapper/model/Text.hpp"
#include "SUAPI-CppWrapper/model/Dimension.hpp"
#include "SUAPI-CppWrapper/model/ArcCurve.hpp"

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
    throw std::logic_error("CW::Entities::bounding_box(): Entities is null");
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
  GeometryInputPlus geom_input(&model);
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


void Entities::fill(GeometryInput &geom_input) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::fill(): Entities is null");
  }
  // Check geom_input is not empty.
  if (geom_input.empty()) {
    return;
  }
  SUResult fill_res = SUEntitiesFill(m_entities, geom_input.ref(), true);
  assert(fill_res == SU_ERROR_NONE); _unused(fill_res);
}


std::vector<Face> Entities::add_faces(std::vector<Face>& faces) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_faces(): Entities is null");
  }
  for (const auto& face : faces) {
    if (face.attached()) {
      throw std::invalid_argument("CW::Entities::add_faces(): Face is already attached to a model. Use Face::copy() to create a detached copy first.");
    }
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
  GeometryInputPlus geom_input(&model);
  // GeometryInputPlus object must first load layers and materials from the definition's model.
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


std::vector<GuidePoint> Entities::guide_points() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::guide_points(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumGuidePoints(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<GuidePoint>(0);
  }
  std::vector<SUGuidePointRef> refs(count, SU_INVALID);
  res = SUEntitiesGetGuidePoints(m_entities, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<GuidePoint> points(count);
  std::transform(refs.begin(), refs.end(), points.begin(),
  [](const SUGuidePointRef& value) {
    return GuidePoint(value);
  });
  return points;
}


void Entities::add_guide_points(std::vector<GuidePoint>& points) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_guide_points(): Entities is null");
  }
  for (const auto& pt : points) {
    if (pt.attached()) {
      throw std::invalid_argument("CW::Entities::add_guide_points(): GuidePoint is already attached to a model");
    }
  }
  std::vector<SUGuidePointRef> refs(points.size(), SU_INVALID);
  std::transform(points.begin(), points.end(), refs.begin(),
  [](const GuidePoint& pt) { return pt.ref(); });
  SUResult res = SUEntitiesAddGuidePoints(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (auto& pt : points)
    pt.attached(true);
}


std::vector<GuideLine> Entities::guide_lines() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::guide_lines(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumGuideLines(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<GuideLine>(0);
  }
  std::vector<SUGuideLineRef> refs(count, SU_INVALID);
  res = SUEntitiesGetGuideLines(m_entities, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<GuideLine> lines(count);
  std::transform(refs.begin(), refs.end(), lines.begin(),
  [](const SUGuideLineRef& value) {
    return GuideLine(value);
  });
  return lines;
}


void Entities::add_guide_lines(std::vector<GuideLine>& lines) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_guide_lines(): Entities is null");
  }
  for (const auto& line : lines) {
    if (line.attached()) {
      throw std::invalid_argument("CW::Entities::add_guide_lines(): GuideLine is already attached to a model");
    }
  }
  std::vector<SUGuideLineRef> refs(lines.size(), SU_INVALID);
  std::transform(lines.begin(), lines.end(), refs.begin(),
  [](const GuideLine& line) { return line.ref(); });
  SUResult res = SUEntitiesAddGuideLines(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (auto& line : lines)
    line.attached(true);
}


std::vector<SectionPlane> Entities::section_planes() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::section_planes(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumSectionPlanes(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<SectionPlane>(0);
  }
  std::vector<SUSectionPlaneRef> refs(count, SU_INVALID);
  res = SUEntitiesGetSectionPlanes(m_entities, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<SectionPlane> planes(count);
  std::transform(refs.begin(), refs.end(), planes.begin(),
  [](const SUSectionPlaneRef& value) {
    return SectionPlane(value);
  });
  return planes;
}


void Entities::add_section_planes(std::vector<SectionPlane>& planes) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_section_planes(): Entities is null");
  }
  for (const auto& sp : planes) {
    if (sp.attached()) {
      throw std::invalid_argument("CW::Entities::add_section_planes(): SectionPlane is already attached to a model");
    }
  }
  std::vector<SUSectionPlaneRef> refs(planes.size(), SU_INVALID);
  std::transform(planes.begin(), planes.end(), refs.begin(),
  [](const SectionPlane& sp) { return sp.ref(); });
  SUResult res = SUEntitiesAddSectionPlanes(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (auto& sp : planes)
    sp.attached(true);
}


std::vector<Image> Entities::images() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::images(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumImages(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Image>(0);
  }
  std::vector<SUImageRef> refs(count, SU_INVALID);
  res = SUEntitiesGetImages(m_entities, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Image> images(count);
  std::transform(refs.begin(), refs.end(), images.begin(),
  [](const SUImageRef& value) {
    return Image(value);
  });
  return images;
}


void Entities::add_image(Image& image) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_image(): Entities is null");
  }
  if (image.attached()) {
    throw std::invalid_argument("CW::Entities::add_image(): Image is already attached to a model");
  }
  SUResult res = SUEntitiesAddImage(m_entities, image.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
  image.attached(true);
}


std::vector<Text> Entities::texts() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::texts(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumTexts(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Text>(0);
  }
  std::vector<SUTextRef> refs(count, SU_INVALID);
  res = SUEntitiesGetTexts(m_entities, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Text> texts(count);
  std::transform(refs.begin(), refs.end(), texts.begin(),
  [](const SUTextRef& value) {
    return Text(value);
  });
  return texts;
}


void Entities::add_texts(std::vector<Text>& texts) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_texts(): Entities is null");
  }
  for (const auto& t : texts) {
    if (t.attached()) {
      throw std::invalid_argument("CW::Entities::add_texts(): Text is already attached to a model");
    }
  }
  std::vector<SUTextRef> refs(texts.size(), SU_INVALID);
  std::transform(texts.begin(), texts.end(), refs.begin(),
  [](const Text& t) { return t.ref(); });
  SUResult res = SUEntitiesAddTexts(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (auto& t : texts)
    t.attached(true);
}


std::vector<Dimension> Entities::dimensions() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::dimensions(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumDimensions(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Dimension>{};
  }
  std::vector<SUDimensionRef> refs(count, SU_INVALID);
  res = SUEntitiesGetDimensions(m_entities, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Dimension> dims(count);
  std::transform(refs.begin(), refs.end(), dims.begin(),
  [](const SUDimensionRef& value) {
    return Dimension(value, true);
  });
  return dims;
}


#if SketchUpAPI_VERSION_MAJOR >= 2026
void Entities::add_dimensions(std::vector<Dimension>& dimensions)  {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_dimensions(): Entities is null");
  }
  for (const auto& dim : dimensions) {
    if (dim.attached()) {
      throw std::invalid_argument("CW::Entities::add_dimensions(): Dimension is already attached to a model");
    }
  }
  std::vector<SUDimensionRef> refs(dimensions.size(), SU_INVALID);
  std::transform(dimensions.begin(), dimensions.end(), refs.begin(),
  [](const Dimension& dim) { return dim.ref(); });
  SUResult res = SUEntitiesAddDimensions(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (auto& dim : dimensions)
    dim.attached(true);
}
#endif


std::vector<ArcCurve> Entities::arc_curves() const {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::arc_curves(): Entities is null");
  }
  size_t count = 0;
  SUResult res = SUEntitiesGetNumArcCurves(m_entities, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<ArcCurve>{};
  }
  std::vector<SUArcCurveRef> refs(count, SU_INVALID);
  res = SUEntitiesGetArcCurves(m_entities, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<ArcCurve> arcs;
  arcs.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    arcs.emplace_back(refs[i], true);
  }
  return arcs;
}


void Entities::add_arc_curves(std::vector<ArcCurve>& arcs) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::add_arc_curves(): Entities is null");
  }
  for (const auto& arc : arcs) {
    if (arc.attached()) {
      throw std::invalid_argument("CW::Entities::add_arc_curves(): ArcCurve is already attached to a model");
    }
  }
  std::vector<SUArcCurveRef> refs(arcs.size(), SU_INVALID);
  std::transform(arcs.begin(), arcs.end(), refs.begin(),
  [](const ArcCurve& arc) { return arc.ref(); });
  SUResult res = SUEntitiesAddArcCurves(m_entities, refs.size(), refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (auto& arc : arcs)
    arc.attached(true);
}


bool Entities::transform_entities(std::vector<Entity>& elems, const Transformation& transform) {
  if (!SUIsValid(m_entities)) {
    throw std::logic_error("CW::Entities::transform_entities(): Entities is null");
  }
  SUTransformation trans_ref = transform.ref();
  SUResult res = SUEntitiesTransform(m_entities, elems.size(), elems[0], &trans_ref);
  if (res == SU_ERROR_UNSUPPORTED) {
    throw std::invalid_argument("CW::Entities::transform_entities(): One of the elements given in the Entity vector is not contained by this Entities object.");
  }
  else if (res == SU_ERROR_GENERIC) {
    return false;
  }
  assert(res == SU_ERROR_NONE); _unused(res);
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
  else if (res == SU_ERROR_GENERIC) {
    return false;
  }
  assert(res == SU_ERROR_NONE); _unused(res);
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
