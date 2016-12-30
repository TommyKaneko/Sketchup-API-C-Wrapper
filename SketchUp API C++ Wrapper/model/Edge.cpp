//
//  Edge.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "Edge.hpp"
#include "Color.hpp"
#include "Vertex.hpp"

namespace CW {

SUEdgeRef Edge::create_edge(std::vector<SUPoint3D> points, SU_RESULT &result) {
	SUEdgeRef edge = SU_INVALID;
  result = SUEdgeCreate(&edge, &points[0], &points[1]);
  return edge;
}

Edge::Edge(std::vector<SUPoint3D> points):
	Edge(create_edge(points, m_create_result), true)
{
}


Edge::Edge(SUPoint3D start, SUPoint3D end):
	Edge(std::vector<SUPoint3D>{start, end})
{
}


Edge::Edge(SUEdgeRef edge, bool release_on_destroy):
	m_edge(edge),
  m_release_on_destroy(release_on_destroy),
  DrawingElement(SUEdgeToDrawingElement(edge))
{
}

Edge::~Edge() {
	if (m_release_on_destroy) {
  	SUEdgeRelease(&m_edge);
  }
}


SUEdgeRef Edge::ref() const {
	return m_edge;
}

Edge::operator SUEdgeRef() {
	return m_edge;
}

Edge::operator SUEdgeRef*() {
	return &m_edge;
}
  

Edge::operator bool() const {
	if (m_create_result == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool Edge::operator!() const {
	return !bool(this);
}


Color Edge::color() const {
	SUColor color = SU_INVALID;
	SUEdgeGetColor(m_edge, &color);
  return Color(color);
}


bool Edge::color(Color input_color) {
	SUColor color = input_color.ref();
	SU_RESULT result = SUEdgeSetColor(m_edge, &color);
  if (result == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


Vertex Edge::end() const {
	SUVertexRef vertex = SU_INVALID;
  SUEdgeGetEndVertex(m_edge, &vertex);
  return Vertex(vertex);
}


SU_RESULT Edge::get_result() const {
	return m_create_result;
}


Vector3D Edge::vector() const {
	return end().position() - start().position();
}


bool Edge::smooth() const {
	bool smooth_flag;
	SUEdgeGetSmooth(m_edge, &smooth_flag);
	return smooth_flag;
}


bool Edge::smooth(bool smooth) {
	SU_RESULT result = SUEdgeSetSmooth(m_edge, smooth);
  if (result == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool Edge::soft() const {
	bool soft_flag;
	SUEdgeGetSoft(m_edge, &soft_flag);
	return soft_flag;
}


bool Edge::soft(bool soft) {
	SU_RESULT result = SUEdgeSetSoft(m_edge, soft);
  if (result == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


Vertex Edge::start() const {
	SUVertexRef vertex = SU_INVALID;
  SUEdgeGetStartVertex(m_edge, &vertex);
  return Vertex(vertex);
}


} /* namespace CW */