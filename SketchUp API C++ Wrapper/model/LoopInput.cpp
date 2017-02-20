//
//  LoopInput.cpp
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

#include "LoopInput.hpp"

#include <cassert>

#include "Edge.hpp"
#include "Curve.hpp"
#include "GeometryInput.hpp"
#include "Material.hpp"
#include "Layer.hpp"

namespace CW {

SULoopInputRef LoopInput::create_loop_input_ref() {
	SULoopInputRef loop_input = SU_INVALID;
	SULoopInputCreate(&loop_input);
	return loop_input;
}

/******************************
** Constructors / Destructor **
*******************************/
LoopInput::LoopInput():
	m_loop_input(create_loop_input_ref())
{}


LoopInput::LoopInput(SULoopInputRef loop_input):
	m_loop_input(loop_input)
{}


LoopInput::LoopInput(const std::vector<Edge>& loop_edges,  size_t vertex_index):
	LoopInput()
{
	for (size_t i=0; i < loop_edges.size(); ++i) {
  	add_vertex_index(vertex_index);
    set_edge_hidden(i, loop_edges[i].hidden());
    set_edge_soft(i, loop_edges[i].soft());
    set_edge_smooth(i, loop_edges[i].smooth());
    Material edge_material = loop_edges[i].material();
    if (!!edge_material) {
    	set_edge_material(i, edge_material);
    }
    Layer edge_layer = loop_edges[i].layer();
    if (!!edge_layer) {
    	set_edge_layer(i, edge_layer);
    }
    ++vertex_index;
  }
}



LoopInput::LoopInput(const LoopInput& other):
	m_loop_input(create_loop_input_ref())
{
	// LoopInputRef cannot be copied across at this stage. If it is important, something can be done.
  assert(false);
}

LoopInput::~LoopInput() {
	if (SUIsValid(m_loop_input)) {
  	SULoopInputRelease(&m_loop_input);
  }
}

/*******************
** Public Methods **
********************/
LoopInput& LoopInput::operator=(const LoopInput& other) {
	// LoopInputRef cannot be copied across at this stage. If it is important, something can be done, but let's see.
  assert(false);
  /**
  size_t vertex_index = 0;
  SULoopInputRef other_loop_input = other.ref();
  while (SULoopInputEdgeSetHidden(other_loop_input, size_t edge_index, bool hidden))
	*/
}


SULoopInputRef LoopInput::ref() const {
	return m_loop_input;
}

LoopInput::operator SULoopInputRef() const {
	return ref();
}

LoopInput::operator SULoopInputRef*() {
	return &m_loop_input;
}

LoopInput::operator bool() const {
	if (SUIsInvalid(m_loop_input)) {
  	return false;
  }
  if (m_edge_num > 2) {
  	// TODO: this is a little suspect, as m_edge_num is not correct if the LoopInputRef was created before initialising this object.
    return true;
  }
  return false;
}


LoopInput& LoopInput::add_vertex_index(const size_t index) {
	SU_RESULT res = SULoopInputAddVertexIndex(m_loop_input, index);
	assert(res == SU_ERROR_NONE);
  m_edge_num++;
	return (*this);
}


LoopInput& LoopInput::set_edge_hidden(const size_t edge_index, const bool hidden) {
	SU_RESULT res = SULoopInputEdgeSetHidden(m_loop_input, edge_index, hidden);
  assert(res == SU_ERROR_NONE);
  return (*this);
}

LoopInput& LoopInput::set_edge_soft(const size_t edge_index, const bool soft) {
	SU_RESULT res = SULoopInputEdgeSetSoft(m_loop_input, edge_index, soft);
  assert(res == SU_ERROR_NONE);
  return (*this);
}

LoopInput& LoopInput::set_edge_smooth(const size_t edge_index, const bool smooth) {
	SU_RESULT res = SULoopInputEdgeSetSoft(m_loop_input, edge_index, smooth);
  assert(res == SU_ERROR_NONE);
  return (*this);
}

LoopInput& LoopInput::set_edge_material(const size_t edge_index, const Material& material) {
	SU_RESULT res = SULoopInputEdgeSetMaterial(m_loop_input, edge_index, material.ref());
  assert(res == SU_ERROR_NONE);
  return (*this);}

LoopInput& LoopInput::set_edge_layer(const size_t edge_index, const Layer& layer) {
	SU_RESULT res = SULoopInputEdgeSetLayer(m_loop_input, edge_index, layer.ref());
  assert(res == SU_ERROR_NONE);
  return (*this);
}



/*
LoopInput& LoopInput::add_vertices(std::vector<Point3D> points)
{
  for (size_t i=0; i< points.size(); ++i) {
  	if (i == (points.size()-1)) {
      m_edges.push_back(Edge(points[i], points[0]));
    }
    else {
      m_edges.push_back(Edge(points[i], points[i+1]));
    }
  }
	return *this;
}
*/



/*
* Adds an edge on the end of the Loop
*/
/*
LoopInput& add_edge(Edge edge);
LoopInput& add_edges(std::vector<Edge> edges);

//LoopInput& add_curve(Curve curve);
//LoopInput& add_curves(std::vector<Curve> curves);
*/

} /* namespace CW */