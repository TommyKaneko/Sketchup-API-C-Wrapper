//
//  GeometryInput.cpp
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
#include <iostream>

#include "SUAPI-CppWrapper/model/GeometryInput.hpp"

#include <SketchupAPI/sketchup_info.h>

#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/LoopInput.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/MaterialInput.hpp"

namespace CW {

std::unordered_map<SUGeometryInputRef, size_t> GeometryInput::num_objects_ = {};

/***************************
** Private Static Methods **
****************************/
SUGeometryInputRef GeometryInput::create_geometry_input() {
  SUGeometryInputRef geom_input = SU_INVALID;
  SUResult res = SUGeometryInputCreate(&geom_input); // this is equivalent to allocating memory
  assert(res == SU_ERROR_NONE); _unused(res);
  return geom_input;
}

/*
SUResult GeometryInput::add_loop(LoopInput &loop) {
  std::vector<Point3D> vertices = loop.get_vertices();

  std::vector<size_t> indices = add_vertices(vertices);
  std::vector<Edge> edges = loop.get_edges();

  for (size_t i=0; i < indices.size(); i++) {
    SULoopInputEdgeSetHidden(loop.ref(), indices[i], edges[i].hidden());
    SULoopInputEdgeSetSoft(loop.ref(), indices[i], edges[i].soft());
    SULoopInputEdgeSetSmooth(loop.ref(), indices[i], edges[i].smooth());
  }
  return SU_ERROR_NONE;
}
*/

/******************************
** Constructors / Destructor **
*******************************/
// GeometryInput::GeometryInput(SUModelRef target_model):
//   m_geometry_input(create_geometry_input()),
//   m_attached(false),
//   m_target_model(target_model)
// {
//   num_objects_[m_geometry_input] = 1;
// }

GeometryInput::GeometryInput(Model* target_model):
  m_geometry_input(create_geometry_input()),
  m_attached(false),
  m_target_model(target_model),
  m_material_dict(target_model),
  m_layer_dict(target_model)
{
  num_objects_[m_geometry_input] = 1;
}


GeometryInput::~GeometryInput() {
  if (SUIsValid(m_geometry_input) && !m_attached && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input); // This is equivalent to the delete command.
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  else {
    --num_objects_[m_geometry_input];
  }
}


GeometryInput::GeometryInput(const GeometryInput& other):
  m_target_model(other.m_target_model),
  m_material_dict(other.m_target_model),
  m_layer_dict(other.m_target_model)
{
  if (SUIsValid(m_geometry_input) && !m_attached && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  else {
    --num_objects_[m_geometry_input];
  }
  m_geometry_input = other.m_geometry_input;
  m_attached = other.m_attached;
  ++num_objects_[m_geometry_input];
}


GeometryInput& GeometryInput::operator=(const GeometryInput& other) {
  if (SUIsValid(m_geometry_input) && !m_attached && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  else {
    --num_objects_[m_geometry_input];
  }
  m_geometry_input = other.m_geometry_input;
  m_attached = other.m_attached;
  m_target_model = other.m_target_model;
  m_material_dict = other.m_material_dict;
  m_layer_dict = other.m_layer_dict;
  ++num_objects_[m_geometry_input];
  return (*this);
}



SUGeometryInputRef GeometryInput::ref() const {
  return m_geometry_input;
}


bool GeometryInput::operator!() const {
  return SUIsInvalid(m_geometry_input);
}


void GeometryInput::load_materials(const std::vector<Material>& materials) {
  // Check to see if each material is in the model already
  std::vector<Material> target_materials = m_target_model->materials();
  std::vector<Material> materials_to_add; materials_to_add.reserve(materials.size());
  for (const Material& material : materials) {
    bool matched = false;
    for (const Material& target_material : target_materials) {
      if (material == target_material) {
        // We can directly add the material to the map
        m_material_dict.add_reference(material, target_material);
        matched = true;
        break;
      }
      else if (material.name() == target_material.name()) {
        // match with name
        m_material_dict.add_reference(material, target_material);
        matched = true;
        break;
      }
    }
    if (!matched) {
      // Create a new material in the
      materials_to_add.push_back(material);
    }
  }
  if (materials_to_add.size() == 0) return;
  // Add the materials that do not exist in the model.
  m_target_model->add_materials(materials_to_add, false);
  target_materials = m_target_model->materials(); // Refresh the list of materials.
  for (const Material& material : materials_to_add) {
    for (const Material& target_material : target_materials) {
      if (material == target_material) {
        // We can directly add the material to the map
        m_material_dict.add_reference(material, target_material);
        break;
      }
      else if (material.name() == target_material.name()) {
        // match with name
        m_material_dict.add_reference(material, target_material);
        break;
      }
    }
  }
  return;
}


void GeometryInput::load_layers(const std::vector<Layer>& layers) {
  // Check to see if each layer is in the model already
  std::vector<Layer> target_layers = m_target_model->layers();
  std::vector<Layer> layers_to_add; layers_to_add.reserve(layers.size());
  for (const Layer& layer : layers) {
    bool matched = false;
    for (const Layer& target_layer : target_layers) {
      if (layer == target_layer) {
        // We can directly add the layer to the map
        m_layer_dict.add_reference(layer, target_layer);
        matched = true;
        break;
      }
      else if (layer.name() == target_layer.name()) {
        // match with name
        m_layer_dict.add_reference(layer, target_layer);
        matched = true;
        break;
      }
    }
    if (!matched) {
      // Create a new layer in the
      layers_to_add.push_back(layer);
    }
  }
  if (layers_to_add.size() == 0) return;
  // Add the layers that do not exist in the model.
  m_target_model->add_layers(layers_to_add, false);
  target_layers = m_target_model->layers(); // Refresh the list of materials.
  for (const Layer& layer : layers_to_add) {
    for (const Layer& target_layer : target_layers) {
      if (layer == target_layer) {
        // We can directly add the layer to the map
        m_layer_dict.add_reference(layer, target_layer);
        break;
      }
      else if (layer.name() == target_layer.name()) {
        // match with name
        m_layer_dict.add_reference(layer, target_layer);
        break;
      }
    }
  }
  return;
}


size_t GeometryInput::num_faces() const {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::num_faces(): GeometryInput is null");
  }
  return this->counts()[1];
}


size_t GeometryInput::add_face(const Face &face, bool copy_material_layer) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::add_face(): GeometryInput is null");
  }
  if(!face) {
    throw std::invalid_argument("CW::GeometryInput::add_face(): Face argument is null");
  }
  LoopInput outer_loop_input;
  // Add outer loop
  std::vector<Point3D> outer_points = face.outer_loop().points();
  std::vector<Edge> outer_edges = face.outer_loop().edges();
  for (size_t i=0; i < outer_points.size(); ++i) {
    size_t v_index = this->add_vertex(outer_points[i]);
    outer_loop_input.add_vertex_index(v_index);
    if (outer_edges[i].hidden()) {
      outer_loop_input.set_edge_hidden(v_index, true);
    }
    if (outer_edges[i].smooth()) {
      outer_loop_input.set_edge_smooth(v_index, true);
    }
    if (outer_edges[i].soft()) {
      outer_loop_input.set_edge_soft(v_index, true);
    }
    if (!!outer_edges[i].material()) {
      outer_loop_input.set_edge_material(v_index, m_material_dict.get_reference(outer_edges[i].material()));
    }
    if (!!outer_edges[i].layer()) {
      outer_loop_input.set_edge_layer(v_index, m_layer_dict.get_reference(outer_edges[i].layer()));
    }
  }
  size_t added_face_index = this->add_face(outer_loop_input);
  // Add inner loops
  std::vector<Loop> inner_loops = face.inner_loops();
  for (size_t i=0; i < inner_loops.size(); ++i) {
    LoopInput inner_loop_input;
    std::vector<Point3D> inner_points = inner_loops[i].points();
    std::vector<Edge> inner_edges = inner_loops[i].edges();
    for (size_t j=0; j < inner_points.size(); ++j) {
      size_t v_index = this->add_vertex(inner_points[i]);
      inner_loop_input.add_vertex_index(v_index);
      if (inner_edges[i].hidden()) {
        inner_loop_input.set_edge_hidden(v_index, true);
      }
      if (inner_edges[i].smooth()) {
        inner_loop_input.set_edge_smooth(v_index, true);
      }
      if (inner_edges[i].soft()) {
        inner_loop_input.set_edge_soft(v_index, true);
      }
      if (!!inner_edges[i].material()) {
        inner_loop_input.set_edge_material(v_index, m_material_dict.get_reference(inner_edges[i].material()));
      }
      if (!!inner_edges[i].layer()) {
        inner_loop_input.set_edge_layer(v_index, m_layer_dict.get_reference(inner_edges[i].layer()));
      }
    }
    this->face_add_inner_loop(added_face_index, inner_loop_input);
  }
  if (copy_material_layer) {
    // Add layer
    Layer face_layer = face.layer();
    if (!!face_layer) {
      this->face_layer(added_face_index, m_layer_dict.get_reference(face_layer));
      face_layer.attached(true);
    }
    #if SketchUpAPI_VERSION_MAJOR < 2021
    // Old way of setting the material
    Material front_mat = face.material();
    if (!!front_mat) {
      MaterialInput front_material_input(front_mat);
      //assert(m_target_model->material_exists(front_material_input.material()));
      this->face_front_material(added_face_index, front_material_input);
    }
    Material back_mat = face.back_material();
    if (!!back_mat) {
      MaterialInput back_material_input(face.back_material());
      assert(m_target_model->material_exists(back_material_input.material()));
      this->face_back_material(added_face_index, back_material_input);
    }
    #else
    // New way of setting the material with MaterialInputPosition
    if (!!face.material()) {
      MaterialPositionInput material_input = face.material_position_front();
      // Replace material with one in the target model
      material_input.material(this->material_reference(face.material()));
      face_front_material_position(added_face_index, material_input);
    }
    if (!!face.back_material()) {
      MaterialPositionInput material_input = face.material_position_back();
      material_input.material(this->material_reference(face.back_material()));
      face_back_material_position(added_face_index, material_input);
    }
    #endif
    // TODO: create a way to add attributes to the faces.
  }
  return added_face_index;
}


size_t GeometryInput::add_faces(const std::vector<Face>& faces, bool copy_material_layer) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::add_faces(): GeometryInput is null");
  }
  size_t index = 0;
  for (size_t i=0; i < faces.size(); ++i) {
    index = add_face(faces[i]);
  }
  return index;
}


size_t GeometryInput::add_edge(const Edge &edge, const Material& material, const Layer& layer) {
  if(!edge) {
    throw std::invalid_argument("CW::GeometryInput::add_edge(): Edge argument is null");
  }
  if (SU_API_VERSION_MAJOR < 5) {
    // Edges can only be added since SU2017
    return 0;
  }
  Point3D start_point = edge.start().position();
  size_t start_index = this->add_vertex(start_point);
  Point3D end_point = edge.end().position();
  size_t end_index = this->add_vertex(end_point);
  size_t added_edge_index = this->add_edge(start_index, end_index);

  // Add other information about the edge
  if (edge.hidden()) {
    this->edge_hidden(added_edge_index, true);
  }
  if (edge.smooth()) {
    this->edge_smooth(added_edge_index, true);
  }
  if (edge.soft()) {
    this->edge_soft(added_edge_index, true);
  }
  if (!!material) {
    this->edge_material(added_edge_index, m_material_dict.get_reference(material));
  }
  if (!!layer) {
    this->edge_layer(added_edge_index, m_layer_dict.get_reference(layer));
  }
  return added_edge_index;
}


size_t GeometryInput::add_edges(const std::vector<Edge>& edges) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::add_edges(): GeometryInput is null");
  }
  size_t index = 0;
  for (size_t i=0; i < edges.size(); ++i) {
    index = add_edge(edges[i]);
  }
  return index;
}


size_t GeometryInput::add_edges(const std::vector<Edge>& edges, const std::vector<Material>& materials, const std::vector<Layer>& layers) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::add_edges(): GeometryInput is null");
  }
  if(edges.size() != materials.size() || edges.size() != layers.size()) {
    throw std::logic_error("CW::GeometryInput::add_edges(): size of layers and materials vector must match the size of edges");
  }
  size_t edge_index = 0;
  for (size_t i=0; i < edges.size(); ++i) {
    edge_index = this->add_edge(edges[i], materials[i], layers[i]);
  }
  return edge_index;
}


bool GeometryInput::empty() const {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::empty(): GeometryInput is null");
  }
  std::array<size_t, 5> counts = this->counts();
  if (counts[1] == 0 && counts[2] == 0) {
    return true;
  }
  return false;
}


size_t GeometryInput::add_vertex(const Point3D& point) {
  SUResult res = SUGeometryInputAddVertex(m_geometry_input, point);
  assert(res == SU_ERROR_NONE); _unused(res);
  m_vertex_index++;
  return m_vertex_index-1;
}


void GeometryInput::set_vertices(const std::vector<SUPoint3D>& points) {
  assert(this->counts()[1] == 0); // Undefined behaviour when overwriting vertices
  assert(this->counts()[2] == 0); // Undefined behaviour when overwriting vertices
  SUResult res = SUGeometryInputSetVertices(m_geometry_input, points.size(), points.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  // Overwrite the existing vertex count
  m_vertex_index = points.size();
}


void GeometryInput::set_vertices(const std::vector<Point3D>& points) {
  std::vector<SUPoint3D> point_refs(points.size());
  std::transform(points.begin(), points.end(), point_refs.begin(),
    [](const Point3D& value) {
    return value;
  });
  return this->set_vertices(point_refs);
}


size_t GeometryInput::add_edge(size_t vertex0_index, size_t vertex1_index) {
  size_t added_edge_index;
  SUResult res = SUGeometryInputAddEdge(m_geometry_input, vertex0_index, vertex1_index, &added_edge_index);
  assert(res == SU_ERROR_NONE); _unused(res);
  return added_edge_index;
}


void GeometryInput::edge_hidden(size_t edge_index, bool hidden) {
  SUResult res = SUGeometryInputEdgeSetHidden(m_geometry_input, edge_index, hidden);
  assert(res == SU_ERROR_NONE); _unused(res);
}


void GeometryInput::edge_soft(size_t edge_index, bool soft) {
  SUResult res = SUGeometryInputEdgeSetSoft(m_geometry_input, edge_index, soft);
  assert(res == SU_ERROR_NONE); _unused(res);
}


void GeometryInput::edge_smooth(size_t edge_index, bool smooth) {
  SUResult res = SUGeometryInputEdgeSetSmooth(m_geometry_input, edge_index, smooth);
  assert(res == SU_ERROR_NONE); _unused(res);
}


void GeometryInput::edge_material(size_t edge_index, const Material& material) {
  SUResult res = SUGeometryInputEdgeSetMaterial(m_geometry_input, edge_index, material.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
  // TODO: check that material exists in target model.
}


void GeometryInput::edge_layer(size_t edge_index, const Layer& layer) {
  SUResult res = SUGeometryInputEdgeSetLayer(m_geometry_input, edge_index, layer.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
  // TODO: check that layer exists in target model.
}


size_t GeometryInput::add_curve(const std::vector<size_t>& edge_indices) {
  size_t added_curve_index;
  SUResult res = SUGeometryInputAddCurve(m_geometry_input, edge_indices.size(), edge_indices.data(), &added_curve_index);
  assert(res == SU_ERROR_NONE); _unused(res);
  return added_curve_index;
}


std::pair<size_t, size_t> GeometryInput::add_arc_curve(size_t start_point, size_t end_point, const Point3D& center, const Vector3D& normal, size_t num_segments) {
  size_t added_curve_index;
  size_t control_edge_index;
  SUResult res = SUGeometryInputAddArcCurve(m_geometry_input, start_point, end_point, center, normal, num_segments, &added_curve_index, &control_edge_index);
  assert(res == SU_ERROR_NONE); _unused(res);
  return std::pair<size_t, size_t> {added_curve_index, control_edge_index};
}


size_t GeometryInput::add_face(LoopInput& loop_input) {
  size_t added_face_index;
  SULoopInputRef loop_ref = loop_input.ref();
  SUResult res = SUGeometryInputAddFace(m_geometry_input, &loop_ref, &added_face_index);
  assert(res == SU_ERROR_NONE); _unused(res);
  loop_input.m_attached = true;
  return added_face_index;
}


void GeometryInput::face_reverse(size_t face_index, bool reverse) {
  SUResult res = SUGeometryInputFaceSetReverse(m_geometry_input, face_index, reverse);
  assert(res == SU_ERROR_NONE); _unused(res);
}


void GeometryInput::face_layer(size_t face_index, const Layer& layer) {
  SUResult res = SUGeometryInputFaceSetLayer(m_geometry_input, face_index, layer.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void GeometryInput::face_add_inner_loop(size_t face_index, LoopInput& inner_loop) {
  SULoopInputRef loop_ref = inner_loop.ref();
  SUResult res = SUGeometryInputFaceAddInnerLoop(m_geometry_input, face_index, &loop_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  inner_loop.m_attached = true;
}


void GeometryInput::face_front_material(size_t face_index, MaterialInput& material_input) {
  SUMaterialInput material_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetFrontMaterial(m_geometry_input, face_index, &material_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  // TODO: assert MateriealRef in the MaterialInput is not attached to a different model from the one it will be added to.
}


void GeometryInput::face_back_material(size_t face_index, MaterialInput& material_input) {
  SUMaterialInput material_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetBackMaterial (m_geometry_input, face_index, &material_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  // TODO: assert MateriealRef in the MaterialInput is not attached to a different model from the one it will be added to.
}
#if SketchUpAPI_VERSION_MAJOR >= 2021


void GeometryInput::face_front_material_position(size_t face_index, const MaterialPositionInput& material_input) {
  Material material = material_input.material();
  if (material.model() != *this->m_target_model) {
    throw std::logic_error("CW::GeometryInput::face_front_material_position(): material does not reference a material in the target model");
  }
  SUMaterialPositionInput material_input_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetFrontMaterialByPosition(this->ref(), face_index, &material_input_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (res == SU_ERROR_OUT_OF_RANGE) {
    throw std::invalid_argument("CW::GeometryInput::face_front_material_position(): Face index is out of range");
  }
}

void GeometryInput::face_back_material_position(size_t face_index, const MaterialPositionInput& material_input) {
  Material material = material_input.material();
  if (material.model() != *this->m_target_model) {
    throw std::logic_error("CW::GeometryInput::face_back_material_position(): material does not reference a material in the target model");
  }
  SUMaterialPositionInput material_input_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetBackMaterialByPosition(this->ref(), face_index, &material_input_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (res == SU_ERROR_OUT_OF_RANGE) {
    throw std::invalid_argument("CW::GeometryInput::face_back_material_position(): Face index is out of range");
  }
}
#endif


void GeometryInput::face_hidden(size_t face_index, bool hidden) {
  SUResult res = SUGeometryInputFaceSetHidden(m_geometry_input, face_index, hidden);
  assert(res == SU_ERROR_NONE); _unused(res);
}


std::array<size_t, 5> GeometryInput::counts() const {
  std::array<size_t, 5> count_arr;
  SUResult res = SUGeometryInputGetCounts(m_geometry_input, &count_arr[0], &count_arr[1], &count_arr[2], &count_arr[3], &count_arr[4]);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count_arr;
}


Material GeometryInput::material_reference(const Material& material) const {
  Material reference = m_material_dict.get_reference(material);
  return reference;
}


Layer GeometryInput::layer_reference(const Layer& layer) const {
  Layer reference = m_layer_dict.get_reference(layer);
  return reference;
}


} /* namespace CW */

bool operator==(const SUGeometryInputRef& lhs, const SUGeometryInputRef& rhs) {
  return lhs.ptr == rhs.ptr;
}
