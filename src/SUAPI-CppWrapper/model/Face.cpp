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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include <cassert>
#include <stdexcept>

#include <SketchUpAPI/model/edge.h>

#include "SUAPI-CppWrapper/model/Face.hpp"

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Texture.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/LoopInput.hpp"
#include "SUAPI-CppWrapper/model/GeometryInput.hpp"
#include "SUAPI-CppWrapper/model/MaterialInput.hpp"

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
  std::vector<SUPoint3D> su_points(outer_points.size());
  std::transform(outer_points.begin(), outer_points.end(), su_points.begin(),
    [](const Point3D& value){
      return (SUPoint3D)value;
    });
  SUResult res = SUFaceCreate(&face, su_points.data(), &loop_input_ref);
  if (res != SU_ERROR_NONE) {
    // The points cannot be made into a face: either the points do not lie in a plane, or is somehow problematic.
    return SU_INVALID;
  }
  loop_input.m_attached = true;
  return face;
}


SUFaceRef Face::copy_reference(const Face& other) {
  if (other.m_attached || !other) {
    return other.ref();
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
  DrawingElement(SU_INVALID, false)
{}


Face::Face(std::vector<Point3D>& outer_loop):
  Face(create_face(outer_loop), false)
{}


Face::Face(std::vector<Point3D>& outer_loop, LoopInput& loop_input):
  Face(create_face(outer_loop, loop_input), false)
{}


Face::Face(SUFaceRef face, bool attached):
  DrawingElement(SUFaceToDrawingElement(face), attached)
{}


/** Copy constructor */
Face::Face(const Face& other):
  DrawingElement(other, SUFaceToDrawingElement(copy_reference(other)))
{
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    /**  The code below causes errors as the face has not been attached to an object yet.
    // Add the inner loops
    std::vector<Loop> inner_loops = other.inner_loops();
    std::vector<std::vector<Point3D>> inner_loops_points;
    inner_loops_points.reserve(inner_loops.size());
    for (size_t i=0; i < inner_loops.size(); ++i) {
      std::vector<Point3D> inner_points = inner_loops[i].points();
      LoopInput inner_loop_input = inner_loops[i].loop_input();
      add_inner_loop(inner_points, inner_loop_input);
    }
    */
    this->back_material(other.back_material());
  }
}


Face::~Face() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUFaceRef face = this->ref();
    SUResult res = SUFaceRelease(&face);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}

/******************
* Public Methods **
*******************/
/** Copy assignment operator */
Face& Face::operator=(const Face& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUFaceRef face = this->ref();
    SUResult res = SUFaceRelease(&face);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUFaceToEntity(copy_reference(other));
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    this->back_material(other.back_material());
    // TODO: copy other properties
  }
  DrawingElement::operator=(other);
  return *this;
}


SUFaceRef Face::ref() const {  return SUFaceFromEntity(m_entity); }


Face::operator SUFaceRef() const {  return this->ref();}


bool Face::operator!() const {
  if (SUIsInvalid(m_entity)) {
    return true;
  }
  return false;
}


double Face::area() const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::area(): Face is null");
  }
  double area;
  SUResult res = SUFaceGetArea(this->ref(), &area);
  assert(res == SU_ERROR_NONE); _unused(res);
  return area;
}


void Face::add_inner_loop(std::vector<Point3D>& points, LoopInput &loop_input) {
  if (!(*this)) {
    throw std::logic_error("CW::Face::add_inner_loop(): Face is null");
  }
  if (points.size() != loop_input.m_edge_num) {
    throw std::invalid_argument("CW::Face::add_inner_loop(): Unequal number of vertices between given Point3D vector and LoopInput object");
  }
  SUResult res = SUFaceAddInnerLoop(this->ref(), points[0], loop_input);
  if (res == SU_ERROR_INVALID_INPUT) {
    throw std::invalid_argument("CW::Face::add_inner_loop(): Arguments are invalid");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
}


Material Face::back_material() const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::back_material(): Face is null");
  }
  SUMaterialRef material = SU_INVALID;
  SUResult res = SUFaceGetBackMaterial(this->ref(), &material);
  if (res == SU_ERROR_NO_DATA) {
    return Material();
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return Material(material);
}


Material Face::back_material(const Material& material) {
  if (!(*this)) {
    throw std::logic_error("CW::Face::back_material(): Face is null");
  }
  SUResult res = SUFaceSetBackMaterial(this->ref(), material.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
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


std::vector<Loop> Face::inner_loops() const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::inner_loops(): Face is null");
  }
  size_t num_loops = 0;
  SUFaceGetNumInnerLoops(this->ref(), &num_loops);
  std::vector<SULoopRef> inner_loops(num_loops, SU_INVALID);
  SUFaceGetInnerLoops(this->ref(), num_loops, inner_loops.data(), &num_loops);
  std::vector<Loop> loops(num_loops);
  std::transform(inner_loops.begin(), inner_loops.end(), loops.begin(),
    [](const SULoopRef& value){
      return Loop(value);
    });
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
  SUResult res = SUFaceGetOuterLoop(this->ref(), &lp);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Loop(lp);
}


Plane3D Face::plane() const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::plane(): Face is null");
  }
  SUPlane3D plane;
  SUFaceGetPlane(this->ref(), &plane);
  return Plane3D(plane);
}


/** NOT POSSIBLE WITH C API - @see class MaterialInput **/
//bool Face::position_material(const Material& material, const std::vector<Point3D>& pt_array, bool o_front) {}
#if SketchUpAPI_VERSION_MAJOR >= 2021

Vector3D Face::get_texture_projection(const bool frontside) const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::get_texture_projection(): Face is null");
  }
  SUVector3D projection;
  SUResult res = SUFaceGetTextureProjection(this->ref(), frontside, &projection);
  if (res == SU_ERROR_NO_DATA) {
    throw std::logic_error("CW::Face::get_texture_projection(): the Face does not have a projected texture applied");
  }
  assert(res == SU_ERROR_NONE);
  return Vector3D(projection);
}


bool Face::is_texture_positioned(bool front) const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::is_texture_positioned(): Face is null");
  }
  bool is_positioned;
  SUResult res = SUFaceIsTexturePositioned(this->ref(), front, &is_positioned);
  assert(res == SU_ERROR_NONE);
  return is_positioned;
}


bool Face::is_texture_positioned_front() const {
  return is_texture_positioned(true);
}


bool Face::is_texture_positioned_back() const {
  return is_texture_positioned(false);
}


bool Face::is_texture_projected(bool front) const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::is_texture_projected(): Face is null");
  }
  bool is_projected;
  SUResult res = SUFaceIsTextureProjected(this->ref(), front, &is_projected);
  assert(res == SU_ERROR_NONE);
  return is_projected;
}


bool Face::is_texture_projected_front() const {
  return is_texture_projected(true);
}


bool Face::is_texture_projected_back() const {
  return is_texture_projected(false);
}


MaterialPositionInput Face::material_position(bool front) const {
  if (!(*this)) {
    throw std::logic_error("CW::Face::material_position(): Face is null");
  }
  if (!this->material()) {
    throw std::logic_error("CW::Face::material_position(): Face does not have a material");
  }
  SUPoint3D reference_point = this->vertices()[0].position(); // TODO: this is an arbitrary reference point - find a better way
  SUMaterialPositionInput material_input = {
    .num_uv_coords = 4,
    //.uv_coords = uv_coords,
    //.points = points,
    .material = this->material(),
    .projection = SU_INVALID
  };
  if (((front && !!this->material().texture()) || (!front && !!this->back_material().texture())) && this->is_texture_projected(front)) {
    material_input.projection = this->get_texture_projection(front);
  }
  SUResult res = SUFaceGetUVTileAt(this->ref(), &reference_point, front, material_input.points, material_input.uv_coords);
  // TODO: test returns - a MaterialPositionInput without any textures should be valid.
  return MaterialPositionInput(material_input);
}


MaterialPositionInput Face::material_position_front() const {
  return this->material_position(true);
}


MaterialPositionInput Face::material_position_back() const {
  return this->material_position(false);
}


bool Face::material_position(const MaterialPositionInput& material_input, bool front) {
  if (!(*this)) {
    throw std::logic_error("CW::Face::material_position(): Face is null");
  }
  SUPoint3D reference_point = this->vertices()[0].position();
  SUMaterialPositionInput material_input_out = material_input.ref();
  SUResult res = SUFacePositionMaterial( this->ref(), front, &material_input_out);
  if (res == SU_ERROR_INVALID_ARGUMENT) {
    if (!material_input.material() || !material_input.material().texture()) {
      throw std::logic_error("CW::Face::material_position(): No texture was given to project");
    }
    else {
      throw std::logic_error("CW::Face::material_position(): given material_input parameters are invalid");
    }
  }
  assert(res == SU_ERROR_NONE);
  return true;
}


bool Face::material_position_front(const MaterialPositionInput& material_input){
  return this->material_position(material_input,  true);
}


bool Face::material_position_back(const MaterialPositionInput& material_input) {
  return this->material_position(material_input, false);
}

#endif

Face& Face::reverse() {
  if (!(*this)) {
    throw std::logic_error("CW::Face::reverse(): Face is null");
  }
  SUResult res = SUFaceReverse(this->ref());
  assert(res == SU_ERROR_NONE); _unused(res);
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
  SUFaceGetNumVertices(this->ref(), &num_vertices);
  std::vector<SUVertexRef> vertex_refs(num_vertices, SU_INVALID);
  SUFaceGetVertices(this->ref(), num_vertices, vertex_refs.data(), &num_vertices);
  std::vector<Vertex> vertices(num_vertices);
  std::transform(vertex_refs.begin(), vertex_refs.end(), vertices.begin(),
    [](const SUVertexRef& value){
      return Vertex(value);
    });
  return vertices;
}

} /* namespace CW */
