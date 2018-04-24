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
  SUResult res = SUGeometryInputCreate(&geom_input);
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
GeometryInput::GeometryInput(SUModelRef target_model):
  m_geometry_input(create_geometry_input()),
  m_attached(false),
  m_target_model(target_model)
{
  num_objects_[m_geometry_input] = 1;
}


GeometryInput::~GeometryInput() {
  if (SUIsValid(m_geometry_input) && !m_attached && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  else {
    --num_objects_[m_geometry_input];
  }
}


GeometryInput::GeometryInput(const GeometryInput& other):
  m_target_model(other.m_target_model)
{
  if (SUIsValid(m_geometry_input) && !m_attached && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE);
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
  ++num_objects_[m_geometry_input];
  return (*this);
}



SUGeometryInputRef GeometryInput::ref() const {
  return m_geometry_input;
}


bool GeometryInput::operator!() const {
  return SUIsInvalid(m_geometry_input);
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
    // TODO: set layer and material
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
      // TODO: set layer and material
    }
    this->face_add_inner_loop(added_face_index, inner_loop_input);
  }
  if (copy_material_layer) {
    // Add layer
    Layer face_layer = face.layer();
    if (!!face_layer) {
      assert(Model(m_target_model, false).layer_exists(face_layer));
      this->face_layer(added_face_index, face_layer);
      face_layer.attached(true);
    }
    // Set Materials TODO: not done quite right here, I don't think... uv coords are not set.
    Material front_mat = face.material();
    if (!!front_mat) {
      MaterialInput front_material_input(front_mat);
      assert(Model(m_target_model, false).material_exists(front_material_input.material()));
      this->face_front_material(added_face_index, front_material_input);
    }
    Material back_mat = face.back_material();
    if (!!back_mat) {
      MaterialInput back_material_input(face.back_material());
      assert(Model(m_target_model, false).material_exists(back_material_input.material()));
      this->face_back_material(added_face_index, back_material_input);
    }
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


size_t GeometryInput::add_edge(const Edge &edge) {
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
  // TODO: add edge Layer and Material
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
  assert(res == SU_ERROR_NONE);
  m_vertex_index++;
  return m_vertex_index-1;
}


void GeometryInput::set_vertices(const std::vector<SUPoint3D>& points) {
  assert(this->counts()[1] == 0); // Undefined behaviour when overwriting vertices
  assert(this->counts()[2] == 0); // Undefined behaviour when overwriting vertices
  SUResult res = SUGeometryInputSetVertices(m_geometry_input, points.size(), points.data());
  assert(res == SU_ERROR_NONE);
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
  assert(res == SU_ERROR_NONE);
  return added_edge_index;
}


void GeometryInput::edge_hidden(size_t edge_index, bool hidden) {
  SUResult res = SUGeometryInputEdgeSetHidden(m_geometry_input, edge_index, hidden);
  assert(res == SU_ERROR_NONE);
}


void GeometryInput::edge_soft(size_t edge_index, bool soft) {
  SUResult res = SUGeometryInputEdgeSetSoft(m_geometry_input, edge_index, soft);
  assert(res == SU_ERROR_NONE);
}


void GeometryInput::edge_smooth(size_t edge_index, bool smooth) {
  SUResult res = SUGeometryInputEdgeSetSmooth(m_geometry_input, edge_index, smooth);
  assert(res == SU_ERROR_NONE);
}


void GeometryInput::edge_material(size_t edge_index, const Material& material) {
  SUResult res = SUGeometryInputEdgeSetMaterial(m_geometry_input, edge_index, material.ref());
  assert(res == SU_ERROR_NONE);
  // TODO: check that material exists in target model.
}


void GeometryInput::edge_layer(size_t edge_index, const Layer& layer) {
  SUResult res = SUGeometryInputEdgeSetLayer(m_geometry_input, edge_index, layer.ref());
  assert(res == SU_ERROR_NONE);
  // TODO: check that layer exists in target model.
}


size_t GeometryInput::add_curve(const std::vector<size_t>& edge_indices) {
  size_t added_curve_index;
  SUResult res = SUGeometryInputAddCurve(m_geometry_input, edge_indices.size(), edge_indices.data(), &added_curve_index);
  assert(res == SU_ERROR_NONE);
  return added_curve_index;
}


std::pair<size_t, size_t> GeometryInput::add_arc_curve(size_t start_point, size_t end_point, const Point3D& center, const Vector3D& normal, size_t num_segments) {
  size_t added_curve_index;
  size_t control_edge_index;
  SUResult res = SUGeometryInputAddArcCurve(m_geometry_input, start_point, end_point, center, normal, num_segments, &added_curve_index, &control_edge_index);
  assert(res == SU_ERROR_NONE);
  return std::pair<size_t, size_t> {added_curve_index, control_edge_index};
}


size_t GeometryInput::add_face(LoopInput& loop_input) {
  size_t added_face_index;
  SULoopInputRef loop_ref = loop_input.ref();
  SUResult res = SUGeometryInputAddFace(m_geometry_input, &loop_ref, &added_face_index);
  assert(res == SU_ERROR_NONE);
  loop_input.m_attached = true;
  return added_face_index;
}


void GeometryInput::face_reverse(size_t face_index, bool reverse) {
  SUResult res = SUGeometryInputFaceSetReverse(m_geometry_input, face_index, reverse);
  assert(res == SU_ERROR_NONE);
}


void GeometryInput::face_layer(size_t face_index, const Layer& layer) {
  SUResult res = SUGeometryInputFaceSetLayer(m_geometry_input, face_index, layer.ref());
  assert(res == SU_ERROR_NONE);
}


void GeometryInput::face_add_inner_loop(size_t face_index, LoopInput& inner_loop) {
  SULoopInputRef loop_ref = inner_loop.ref();
  SUResult res = SUGeometryInputFaceAddInnerLoop(m_geometry_input, face_index, &loop_ref);
  assert(res == SU_ERROR_NONE);
  inner_loop.m_attached = true;
}


void GeometryInput::face_front_material(size_t face_index, MaterialInput& material_input) {
  SUMaterialInput material_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetFrontMaterial(m_geometry_input, face_index, &material_ref);
  assert(res == SU_ERROR_NONE);
  // TODO: assert MateriealRef in the MaterialInput is not attached to a different model from the one it will be added to.
}


void GeometryInput::face_back_material(size_t face_index, MaterialInput& material_input) {
  SUMaterialInput material_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetBackMaterial (m_geometry_input, face_index, &material_ref);
  assert(res == SU_ERROR_NONE);
  // TODO: assert MateriealRef in the MaterialInput is not attached to a different model from the one it will be added to.
}


void GeometryInput::face_hidden(size_t face_index, bool hidden) {
  SUResult res = SUGeometryInputFaceSetHidden(m_geometry_input, face_index, hidden);
  assert(res == SU_ERROR_NONE);
}


std::array<size_t, 5> GeometryInput::counts() const {
  std::array<size_t, 5> count_arr;
  SUResult res = SUGeometryInputGetCounts(m_geometry_input, &count_arr[0], &count_arr[1], &count_arr[2], &count_arr[3], &count_arr[4]);
  assert(res == SU_ERROR_NONE);
  return count_arr;
}

} /* namespace CW */

bool operator==(const SUGeometryInputRef& lhs, const SUGeometryInputRef& rhs) {
  return lhs.ptr == rhs.ptr;
}

