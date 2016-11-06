//
//  GeometryInput.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 04/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include <cassert>

#include "GeometryInput.hpp"
#include "LoopInput.hpp"

namespace CW {

GeometryInput::GeometryInput():
	m_geometry_input(create_geometry_input())
{
}

GeometryInput::~GeometryInput() {
	SUGeometryInputRelease(&m_geometry_input);
}


SUGeometryInputRef GeometryInput::create_geometry_input() {
	SUGeometryInputRef geom_input = SU_INVALID;
	SUGeometryInputCreate(&geom_input);
  return geom_input;
}


size_t GeometryInput::add_vertex(Point3D vertex) {
  SUPoint3D vertex_ref = vertex;
  SUGeometryInputAddVertex(m_geometry_input, &vertex_ref);
  return m_vertex_index++;
}

std::vector<size_t> GeometryInput::add_vertices(std::vector<Point3D> vertices) {
  std::vector<size_t> indices;
  for (size_t i = 0; i < vertices.size(); i++) {
  	indices.push_back(add_vertex(vertices[i]));
  }
  return indices;
}


SU_RESULT GeometryInput::add_loop(LoopInput &loop) {
  std::vector<Point3D> vertices = loop.get_vertices();
  
  std::vector<size_t> indices = add_vertices(vertices);
  std::vector<Edge> edges = loop.get_edges();

  for (size_t i=0; i < indices.size(); i++) {
    SULoopInputEdgeSetHidden(loop.ref(), indices[i], edges[i].hidden());
    SULoopInputEdgeSetSoft(loop.ref(), indices[i], edges[i].soft());
    SULoopInputEdgeSetSmooth(loop.ref(), indices[i], edges[i].smooth());
  }
  return SU_ERROR_NONE;
}


size_t GeometryInput::add_face(Face &face) {
	//SULoopInputRef outer_loop = face.outer_loop().loop_input().ref();
	LoopInput outer_loop = face.outer_loop().loop_input();
  SU_RESULT res = add_loop(outer_loop);
  assert(res == SU_ERROR_NONE);
  SULoopInputRef outer_loop_ref = outer_loop.ref();
  size_t added_face_index;
  res = SUGeometryInputAddFace(m_geometry_input, &outer_loop_ref, &added_face_index);
  assert(res == SU_ERROR_NONE);
  // Add inner_loops
  std::vector<Loop> inner_loops = face.inner_loops();
  for (size_t i = 0; i < inner_loops.size(); ++i) {
  	LoopInput inner_loop = inner_loops[i].loop_input();
    res = add_loop(inner_loop);
	  assert(res == SU_ERROR_NONE);
    SULoopInputRef loop_input = inner_loop.ref();
    res = SUGeometryInputFaceAddInnerLoop(m_geometry_input, added_face_index, &loop_input);
	  assert(res == SU_ERROR_NONE);
	}
  // TODO: deallocate LoopInputRef?  It doesn't FEEL necessary because it has been added to the Geometry Input object. Needs testing.
  // Add layer
  SUGeometryInputFaceSetLayer(m_geometry_input, added_face_index, face.layer().ref());
  // Set Materials
  SUMaterialInput material = face.material().input().ref();
  res = SUGeometryInputFaceSetFrontMaterial(m_geometry_input, added_face_index, &material);
  SUMaterialInput back_material = face.back_material().input().ref();
  res = SUGeometryInputFaceSetBackMaterial(m_geometry_input, added_face_index, &back_material);
	
  return added_face_index;
}


} /* namespace CW */
