//
//  Loop.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "Loop.hpp"
#include "LoopInput.hpp"
#include "Vertex.hpp"

#include <cassert>

namespace CW {

Loop::Loop(SULoopRef loop):
  m_loop(loop)
{}


LoopInput Loop::loop_input() {
  std::vector<Vertex> vertices = this->vertices();
  std::vector<Point3D> points;
  points.reserve(vertices.size());
  for (size_t i=0; i < vertices.size(); ++i) {
    points.push_back(vertices[i].position());
  }
  return LoopInput(points);
}


std::vector<Edge> Loop::edges() {
  size_t count;
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


std::vector<Vertex> Loop::vertices() {
  size_t count;
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
  
  
} /* namespace CW */
