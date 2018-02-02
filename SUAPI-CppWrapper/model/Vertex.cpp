//
//  Vertex.cpp
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

#include "SUAPI-CppWrapper/model/Vertex.hpp"

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
  if (!(*this)) {
    throw std::logic_error("CW::Vertex::position(): Vertex is null");
  }
  SUPoint3D point;
  SUResult res = SUVertexGetPosition(m_vertex, &point);
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
