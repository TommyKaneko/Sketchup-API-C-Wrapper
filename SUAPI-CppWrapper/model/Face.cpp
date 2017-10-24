//
//  Face.cpp
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

#include <cassert>

#include <SketchUpAPI/model/edge.h>

#include "SUAPI-CppWrapper/model/Face.hpp"

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/LoopInput.hpp"

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
  SULoopInputRef loop_input_ref = loop_input.ref();
  SUPoint3D* su_points = new SUPoint3D[outer_points.size()];
  for (size_t i=0; i < outer_points.size(); i++) {
    su_points[i] = SUPoint3D(outer_points[i]);
  }
  SUResult res = SUFaceCreate(&face, &su_points[0], &loop_input_ref);
  if (res != SU_ERROR_NONE) {
  	// The points cannot be made into a face: either the points do not lie in a plane, or is somehow problematic.
    return SU_INVALID;
  }
  loop_input.m_attached = true;
  delete su_points;
	return face;
}


SUFaceRef Face::copy_reference(const Face& other) {
	if (other.m_attached || !other) {
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
	DrawingElement(SU_INVALID, false),
	m_face(SU_INVALID)
{}


Face::Face(std::vector<Point3D>& outer_loop):
	Face(create_face(outer_loop), false)
{}


Face::Face(std::vector<Point3D>& outer_loop, LoopInput& loop_input):
	Face(create_face(outer_loop, loop_input), false)
{}


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
  	SUResult res = SUFaceRelease(&m_face);
		assert(res == SU_ERROR_NONE);
  }
}

/******************
* Public Methods **
*******************/
/** Copy assignment operator */
Face& Face::operator=(const Face& other) {
  if (!m_attached && SUIsValid(m_face)) {
    SUResult res = SUFaceRelease(&m_face);
    assert(res == SU_ERROR_NONE);
  }
  m_face = copy_reference(other);
  m_drawing_element = SUFaceToDrawingElement(m_face);
  DrawingElement::operator=(other);
  return *this;
}

  
SUFaceRef Face::ref() const {	return m_face; }
Face::operator SUFaceRef() const {	return m_face;}
Face::operator SUFaceRef*() {	return &m_face;}

bool Face::operator!() const {
	if (SUIsInvalid(this->m_face)) {
  	return true;
  }
  return false;
}


double Face::area() const {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::area(): Face is null");
  }
	double area;
	SUResult res = SUFaceGetArea(m_face, &area);
  assert(res == SU_ERROR_NONE);
  return area;
}


void Face::add_inner_loop(std::vector<Point3D>& points, LoopInput &loop_input) {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::add_inner_loop(): Face is null");
  }
	if (points.size() != loop_input.m_edge_num) {
  	throw std::invalid_argument("CW::Face::add_inner_loop(): Unequal number of vertices between given Point3D vector and LoopInput object");
  }
  SUResult res = SUFaceAddInnerLoop(m_face, points[0], loop_input);
	if (res == SU_ERROR_INVALID_INPUT) {
  	throw std::invalid_argument("CW::Face::add_inner_loop(): Arguments are invalid");
  }
  assert(res == SU_ERROR_NONE);
}


Material Face::back_material() const {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::back_material(): Face is null");
  }
	SUMaterialRef material = SU_INVALID;
	SUResult res = SUFaceGetBackMaterial(m_face, &material);
  if (res == SU_ERROR_NO_DATA) {
  	return Material();
  }
  assert(res == SU_ERROR_NONE);
	return Material(material);
}


Material Face::back_material(const Material& material) {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::back_material(): Face is null");
  }
  SUResult res = SUFaceSetBackMaterial(m_face, material.ref());
  assert(res == SU_ERROR_NONE);
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
	if (!(*this)) {
  	throw std::logic_error("CW::Face::classify_point(): Face is null");
  }
	if (!point) {
  	throw std::invalid_argument("CW::Face::classify_point(): Given Point3D object is null");
  }
	Loop outer_loop = this->outer_loop();
  PointLoopClassify outer_class = outer_loop.classify_point(point);
  switch (outer_class) {
  	case (PointLoopClassify::PointUnknown):
    	return FacePointClass::PointUnknown;
    case (PointLoopClassify::PointNotOnPlane):
    	return FacePointClass::PointNotOnPlane;
    case (PointLoopClassify::PointOnEdge):
    	return FacePointClass::PointOnEdge;
    case (PointLoopClassify::PointOnVertex):
    	return FacePointClass::PointOnVertex;
    case (PointLoopClassify::PointOutside):
    	return FacePointClass::PointOutside;
    case (PointLoopClassify::PointInside): {
    	std::vector<Loop> inner_loops = this->inner_loops();
      for (size_t i=0; i < inner_loops.size(); i++) {
        PointLoopClassify inner_class = inner_loops[i].classify_point(point);
				switch (inner_class) {
          case (PointLoopClassify::PointOnEdge):
            return FacePointClass::PointOnEdge;
          case (PointLoopClassify::PointOnVertex):
            return FacePointClass::PointOnVertex;
          case (PointLoopClassify::PointOutside):
            break;
          case (PointLoopClassify::PointInside):
          	return FacePointClass::PointOutside;
          	break;
          case (PointLoopClassify::PointNotOnPlane):
          case (PointLoopClassify::PointUnknown):
  					assert(false); // this should not happen
        }
      }
      return FacePointClass::PointOutside;
    } break;
  }
  return FacePointClass::PointUnknown;
}


std::vector<Edge> Face::edges() {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::edges(): Face is null");
  }
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


/** NOT POSSIBLE WITH C API - @see class MaterialInput **/
// Vector3D Face::get_texture_projection(const bool frontside) const {}


std::vector<Loop> Face::inner_loops() const {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::inner_loops(): Face is null");
  }
	size_t num_loops = 0;
	SUFaceGetNumInnerLoops(m_face, &num_loops);
  SULoopRef* inner_loops = new SULoopRef[num_loops];
  SUFaceGetInnerLoops(m_face, num_loops, &inner_loops[0], &num_loops);
  std::vector<Loop> loops;
  loops.reserve(num_loops);
	for (size_t i=0; i < num_loops; i++) {
  	loops.push_back(Loop(inner_loops[i]));
  }
  delete inner_loops;
  return loops;
}


std::vector<Loop> Face::loops() const {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::loops(): Face is null");
  }
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
	if (!(*this)) {
  	throw std::logic_error("CW::Face::normal(): Face is null");
  }
	Plane3D c_plane = plane();
  return c_plane.normal();
}


Loop Face::outer_loop() const {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::outer_loop(): Face is null");
  }
  SULoopRef lp = SU_INVALID;
  SUResult res = SUFaceGetOuterLoop(m_face, &lp);
  assert(res == SU_ERROR_NONE);
  return Loop(lp);
}


Plane3D Face::plane() const {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::plane(): Face is null");
  }
	SUPlane3D plane = SU_INVALID;
	SUFaceGetPlane(m_face, &plane);
  return Plane3D(plane);
}


/** NOT POSSIBLE WITH C API - @see class MaterialInput **/
//bool Face::position_material(const Material& material, const std::vector<Point3D>& pt_array, bool o_front) {}


Face& Face::reverse() {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::reverse(): Face is null");
  }
	SUResult res = SUFaceReverse(m_face);
  assert(res == SU_ERROR_NONE);
  return *this;
}


/*
* Sets the texture projection direction.
* @param SUVector3D object representing the direction of the projection. Or bool true to remove texture projection.
* @param bool true for front side, false for back side.
* @return true on success
*/
/** NOT POSSIBLE WITH C API - @see class MaterialInput **/
/**
bool Face::set_texture_projection(const Vector3D& vector, bool frontside) {}

bool Face::set_texture_projection(bool remove, bool frontside) {}
*/


std::vector<Vertex> Face::vertices() const {
	if (!(*this)) {
  	throw std::logic_error("CW::Face::vertices(): Face is null");
  }
	size_t num_vertices = 0;
	SUFaceGetNumVertices(m_face, &num_vertices);
  SUVertexRef* vertices = new SUVertexRef[num_vertices];
  SUResult res = SUFaceGetVertices(m_face, num_vertices, &vertices[0], &num_vertices);
  assert(res == SU_ERROR_NONE);
  std::vector<Vertex> verts;
  verts.reserve(num_vertices);
  for (size_t i=0; i < num_vertices; i++) {
  	verts.push_back(Vertex(vertices[i]));
  }
  delete vertices;
  return verts;
}
  
} /* namespace CW */
