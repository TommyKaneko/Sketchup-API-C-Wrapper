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

#include "SUAPI-CppWrapper/model/LoopInput.hpp"

#include <cassert>
#include <iostream>

#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Curve.hpp"
#include "SUAPI-CppWrapper/model/GeometryInput.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"

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


LoopInput::LoopInput(SULoopInputRef loop_input, bool attached):
	m_loop_input(loop_input),
  m_attached(attached)
{}

/*
LoopInput::LoopInput(std::vector<Edge> loop_edges,  size_t vertex_index):
	LoopInput()
{
	for (size_t i=0; i < loop_edges.size(); ++i) {
  	add_vertex_index(vertex_index);
    if (SU_API_VERSION_MAJOR >= 5) {
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
    }
    ++vertex_index;
  }
}
*/

LoopInput::LoopInput(const std::vector<InputEdgeProperties>& loop_edge_properties, size_t vertex_index):
	LoopInput()
{
	for (size_t i=0; i < loop_edge_properties.size(); ++i) {
  	add_vertex_index(vertex_index);
    if (SU_API_VERSION_MAJOR >= 5) {
      set_edge_hidden(i, loop_edge_properties[i].hidden);
      set_edge_soft(i, loop_edge_properties[i].soft);
      set_edge_smooth(i, loop_edge_properties[i].smooth);
      Material edge_material = loop_edge_properties[i].material;
      if (!!edge_material) {
        set_edge_material(i, edge_material);
      }
      Layer edge_layer = loop_edge_properties[i].layer;
      if (!!edge_layer) {
        set_edge_layer(i, edge_layer);
      }
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
	if (!m_attached && SUIsValid(m_loop_input)) {
  	SU_RESULT res = SULoopInputRelease(&m_loop_input);
    assert(res == SU_ERROR_NONE);
  }
}

/*******************
** Public Methods **
********************/
LoopInput& LoopInput::operator=(const LoopInput& other) {
  // There is no easy way to copy a LoopInput object.
  assert(false);
  // Note that this is a crude copying operation - we are simply creating a reference to the other object.
  //m_loop_input = other.m_loop_input;
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
  if(!(*this)) {
  	throw std::logic_error("CW::LoopInput::add_vertex_index(): LoopInput is null");
  }
	SU_RESULT res = SULoopInputAddVertexIndex(m_loop_input, index);
	assert(res == SU_ERROR_NONE);
  m_edge_num++;
	return (*this);
}


LoopInput& LoopInput::set_edge_hidden(const size_t edge_index, const bool hidden) {
  if(!(*this)) {
  	throw std::logic_error("CW::LoopInput::set_edge_hidden(): LoopInput is null");
  }
	SU_RESULT res = SULoopInputEdgeSetHidden(m_loop_input, edge_index, hidden);
  if(res == SU_ERROR_OUT_OF_RANGE) {
  	throw std::invalid_argument("CW::LoopInput::set_edge_hidden(): edge_index is larger than the number of vertices in the LoopInput");
  }
  assert(res == SU_ERROR_NONE);
  return (*this);
}

LoopInput& LoopInput::set_edge_soft(const size_t edge_index, const bool soft) {
  if(!(*this)) {
  	throw std::logic_error("CW::LoopInput::set_edge_soft(): LoopInput is null");
  }
	SU_RESULT res = SULoopInputEdgeSetSoft(m_loop_input, edge_index, soft);
  if(res == SU_ERROR_OUT_OF_RANGE) {
  	throw std::invalid_argument("CW::LoopInput::set_edge_soft(): edge_index is larger than the number of vertices in the LoopInput");
  }
  assert(res == SU_ERROR_NONE);
  return (*this);
}

LoopInput& LoopInput::set_edge_smooth(const size_t edge_index, const bool smooth) {
  if(!(*this)) {
  	throw std::logic_error("CW::LoopInput::set_edge_smooth(): LoopInput is null");
  }
	SU_RESULT res = SULoopInputEdgeSetSoft(m_loop_input, edge_index, smooth);
  if(res == SU_ERROR_OUT_OF_RANGE) {
  	throw std::invalid_argument("CW::LoopInput::set_edge_smooth(): edge_index is larger than the number of vertices in the LoopInput");
  }
  assert(res == SU_ERROR_NONE);
  return (*this);
}

LoopInput& LoopInput::set_edge_material(const size_t edge_index, const Material& material) {
  if(!(*this)) {
  	throw std::logic_error("CW::LoopInput::set_edge_material(): LoopInput is null");
  }
	SU_RESULT res = SULoopInputEdgeSetMaterial(m_loop_input, edge_index, material.ref());
  if(res == SU_ERROR_OUT_OF_RANGE) {
  	throw std::invalid_argument("CW::LoopInput::set_edge_material(): edge_index is larger than the number of vertices in the LoopInput");
  }
  assert(res == SU_ERROR_NONE);
  return (*this);}

LoopInput& LoopInput::set_edge_layer(const size_t edge_index, const Layer& layer) {
  if(!(*this)) {
  	throw std::logic_error("CW::LoopInput::set_edge_layer(): LoopInput is null");
  }
	SU_RESULT res = SULoopInputEdgeSetLayer(m_loop_input, edge_index, layer.ref());
  if(res == SU_ERROR_OUT_OF_RANGE) {
  	throw std::invalid_argument("CW::LoopInput::set_edge_layer(): edge_index is larger than the number of vertices in the LoopInput");
  }
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
