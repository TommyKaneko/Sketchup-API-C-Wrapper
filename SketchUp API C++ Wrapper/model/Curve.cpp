//
//  Curve.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 08/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//
#include <cassert>

#include "Curve.hpp"

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

SUCurveRef Curve::create_curve(std::vector<Edge> edges, SU_RESULT &result) {
	SUCurveRef curve_ref = SU_INVALID;
  SUEdgeRef edge_array[edges.size()];
  for (size_t i=0; i < edges.size(); i++) {
  	edge_array[i] = edges[i].ref();
  }
  result = SUCurveCreateWithEdges(&curve_ref, &edge_array[0], edges.size());
  return curve_ref;
}


SUCurveRef Curve::ref(){ return m_curve;}


std::vector<Edge> Curve::get_edges() const{
	size_t num_edges;
  SU_RESULT res = SUCurveGetNumEdges(m_curve, &num_edges);
  assert(res == SU_ERROR_NONE);
  SUEdgeRef ref_edges[num_edges];
  res = SUCurveGetEdges(m_curve, num_edges, &ref_edges[0], &num_edges);
  
  std::vector<Edge> edges(num_edges);
  for (size_t i=0; i < num_edges; ++i) {
    edges[i] = Edge{ref_edges[i]};
  }
  return edges;
}


SUCurveType Curve::get_type() {
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