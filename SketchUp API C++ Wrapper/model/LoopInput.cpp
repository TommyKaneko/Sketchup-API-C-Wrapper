//
//  LoopInput.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 08/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "LoopInput.hpp"

namespace CW {

LoopInput::LoopInput():
	m_loop_input(create_loop_input_ref()),
  m_release_on_destroy(true)
{
}

LoopInput::LoopInput(std::vector<Point3D> points):
	m_loop_input(create_loop_input_ref()),
  m_release_on_destroy(true)
{
	add_vertices(points);
}

LoopInput::LoopInput(SULoopInputRef loop_input, bool release_on_destroy):
	m_loop_input(loop_input),
  m_release_on_destroy(release_on_destroy)
{
}

SU_RESULT LoopInput::add_to_geometry_input(GeometryInput &geom_input) {
  std::vector<Point3D> vertices = get_vertices();
  
  std::vector<size_t> indices = geom_input.add_vertices(vertices);
  
  
  for (size_t i=0; i < indices.size(); i++) {
    SULoopInputEdgeSetHidden(m_loop_input, indices[i], m_edges[i].hidden());
    SULoopInputEdgeSetSoft(m_loop_input, indices[i], m_edges[i].soft());
    SULoopInputEdgeSetSmooth(m_loop_input, indices[i], m_edges[i].smooth());
  }
  return SU_ERROR_NONE;
}
  
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

SULoopInputRef create_loop_input_ref() {
	SULoopInputRef loop_input = SU_INVALID;
	SULoopInputCreate(&loop_input);
	return loop_input;
}



SULoopInputRef LoopInput::ref() {
	return m_loop_input;
}
  
} /* namespace CW */