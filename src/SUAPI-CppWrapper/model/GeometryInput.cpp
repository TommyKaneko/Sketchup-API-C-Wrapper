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

#include <cassert>
#include <iostream>

#include "SUAPI-CppWrapper/model/GeometryInput.hpp"

#include <SketchupAPI/sketchup_info.h>

#include "SUAPI-CppWrapper/Initialize.hpp"
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
  
/***************************
** Private Static Methods **
****************************/
SUGeometryInputRef GeometryInput::create_geometry_input() {
  SUGeometryInputRef geom_input = SU_INVALID;
  SUResult res = SUGeometryInputCreate(&geom_input);
  assert(res == SU_ERROR_NONE);
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
GeometryInput::GeometryInput():
  m_geometry_input(create_geometry_input())
{
}


GeometryInput::~GeometryInput() {
  if (SUIsValid(m_geometry_input)) {
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE);
  }
}


GeometryInput::GeometryInput(const GeometryInput& other):
  m_geometry_input(create_geometry_input())
{
  for (size_t i=0; i < other.m_faces.size(); ++i) {
    add_face(other.m_faces[i].second);
  }
  for (size_t i=0; i < other.m_edges.size(); ++i) {
    add_edge(other.m_edges[i].second);
  }
}
  
  
GeometryInput& GeometryInput::operator=(const GeometryInput& other) {
  if (SUIsValid(m_geometry_input)) {
    SUResult res = SUGeometryInputRelease(&m_geometry_input);
    assert(res == SU_ERROR_NONE);
  }
  m_geometry_input = create_geometry_input();
  for (size_t i=0; i < other.m_faces.size(); ++i) {
    add_face(other.m_faces[i].second);
  }
  for (size_t i=0; i < other.m_edges.size(); ++i) {
    add_edge(other.m_edges[i].second);
  }
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
  return m_num_faces;
}


std::vector<std::pair<size_t, Face>> GeometryInput::faces() const {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::faces(): GeometryInput is null");
  }
  return m_faces;
}

/**
size_t GeometryInput::add_vertex(Point3D vertex) {
  SUPoint3D vertex_ref = vertex;
  SUGeometryInputAddVertex(m_geometry_input, &vertex_ref);
  return m_vertex_index++;
}

std::vector<size_t> GeometryInput::add_vertices(std::vector<Point3D> vertices) {
  std::vector<size_t> indices;
  for (size_t i = 0; i < vertices.size(); i++) {
    indices.push_back(add_vertex(vertices[i]));
  }
  return indices;
}
*/

size_t GeometryInput::add_face(const Face &face) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::add_face(): GeometryInput is null");
  }
  if(!face) {
    throw std::invalid_argument("CW::GeometryInput::add_face(): Face argument is null");
  }
  SULoopInputRef loop_input = SU_INVALID;
  SUResult res =  SULoopInputCreate(&loop_input);
  assert(res == SU_ERROR_NONE);
  // Add outer loop
  std::vector<Vertex> outer_vertices = face.outer_loop().vertices();
  std::vector<Edge> outer_edges = face.outer_loop().edges();
  for (size_t i=0; i < outer_vertices.size(); ++i) {
    SUPoint3D point = outer_vertices[i].position();
    res = SUGeometryInputAddVertex(m_geometry_input, &point);
    assert(res == SU_ERROR_NONE);
    res = SULoopInputAddVertexIndex(loop_input, m_vertex_index);
    assert(res == SU_ERROR_NONE);
    if (outer_edges[i].hidden()) {
      res = SULoopInputEdgeSetHidden(loop_input, i, true);
      assert(res == SU_ERROR_NONE);
    }
    if (outer_edges[i].smooth()) {
      res = SULoopInputEdgeSetSmooth(loop_input, i, true);
      assert(res == SU_ERROR_NONE);
    }
    if (outer_edges[i].soft()) {
      res = SULoopInputEdgeSetSoft(loop_input, i, true);
      assert(res == SU_ERROR_NONE);
    }
    ++m_vertex_index;
  }
  size_t added_face_index;
  res = SUGeometryInputAddFace(m_geometry_input, &loop_input, &added_face_index);
  assert(res == SU_ERROR_NONE);
  // Add inner loops
  std::vector<Loop> inner_loops = face.inner_loops();
  for (size_t i=0; i < inner_loops.size(); ++i) {
    SULoopInputRef inner_loop_input = SU_INVALID;
    res =  SULoopInputCreate(&loop_input);
    assert(res == SU_ERROR_NONE);
    std::vector<Vertex> inner_vertices = inner_loops[i].vertices();
    std::vector<Edge> inner_edges = inner_loops[i].edges();
    for (size_t j=0; j < inner_vertices.size(); ++j) {
      SUPoint3D point = inner_vertices[j].position();
      res = SUGeometryInputAddVertex(m_geometry_input, &point);
      assert(res == SU_ERROR_NONE);
      res = SULoopInputAddVertexIndex(inner_loop_input, m_vertex_index);
      assert(res == SU_ERROR_NONE);
      ++m_vertex_index;
      if (inner_edges[j].hidden()) {
        res = SULoopInputEdgeSetHidden(loop_input, j, true);
        assert(res == SU_ERROR_NONE);
      }
      if (inner_edges[j].smooth()) {
        res = SULoopInputEdgeSetSmooth(loop_input, j, true);
        assert(res == SU_ERROR_NONE);
      }
      if (inner_edges[j].soft()) {
        res = SULoopInputEdgeSetSoft(loop_input, j, true);
        assert(res == SU_ERROR_NONE);
      }
    }
    res = SUGeometryInputFaceAddInnerLoop(m_geometry_input, added_face_index, &inner_loop_input);
    assert(res == SU_ERROR_NONE);
  }
  // TODO: deallocate LoopInputRef?  It doesn't FEEL necessary because it has been added to the Geometry Input object. Needs testing.
  // Add layer
  // TODO: there are problems with releasing a model that gets a layer from another model.  The following code causes a EXEC_BAD_ACCESS error when calling SUModelRelease, during CLayerDestroy process.
  /*
  Layer face_layer = face.layer();
  if (!!face_layer) {
    res = SUGeometryInputFaceSetLayer(m_geometry_input, added_face_index, face_layer.ref());
    assert(res == SU_ERROR_NONE);
  }
  // Set Materials TODO: not done quite right here, I don't think... uv coords are not set.
  SUMaterialInput material = MaterialInput(face.material()).ref();
  res = SUGeometryInputFaceSetFrontMaterial(m_geometry_input, added_face_index, &material);
  assert(res == SU_ERROR_NONE);
  SUMaterialInput back_material = MaterialInput(face.back_material()).ref();
  res = SUGeometryInputFaceSetBackMaterial(m_geometry_input, added_face_index, &back_material);
  assert(res == SU_ERROR_NONE);
  */
  // There are further properties that need to be added, such as attributes, and these must be added after the entities::fill() operation.
  m_faces.push_back(std::pair<size_t, Face> (added_face_index, face));
  ++m_num_faces;
  return added_face_index;
}


size_t GeometryInput::add_faces(const std::vector<Face>& faces) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::add_faces(): GeometryInput is null");
  }
  size_t index = m_num_faces;
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
  size_t start_vertex_index = m_vertex_index;
  SUPoint3D start_point = edge.start().position();
  SUResult res = SUGeometryInputAddVertex(m_geometry_input, &start_point);
  m_vertex_index++;
  assert(res == SU_ERROR_NONE);
  size_t end_vertex_index = m_vertex_index;
  SUPoint3D end_point = edge.end().position();
  res = SUGeometryInputAddVertex(m_geometry_input, &end_point);
  m_vertex_index++;
  assert(res == SU_ERROR_NONE);
  size_t added_edge_index;
  res = SUGeometryInputAddEdge(m_geometry_input, start_vertex_index, end_vertex_index, &added_edge_index);
  assert(res == SU_ERROR_NONE);
  
  // Add other information about the edge
  if (!!edge.material()) {
    res = SUGeometryInputEdgeSetMaterial(m_geometry_input, added_edge_index, edge.material().ref());
    assert(res == SU_ERROR_NONE);
  }
  if (edge.hidden()) {
    res = SUGeometryInputEdgeSetHidden(m_geometry_input, added_edge_index, true);
    assert(res == SU_ERROR_NONE);
  }
  if (edge.smooth()) {
    res = SUGeometryInputEdgeSetSmooth(m_geometry_input, added_edge_index, true);
    assert(res == SU_ERROR_NONE);
  }
  if (edge.soft()) {
    res = SUGeometryInputEdgeSetSoft(m_geometry_input, added_edge_index, true);
    assert(res == SU_ERROR_NONE);
  }
  m_edges.push_back(std::pair<size_t, Edge> (added_edge_index, edge));
  ++m_num_edges;
  return added_edge_index;
}


size_t GeometryInput::add_edges(const std::vector<Edge>& edges) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::add_edges(): GeometryInput is null");
  }
  size_t index = m_num_edges;
  for (size_t i=0; i < edges.size(); ++i) {
    index = add_edge(edges[i]);
  }
  return index;
}


bool GeometryInput::empty() const {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInput::empty(): GeometryInput is null");
  }
  if (m_faces.size() == 0 && m_edges.size() == 0) {
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
  assert(m_num_faces == 0); // Undefined behaviour when overwriting vertices
  assert(m_num_edges == 0); // Undefined behaviour when overwriting vertices
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

