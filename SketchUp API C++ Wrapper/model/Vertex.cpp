//
//  Vertex.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "Vertex.hpp"

#include <cassert>

namespace CW {


Vertex::Vertex(SUVertexRef vertex):
  Entity(SUVertexToEntity(vertex)),
  m_vertex(vertex)
{}


Point3D Vertex::position() const {
  SUPoint3D point;
  SU_RESULT res = SUVertexGetPosition(m_vertex, &point);
  assert(res == SU_ERROR_NONE);
  return Point3D(point);
}


Vertex::operator SUVertexRef() {
  return m_vertex;
}

Vertex::operator SUVertexRef*() {
  return &m_vertex;
}

Vertex::operator SUPoint3D() const {
  return position();
}

Vertex::operator Point3D() const {
  return position();
}

} /* namespace CW */
