//
//  Loop.cpp
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

#include "Loop.hpp"

#include "LoopInput.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"

#include <cassert>

namespace CW {

Loop::Loop(SULoopRef loop):
  m_loop(loop)
{}


Loop::Loop():
	m_loop(SU_INVALID)
{}


bool Loop::operator!() const {
	return SUIsInvalid(m_loop);
}


LoopInput Loop::loop_input() const {
	std::vector<Edge> edges = this->edges();
  return LoopInput (this->edges());
}


std::vector<Edge> Loop::edges() const {
  assert(!!(*this));
  size_t count = 0;
  SU_RESULT res = SULoopGetNumVertices(m_loop, &count);
  assert(res == SU_ERROR_NONE);
  SUEdgeRef edge_array[count];
  res = SULoopGetEdges(m_loop, count, &edge_array[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Edge> edges;
  edges.reserve(count);
  for (size_t i=0; i < count; ++i) {
    edges.push_back(Edge(edge_array[i]));
  }
  return edges;
}


std::vector<Vertex> Loop::vertices() const {
  assert(!!(*this));
  size_t count = 0;
  SU_RESULT res = SULoopGetNumVertices(m_loop, &count);
  assert(res == SU_ERROR_NONE);
  SUVertexRef verts_array[count];
  res = SULoopGetVertices(m_loop, count, &verts_array[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Vertex> vertices;
  vertices.reserve(count);
  for (size_t i=0; i < count; ++i) {
    vertices.push_back(Vertex(verts_array[i]));
  }
  return vertices;
}

std::vector<Point3D> Loop::points() const {
	std::vector<Vertex> verts = vertices();
  std::vector<Point3D> points;
  points.reserve(verts.size());
  for (size_t i=0; i < verts.size(); ++i) {
  	points.push_back(verts[i].position());
  }
  return points;
}


size_t Loop::size() const {
  size_t count = 0;
  SU_RESULT res = SULoopGetNumVertices(m_loop, &count);
  assert(res == SU_ERROR_NONE);
	return count;
}


SULoopRef Loop::ref() const {
	return m_loop;
}
  
} /* namespace CW */
