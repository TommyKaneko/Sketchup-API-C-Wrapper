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
/******************************
** Private Static Methods *****
*******************************/
SUVertexRef Vertex::copy_reference(const Vertex& other) {
	// As vertices cannot be "copied", we simply pass the existing reference.
  return other.m_vertex;
}

/******************************
** Constructors / Destructor **
*******************************/
Vertex::Vertex():
	Entity(),
  m_vertex(SU_INVALID)
{}


Vertex::Vertex(SUVertexRef vertex):
  Entity(SUVertexToEntity(vertex), true),
  m_vertex(vertex)
{}


Vertex::Vertex(const Vertex& other):
	Entity(other, SUVertexToEntity(copy_reference(other))),
  m_vertex(SUVertexFromEntity(m_entity))
{}


Vertex::~Vertex() {
	// Vertices cannot be released, as they are elements that are attached to something else.
}


Vertex& Vertex::operator=(const Vertex& other) {
	// Simply assign the other vertex to this object.
	m_vertex = other.m_vertex;
  Entity::operator=(other);
  return (*this);
}


/*******************
** Public Methods **
********************/
Point3D Vertex::position() const {
  SUPoint3D point;
  SU_RESULT res = SUVertexGetPosition(m_vertex, &point);
  assert(res == SU_ERROR_NONE);
  return Point3D(point);
}


Vertex::operator SUVertexRef() const {
  return ref();
}

Vertex::operator SUVertexRef*() {
  return &m_vertex;
}

SUVertexRef Vertex::ref() const {
	return m_vertex;
}

Vertex::operator SUPoint3D() const {
  return position();
}

Vertex::operator Point3D() const {
  return position();
}

} /* namespace CW */
