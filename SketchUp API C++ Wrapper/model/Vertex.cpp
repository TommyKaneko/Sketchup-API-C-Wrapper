//
//  Vertex.cpp
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
  m_entity = SUVertexToEntity(m_vertex);
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
