//
//  GeometryInput.cpp
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
#include <iostream>

#include "GeometryInput.hpp"

#include <SketchupAPI/sketchup_info.h>

#include "Entities.hpp"
#include "Vertex.hpp"
#include "Loop.hpp"
#include "LoopInput.hpp"
#include "Layer.hpp"
#include "Material.hpp"
#include "Face.hpp"
#include "Edge.hpp"
#include "MaterialInput.hpp"

namespace CW {
  
/***************************
** Private Static Methods **
****************************/
SUGeometryInputRef GeometryInput::create_geometry_input() {
	SUGeometryInputRef geom_input = SU_INVALID;
	SU_RESULT res = SUGeometryInputCreate(&geom_input);
  assert(res == SU_ERROR_NONE);
  return geom_input;
}

/*
SU_RESULT GeometryInput::add_loop(LoopInput &loop) {
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
  	SU_RESULT res = SUGeometryInputRelease(&m_geometry_input);
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
  	SU_RESULT res = SUGeometryInputRelease(&m_geometry_input);
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


size_t GeometryInput::num_faces() const {
	return m_num_faces;
}


std::vector<std::pair<size_t, Face>> GeometryInput::faces() const {
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
  SULoopInputRef loop_input = SU_INVALID;
	SU_RESULT res =  SULoopInputCreate(&loop_input);
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
  res = SUGeometryInputFaceSetLayer(m_geometry_input, added_face_index, face.layer().ref());
	assert(res == SU_ERROR_NONE);
  // Set Materials TODO: not done quite right here, I don't think... uv coords are not set.
  SUMaterialInput material = MaterialInput(face.material()).ref();
  res = SUGeometryInputFaceSetFrontMaterial(m_geometry_input, added_face_index, &material);
	assert(res == SU_ERROR_NONE);
  SUMaterialInput back_material = MaterialInput(face.back_material()).ref();
  res = SUGeometryInputFaceSetBackMaterial(m_geometry_input, added_face_index, &back_material);
	assert(res == SU_ERROR_NONE);
  // There are further properties that need to be added, such as attributes, and these must be added after the entities::fill() operation.
  m_faces.push_back(std::pair<size_t, Face> (added_face_index, face));
  ++m_num_faces;
  return added_face_index;
}


size_t GeometryInput::add_faces(const std::vector<Face>& faces) {
	size_t index;
  for (size_t i=0; i < faces.size(); ++i) {
  	index = add_face(faces[i]);
  }
  return index;
}


size_t GeometryInput::add_edge(const Edge &edge) {
  // TODO: SUGeometryInputAddEdge only works since 2016.  So skip this for now.
  return 0;
  // Edges can only be added since SU2017
  /*
  size_t major;
  size_t minor;
  SUGetAPIVersion(&major, &minor);
  if (major < 17) {
    return 0;
  }
  */
  size_t start_vertex_index = m_vertex_index;
  SUPoint3D start_point = edge.start().position();
  SU_RESULT res = SUGeometryInputAddVertex(m_geometry_input, &start_point);
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
	size_t index;
  for (size_t i=0; i < edges.size(); ++i) {
  	index = add_edge(edges[i]);
  }
  return index;
}


bool GeometryInput::empty() const {
	if (m_faces.size() == 0 && m_edges.size() == 0) {
  	return true;
  }
  return false;
}

} /* namespace CW */
