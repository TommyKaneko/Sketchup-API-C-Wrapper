//
//  Face.cpp
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

#include <SketchUpAPI/model/edge.h>

#include "Face.hpp"

#include "Geometry.hpp"
#include "Material.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Loop.hpp"
#include "LoopInput.hpp"

namespace CW {

/**************************
* Private static methods **
***************************/
SUFaceRef Face::create_face(std::vector<Point3D>& outer_points) {
	LoopInput loop_input;
  for (size_t i=0; i < outer_points.size(); ++i) {
  	loop_input.add_vertex_index(i);
  }
  return create_face(outer_points, loop_input);
}

SUFaceRef Face::create_face(std::vector<Point3D>& outer_points, LoopInput& loop_input) {
	SUFaceRef face = SU_INVALID;
  SU_RESULT res = SUFaceCreate(&face, outer_points[0], loop_input);
  assert(res == SU_ERROR_NONE);
	return face;
}

/**
SUFaceRef Face::create_face(std::vector<Point3D> outer_loop, std::vector<std::vector<Point3D>> inner_loops) {
	SUFaceRef face = create_face(outer_loop);
  // Add inner loops
  if (SUIsValid(face)) {
    for (size_t i = 0; i < inner_loops.size(); ++i){
      size_t num_il_vertices = inner_loops[i].size();
      SUPoint3D vertices[num_il_vertices];
      LoopInput loop(inner_loops[i]);
      SULoopInputRef inner_loop_input = loop.ref();
      SU_RESULT res = SUFaceAddInnerLoop(face, &vertices[0], &inner_loop_input);
      assert(res == SU_ERROR_NONE);
    }
  }
	return face;
}
*/


SUFaceRef Face::copy_reference(const Face& other) {
	if (other.m_attached) {
  	return other.m_face;
  }
  // The other face has not been attached to the model, so copy its properties to a new object
  Loop other_outer_loop = other.outer_loop();
  std::vector<Point3D> other_outer_points = other_outer_loop.points();
	LoopInput outer_loop_input = other_outer_loop.loop_input();
  SUFaceRef new_face = create_face(other_outer_points, outer_loop_input);
  return new_face;
}

/*****************************
* Constructors / Destructor **
******************************/
Face::Face():
	DrawingElement(SU_INVALID, true),
	m_face(SU_INVALID)
{}


Face::Face(std::vector<Point3D>& outer_loop):
	Face(create_face(outer_loop), false)
{
	
}

Face::Face(std::vector<Point3D>& outer_loop, LoopInput& loop_input):
	Face(create_face(outer_loop, loop_input), false)
{
}
/*
Face::Face(std::vector<Point3D> outer_loop, std::vector<std::vector<Point3D>> inner_loops):
	Face(create_face(outer_loop, inner_loops), true)
{
}
*/

Face::Face(SUFaceRef face, bool attached):
	DrawingElement(SUFaceToDrawingElement(face), attached),
	m_face(face)
{}


/** Copy constructor */
Face::Face(const Face& other):
	DrawingElement(other, SUFaceToDrawingElement(copy_reference(other))),
  m_face(SUFaceFromDrawingElement(m_drawing_element))
{
	if (!other.m_attached && SUIsValid(other.m_face)) {
    // Add the inner loops
    std::vector<Loop> inner_loops = other.inner_loops();
    std::vector<std::vector<Point3D>> inner_loops_points;
    inner_loops_points.reserve(inner_loops.size());
    for (size_t i=0; i < inner_loops.size(); ++i) {
      std::vector<Point3D> inner_points = inner_loops[i].points();
      LoopInput inner_loop_input = inner_loops[i].loop_input();
      add_inner_loop(inner_points, inner_loop_input);
    }
    this->back_material(other.back_material());
  }
}


Face::~Face() {
	if (!m_attached && SUIsValid(m_face)) {
  	SU_RESULT res = SUFaceRelease(&m_face);
		assert(res == SU_ERROR_NONE);
  }
}

/******************
* Public Methods **
*******************/
/** Copy assignment operator */
Face& Face::operator=(const Face& other) {
  if (!m_attached && SUIsValid(m_face)) {
    SU_RESULT res = SUFaceRelease(&m_face);
    assert(res == SU_ERROR_NONE);
  }
  m_face = copy_reference(other);
  m_drawing_element = SUFaceToDrawingElement(m_face);
  DrawingElement::operator=(other);
  return *this;
}


/**
For alternative use of 
SUFaceRef create_face(LoopInput outer_loop, std::vector<LoopInput> inner_loops, SU_RESULT &create_result) {
	SUFaceRef face = SU_INVALID;
  size_t num_vertices = outer_loop.size();
  SUPoint3D* vertices[num_vertices];
  size_t j = 0;
  
  for (size_t i = 0; i < outer_loop.size(); ++i){
  	vertices[j] = outer_loop[i];
    ++j;
	}
  LoopInput loop_input{outer_loop};
  // TODO
  SULoopInputRef outer_loop_ref = outer_loop.ref();
  create_result = SUFaceCreate(&face, vertices[0], &loop_input_ref);
  // Add inner loops
  if (create_result == SU_ERROR_NONE) {
    for (size_t i = 0; i < inner_loops.size(); ++i){
      size_t num_il_vertices = inner_loops[i].size();
      SUPoint3D* vertices[num_il_vertices];
      LoopInput loop{inner_loops[i]};
      SULoopInputRef inner_loop_input = loop.ref();
      SU_RESULT res = SUFaceAddInnerLoop(face, vertices[0], &inner_loop_input);
      assert(res == SU_ERROR_NONE);
    }
  }
	return face;
}
**/



/*
SUFaceRef Face::check_face(SUFaceRef face, SU_RESULT &create_result) {
  
}
*/
  
SUFaceRef Face::ref() const {	return m_face; }
Face::operator SUFaceRef() const {	return m_face;}
Face::operator SUFaceRef*() {	return &m_face;}

bool Face::operator!() const {
	if (SUIsInvalid(m_face)) {
  	return true;
  }
  return false;
}


double Face::area() const {
	double area;
	SU_RESULT res = SUFaceGetArea(m_face, &area);
  assert(res == SU_ERROR_NONE);
  return area;
}


void Face::add_inner_loop(std::vector<Point3D>& points, LoopInput &loop_input) {
  SU_RESULT res = SUFaceAddInnerLoop(m_face, points[0], loop_input);
  assert(res == SU_ERROR_NONE);
}


Material Face::back_material() const {
	Material material{};
	SUFaceGetBackMaterial(m_face, material);
	return material;
}


Material Face::back_material(const Material& material) {
  SUFaceSetBackMaterial(m_face, material);
  return material;
}

/*
* determine if a given Point3d is on the referenced Face. The return value is calculated from this list:
  PointUnknown (indicates an error),
  PointInside (point is on the face, not in a hole),
  PointOnVertex (point touches a vertex),
  PointOnEdge (point is on an edge),
  PointOutside (point outside the face or in a hole),
  PointNotOnPlane (point off the face's plane).
* @param SUPoint3D object.
* @return FacePointClass enum indicating the status of the point relative to the face.
*/
FacePointClass Face::classify_point(const Point3D& point) {
	// TODO
}


std::vector<Edge> Face::edges() {
	std::vector<Edge> total_edges = outer_loop().edges();
  std::vector<Loop> all_loops = loops();
  size_t total_edge_num = 0;
  for (size_t i=0; i < all_loops.size(); i++) {
		total_edge_num += all_loops[i].size();
  }
  total_edges.reserve(total_edge_num);
  for (size_t i=0; i < all_loops.size(); i++) {
  	std::vector<Edge> loop_edges = all_loops[i].edges();
    total_edges.insert(total_edges.end(), loop_edges.begin(), loop_edges.end());
  }
  return total_edges;
}

/*
// TODO
UVHelper Face::get_UVHelper(bool front, bool back, TextureWriter tex_writer) {
	//SUUVHelperRef uv_helper = SU_INVALID;
  UVHelper uv_helper{};
  SUFaceGetUVHelper(m_face, front, back, tex_writer, uv_helper);
  return uv_helper;
}
*/


/*
* Returns a vector representing the projection for either the front or back side of the face.
* @param bool true for frontside, false for back side.
*/
Vector3D Face::get_texture_projection(const bool frontside) const {
	//TODO
}


std::vector<Loop> Face::inner_loops() const {
	size_t num_loops = 0;
	SUFaceGetNumInnerLoops(m_face, &num_loops);
  SULoopRef inner_loops[num_loops];
  SUFaceGetInnerLoops(m_face, num_loops, &inner_loops[0], &num_loops);
  std::vector<Loop> loops;
  loops.reserve(num_loops);
	for (size_t i=0; i < num_loops; i++) {
  	loops.push_back(Loop(inner_loops[i]));
  }
  return loops;
}


std::vector<Loop> Face::loops() const {
	std::vector<Loop> all_loops;
  all_loops.push_back(outer_loop());
  std::vector<Loop> inner_ls = inner_loops();
  all_loops.reserve(all_loops.size() + inner_ls.size());
	all_loops.insert(all_loops.end(), inner_ls.begin(), inner_ls.end());
	return all_loops;
}

/*
// TODO another day.
PolygonMesh mesh();
*/


Vector3D Face::normal() const {
	Plane3D c_plane = plane();
  return c_plane.normal();
}


Loop Face::outer_loop() const {
  SULoopRef lp = SU_INVALID;
  SU_RESULT res = SUFaceGetOuterLoop(m_face, &lp);
  assert(res == SU_ERROR_NONE);
  return Loop(lp);
}


Plane3D Face::plane() const {
	SUPlane3D plane = SU_INVALID;
	SUFaceGetPlane(m_face, &plane);
  return Plane3D(plane);
}

/*
* Positions a material on a face.
* The pt_array must contain 2, 4, 6 or 8 points. The points are used in pairs to tell where a point in the texture image is positioned on the Face. The first point in each pair is a 3D point in the model. It should be a point on the Face. The second point in each pair of points is a 2D point that gives the (u,v) coordinates of a point in the image to match up with the 3D point.
* @param Material object to position.
* @param vector of Point3d objects used to position the material.
* @param bool true to position the texture on the front of the Face or false to position it on the back of the Face.
*/
bool Face::position_material(const Material& material, const std::vector<Point3D>& pt_array, bool o_front) {
	// TODO
}


Face* Face::reverse() {
	SU_RESULT res = SUFaceReverse(m_face);
  if (res == SU_ERROR_NONE) {
  	return this;
  }
  else {
  	return nullptr;
  }
}

/*
* Sets the texture projection direction.
* @param SUVector3D object representing the direction of the projection. Or bool true to remove texture projection.
* @param bool true for front side, false for back side.
* @return true on success
*/
bool Face::set_texture_projection(const Vector3D& vector, bool frontside) {
	// TODO
}
bool Face::set_texture_projection(bool remove, bool frontside) {
	// TODO
}


std::vector<Vertex> Face::vertices() const {
	size_t num_vertices = 0;
	SUFaceGetNumVertices(m_face, &num_vertices);
  SUVertexRef vertices[num_vertices];
  SUFaceGetVertices(m_face, num_vertices, &vertices[0], &num_vertices);
  std::vector<Vertex> verts;
  verts.reserve(num_vertices);
  for (size_t i=0; i < num_vertices; i++) {
  	verts.push_back(Vertex(vertices[i]));
  }
  return verts;
}
  
} /* namespace CW */