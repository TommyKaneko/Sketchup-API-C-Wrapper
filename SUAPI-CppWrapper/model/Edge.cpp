//
//  Edge.cpp
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

#include "SUAPI-CppWrapper/model/Edge.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"

namespace CW {
/**************************
* Private static methods **
***************************/
SUEdgeRef Edge::create_edge(const Point3D& start, const Point3D& end) {
  if (!start || !end) {
    throw std::invalid_argument("CW::Edge::create_edge(): given Point3D is null");
  }
  if (start == end) {
    throw std::invalid_argument("CW::Edge::create_edge(): start and end points are equal - cannot create edge of zero length");
  }
  SUEdgeRef edge = SU_INVALID;
  SUPoint3D start_ref = start;
  SUPoint3D end_ref = end;
  SUResult res = SUEdgeCreate(&edge, &start_ref, &end_ref);
  assert(res == SU_ERROR_NONE);
  return edge;
}


SUEdgeRef Edge::copy_reference(const Edge& other) {
  if (other.m_attached) {
    return other.ref();
  }
  SUEdgeRef new_edge = create_edge(other.start().position(), other.end().position());
  return new_edge;
}

/*****************************
* Constructors / Destructor **
******************************/
Edge::Edge():
  DrawingElement(SU_INVALID, true)
{}


Edge::Edge(const std::vector<Point3D>& points):
  Edge(create_edge(points[0], points[1]), false)
{}


Edge::Edge(const Point3D& start, const Point3D& end):
  Edge(create_edge(start, end), false)
{}


Edge::Edge(const Vertex& start, const Vertex& end):
  Edge(start.position(), end.position())
{}


Edge::Edge(SUEdgeRef edge, bool attached):
  DrawingElement(SUEdgeToDrawingElement(edge), attached)
{}


Edge::Edge(const Edge& other):
  DrawingElement(other, SUEdgeToDrawingElement(copy_reference(other)))
{
  if (!other.m_attached && SUIsValid(other.ref())) {
    this->color(other.color());
    this->smooth(other.smooth());
    this->soft(other.soft());
  }
}


Edge::~Edge() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUEdgeRef edge = this->ref();
    SUResult res = SUEdgeRelease(&edge);
    assert(res == SU_ERROR_NONE);
  }
}

/******************
* Public Methods **
*******************/
Edge& Edge::operator=(const Edge& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUEdgeRef edge = this->ref();
    SUResult res = SUEdgeRelease(&edge);
    assert(res == SU_ERROR_NONE);
  }
  m_entity = SUEdgeToEntity(copy_reference(other));
  if (!other.m_attached) {
    this->color(other.color());
    this->smooth(other.smooth());
    this->soft(other.soft());
  }
  DrawingElement::operator=(other);
  return *this;
}


SUEdgeRef Edge::ref() const {
  return SUEdgeFromEntity(m_entity);
}

Edge::operator SUEdgeRef() const {
  return this->ref();
}

Edge::operator SUEdgeRef*() {
  // TODO: test if the solution below works.
  SUEdgeRef edge = this->ref();
  return &edge;
}
  
/*
Edge::operator bool() const {
  if (m_create_result == SU_ERROR_NONE) {
    return true;
  }
  return false;
}
*/

bool Edge::operator!() const {
  if (SUIsInvalid(m_entity)) {
    return true;
  }
  return false;
}


Color Edge::color() const {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::color(): Edge is null");
  }
  SUColor color = SU_INVALID;
  SUEdgeGetColor(this->ref(), &color);
  return Color(color);
}


bool Edge::color(const Color& input_color) {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::color(): Edge is null");
  }
  SUColor color = input_color.ref();
  SUResult result = SUEdgeSetColor(this->ref(), &color);
  if (result == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


Vertex Edge::end() const {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::end(): Edge is null");
  }
  SUVertexRef vertex = SU_INVALID;
  SUEdgeGetEndVertex(this->ref(), &vertex);
  return Vertex(vertex);
}


std::vector<Face> Edge::faces() const {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::faces(): Edge is null");
  }
  size_t count = 0;
  SUResult res = SUEdgeGetNumFaces(this->ref(), &count);
  SUFaceRef* faces = new SUFaceRef[count];
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Face>();
  }
  res = SUEdgeGetFaces(this->ref(), count, &faces[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Face> return_faces;
  return_faces.reserve(count);
  for (size_t i=0; i < count; ++i) {
    return_faces.push_back(Face(faces[i]));
  }
  delete faces;
  return return_faces;
}

Vector3D Edge::vector() const {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::vector(): Edge is null");
  }
  return end().position() - start().position();
}


bool Edge::smooth() const {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::smooth(): Edge is null");
  }
  bool smooth_flag;
  SUEdgeGetSmooth(this->ref(), &smooth_flag);
  return smooth_flag;
}


bool Edge::smooth(bool smooth) {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::smooth(): Edge is null");
  }
  SUResult result = SUEdgeSetSmooth(this->ref(), smooth);
  if (result == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


bool Edge::soft() const {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::soft(): Edge is null");
  }
  bool soft_flag;
  SUEdgeGetSoft(this->ref(), &soft_flag);
  return soft_flag;
}


bool Edge::soft(bool soft) {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::soft(): Edge is null");
  }
  SUResult result = SUEdgeSetSoft(this->ref(), soft);
  if (result == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


Vertex Edge::start() const {
  if (!(*this)) {
    throw std::logic_error("CW::Edge::start(): Edge is null");
  }
  SUVertexRef vertex = SU_INVALID;
  SUEdgeGetStartVertex(this->ref(), &vertex);
  return Vertex(vertex);
}


} /* namespace CW */
