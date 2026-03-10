//
//  GeometryInput.cpp
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
#include <iostream>

#include "SUAPI-CppWrapper/model/GeometryInput.hpp"

#include <SketchupAPI/sketchup_info.h>

#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/LoopInput.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
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


/******************************
** Constructors / Destructor **
*******************************/
GeometryInput::GeometryInput():
  m_geometry_input(create_geometry_input())
{
  num_objects_[m_geometry_input] = 1;
}


GeometryInput::~GeometryInput() {
  if (SUIsValid(m_geometry_input) && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input); // This is equivalent to the delete command.
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  else {
    --num_objects_[m_geometry_input];
    if (num_objects_[m_geometry_input] == 0) {
      num_objects_.erase(m_geometry_input);
    }
  }
}


GeometryInput::GeometryInput(const GeometryInput& other) {
  if (SUIsValid(m_geometry_input) && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  else {
    --num_objects_[m_geometry_input];
  }
  m_geometry_input = other.m_geometry_input;
  ++num_objects_[m_geometry_input];
}


GeometryInput& GeometryInput::operator=(const GeometryInput& other) {
  if (SUIsValid(m_geometry_input) && num_objects_[m_geometry_input] == 1) {
    num_objects_.erase(m_geometry_input);
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  else {
    --num_objects_[m_geometry_input];
  }
  m_geometry_input = other.m_geometry_input;
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
  m_vertex_count++;
  return m_vertex_count-1;
}


void GeometryInput::set_vertices(const std::vector<SUPoint3D>& points) {
  assert(this->counts()[1] == 0); // Undefined behaviour when overwriting vertices
  assert(this->counts()[2] == 0); // Undefined behaviour when overwriting vertices
  SUResult res = SUGeometryInputSetVertices(m_geometry_input, points.size(), points.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  // Overwrite the existing vertex count
  m_vertex_count = points.size();
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
}


void GeometryInput::edge_layer(size_t edge_index, const Layer& layer) {
  SUResult res = SUGeometryInputEdgeSetLayer(m_geometry_input, edge_index, layer.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
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
  if(loop_input.m_attached) {
    throw std::logic_error("CW::GeometryInput::add_face(): LoopInput is already attached to a GeometryInput object and cannot be added again");
  }
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
}


void GeometryInput::face_back_material(size_t face_index, MaterialInput& material_input) {
  SUMaterialInput material_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetBackMaterial (m_geometry_input, face_index, &material_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
}
#if SketchUpAPI_VERSION_MAJOR >= 2021


void GeometryInput::face_front_material_position(size_t face_index, const MaterialPositionInput& material_input) {
  SUMaterialPositionInput material_input_ref = material_input.ref();
  SUResult res = SUGeometryInputFaceSetFrontMaterialByPosition(this->ref(), face_index, &material_input_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (res == SU_ERROR_OUT_OF_RANGE) {
    throw std::invalid_argument("CW::GeometryInput::face_front_material_position(): Face index is out of range");
  }
}

void GeometryInput::face_back_material_position(size_t face_index, const MaterialPositionInput& material_input) {
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

} /* namespace CW */

bool operator==(const SUGeometryInputRef& lhs, const SUGeometryInputRef& rhs) {
  return lhs.ptr == rhs.ptr;
}
