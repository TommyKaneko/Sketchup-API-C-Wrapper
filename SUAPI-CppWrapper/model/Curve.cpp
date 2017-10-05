//
//  Curve.cpp
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
#include <cassert>

#include "SUAPI-CppWrapper/model/Curve.hpp"

namespace CW {


Curve::Curve(std::vector<Edge> edges, SUCurveType curve_type):
	Curve(create_curve(edges, m_create_result), curve_type)
{
}

Curve::Curve(SUCurveRef curve,  SUCurveType curve_type):
	Entity(SUCurveToEntity(curve)),
	m_curve(curve),
  m_curve_type(curve_type)
{
}

Curve::~Curve(){
}

SUCurveRef Curve::create_curve(std::vector<Edge>& edges, SU_RESULT &result) {
	SUCurveRef curve_ref = SU_INVALID;
  result = SUCurveCreateWithEdges(&curve_ref, edges[0], edges.size());
  return curve_ref;
}


SUCurveRef Curve::ref(){ return m_curve;}


std::vector<Edge> Curve::get_edges() const{
  if (!(*this)) {
  	throw std::logic_error("CW::Curve::get_edges(): Curve is null");
  }
	size_t num_edges = 0;
  SU_RESULT res = SUCurveGetNumEdges(m_curve, &num_edges);
  assert(res == SU_ERROR_NONE);
  SUEdgeRef ref_edges[num_edges];
  res = SUCurveGetEdges(m_curve, num_edges, &ref_edges[0], &num_edges);
  assert(res == SU_ERROR_NONE);
  std::vector<Edge> edges;
  edges.reserve(num_edges);
  for (size_t i=0; i < num_edges; ++i) {
    edges.push_back(Edge(ref_edges[i]));
  }
  return edges;
}


SUCurveType Curve::get_type() {
  if (!(*this)) {
  	throw std::logic_error("CW::Curve::get_type(): Curve is null");
  }
	SUCurveGetType(m_curve, &m_curve_type);
  return m_curve_type;
}
 

SU_RESULT Curve::get_result() const {
	return m_create_result;
}


Curve::operator bool() const {
	if (m_create_result == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


bool Curve::operator!() const {
	return !bool(this);
}

} /* namespace CW */
