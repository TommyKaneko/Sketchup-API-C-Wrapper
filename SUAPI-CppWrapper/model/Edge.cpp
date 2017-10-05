//
//  Edge.cpp
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

#include "SUAPI-CppWrapper/model/Edge.hpp"

#include <cassert>

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
  SU_RESULT res = SUEdgeCreate(&edge, &start_ref, &end_ref);
  assert(res == SU_ERROR_NONE);
  return edge;
}


SUEdgeRef Edge::copy_reference(const Edge& other) {
	if (other.m_attached) {
  	return other.m_edge;
  }
	SUEdgeRef new_edge = create_edge(other.start().position(), other.end().position());
  return new_edge;
}

/*****************************
* Constructors / Destructor **
******************************/
Edge::Edge():
	DrawingElement(SU_INVALID, true),
  m_edge(SU_INVALID)
{}


Edge::Edge(const std::vector<Point3D>& points):
	Edge(create_edge(points[0], points[1]), false)
{
}


Edge::Edge(const Point3D& start, const Point3D& end):
	Edge(create_edge(start, end), false)
{}


Edge::Edge(const Vertex& start, const Vertex& end):
	Edge(start.position(), end.position())
{}


Edge::Edge(SUEdgeRef edge, bool attached):
  DrawingElement(SUEdgeToDrawingElement(edge), attached),
	m_edge(edge)
{}


Edge::Edge(const Edge& other):
	DrawingElement(other, SUEdgeToDrawingElement(copy_reference(other))),
  m_edge(SUEdgeFromDrawingElement(m_drawing_element))
{
	if (!other.m_attached && SUIsValid(other.m_edge)) {
    this->color(other.color());
    this->smooth(other.smooth());
    this->soft(other.soft());
  }
}


Edge::~Edge() {
	if (!m_attached && SUIsValid(m_edge)) {
  	SU_RESULT res = SUEdgeRelease(&m_edge);
    assert(res == SU_ERROR_NONE);
  }
}

/******************
* Public Methods **
*******************/
Edge& Edge::operator=(const Edge& other) {
  if (!m_attached && SUIsValid(m_edge)) {
    SU_RESULT res = SUEdgeRelease(&m_edge);
    assert(res == SU_ERROR_NONE);
  }
  m_edge = copy_reference(other);
  m_drawing_element = SUEdgeToDrawingElement(m_edge);
  DrawingElement::operator=(other);
  if (!other.m_attached) {
    this->color(other.color());
    this->smooth(other.smooth());
    this->soft(other.soft());
  }
  return *this;
}


SUEdgeRef Edge::ref() const {
	return m_edge;
}

Edge::operator SUEdgeRef() const {
	return m_edge;
}

Edge::operator SUEdgeRef*() {
	return &m_edge;
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
	if (SUIsInvalid(m_edge)) {
  	return true;
  }
  return false;
}


Color Edge::color() const {
  if (!(*this)) {
  	throw std::logic_error("CW::Edge::color(): Edge is null");
  }
	SUColor color = SU_INVALID;
	SUEdgeGetColor(m_edge, &color);
  return Color(color);
}


bool Edge::color(const Color& input_color) {
  if (!(*this)) {
  	throw std::logic_error("CW::Edge::color(): Edge is null");
  }
	SUColor color = input_color.ref();
	SU_RESULT result = SUEdgeSetColor(m_edge, &color);
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
  SUEdgeGetEndVertex(m_edge, &vertex);
  return Vertex(vertex);
}


std::vector<Face> Edge::faces() const {
  if (!(*this)) {
  	throw std::logic_error("CW::Edge::faces(): Edge is null");
  }
	size_t count = 0;
	SU_RESULT res = SUEdgeGetNumFaces(m_edge, &count);
  SUFaceRef faces[count];
  assert(res == SU_ERROR_NONE);
	if (count == 0) {
  	return std::vector<Face>();
  }
	res = SUEdgeGetFaces(m_edge, count, &faces[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Face> return_faces;
  return_faces.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	return_faces.push_back(Face(faces[i]));
  }
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
	SUEdgeGetSmooth(m_edge, &smooth_flag);
	return smooth_flag;
}


bool Edge::smooth(bool smooth) {
  if (!(*this)) {
  	throw std::logic_error("CW::Edge::smooth(): Edge is null");
  }
	SU_RESULT result = SUEdgeSetSmooth(m_edge, smooth);
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
	SUEdgeGetSoft(m_edge, &soft_flag);
	return soft_flag;
}


bool Edge::soft(bool soft) {
  if (!(*this)) {
  	throw std::logic_error("CW::Edge::soft(): Edge is null");
  }
	SU_RESULT result = SUEdgeSetSoft(m_edge, soft);
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
  SUEdgeGetStartVertex(m_edge, &vertex);
  return Vertex(vertex);
}


} /* namespace CW */
