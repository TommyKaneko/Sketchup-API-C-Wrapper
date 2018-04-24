//
//  Loop.cpp
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

#include "SUAPI-CppWrapper/model/Loop.hpp"

#include "SUAPI-CppWrapper/model/LoopInput.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"

#include <cassert>
#include <stdexcept>

#include <math.h>

namespace CW {

Loop::Loop():
  Entity()
{}


Loop::Loop(SULoopRef loop, bool attached):
  Entity(SULoopToEntity(loop), attached)
{}


Loop::Loop(const Loop& other):
  Entity(other, other.m_entity)
{}


SULoopRef Loop::ref() const {
  return SULoopFromEntity(m_entity);
}


LoopInput Loop::loop_input() const {
  if(!(*this)) {
    throw std::logic_error("CW::Loop::loop_input(): Loop is null");
  }
  std::vector<Edge> edges = this->edges();
  std::vector<InputEdgeProperties> edge_properties;
  edge_properties.reserve(edges.size());
  for (size_t i=0; i < edges.size(); i++) {
    InputEdgeProperties edge_prop;
    edge_prop.hidden = edges[i].hidden();
    edge_prop.soft = edges[i].soft();
    edge_prop.smooth = edges[i].smooth();
    edge_prop.material = edges[i].material();
    edge_prop.layer = edges[i].layer();
    edge_properties.push_back(edge_prop);
  }
  return LoopInput(edge_properties);
}


std::vector<Edge> Loop::edges() const {
  if(!(*this)) {
    throw std::logic_error("CW::Loop::edges(): Loop is null");
  }
  size_t count = 0;
  SUResult res = SULoopGetNumVertices(this->ref(), &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUEdgeRef> edge_refs(count, SU_INVALID);
  res = SULoopGetEdges(this->ref(), count, edge_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Edge> edges(count);
  std::transform(edge_refs.begin(), edge_refs.end(), edges.begin(),
    [](const SUEdgeRef& value){
      return Edge(value);
    });
  return edges;
}


std::vector<Vertex> Loop::vertices() const {
  if(!(*this)) {
    throw std::logic_error("CW::Loop::vertices(): Loop is null");
  }
  size_t count = 0;
  SUResult res = SULoopGetNumVertices(this->ref(), &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUVertexRef> verts_array(count, SU_INVALID);
  res = SULoopGetVertices(this->ref(), count, verts_array.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Vertex> vertices(count);
  std::transform(verts_array.begin(), verts_array.end(), vertices.begin(),
    [](const SUVertexRef& value){
      return Vertex(value);
    });
  return vertices;
}


std::vector<Point3D> Loop::points() const {
  if(!(*this)) {
    throw std::logic_error("CW::Loop::points(): Loop is null");
  }
  std::vector<Vertex> verts = vertices();
  std::vector<Point3D> points;
  points.reserve(verts.size());
  for (size_t i=0; i < verts.size(); ++i) {
    points.push_back(verts[i].position());
  }
  return points;
}


PointLoopClassify Loop::classify_point(const Point3D& point) const {
  if(!(*this)) {
    throw std::logic_error("CW::Loop::classify_point(): Loop is null");
  }
  if(!point) {
    throw std::invalid_argument("CW::Loop::classify_point(): Point3D given is null");
  }
  return classify_point(this->points(), point);
}


size_t Loop::size() const {
  if(!(*this)) {
    throw std::logic_error("CW::Loop::size(): Loop is null");
  }
  size_t count = 0;
  SUResult res = SULoopGetNumVertices(this->ref(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}
  
  
PointLoopClassify Loop::classify_point(const std::vector<Point3D>& loop_points, const Point3D& test_point) {
  if(loop_points.size() < 3) {
    throw std::invalid_argument("CW::Loop::classify_point(): Fewer than 3 points given - not a valid loop.");
  }
  if(!test_point) {
    throw std::invalid_argument("CW::Loop::classify_point(): Point3D given is null");
  }
  // http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
  // First check that the test point is on the plane
  Plane3D loop_plane = Plane3D::plane_from_loop(loop_points);
  if (!loop_plane) {
    throw std::invalid_argument("CW::Loop::classify_point(): Points given does not form a  valid loop.");
  }
  else if (!loop_plane.on_plane(test_point)) {
    return PointLoopClassify::PointNotOnPlane;
  }
  // Now check if it is on the vertices
  for (size_t i=0; i < loop_points.size(); i++) {
    if (loop_points[i] == test_point) {
      return PointLoopClassify::PointOnVertex;
    }
  }
  // Now check if it is on the edges
  for (size_t i=0; i < loop_points.size(); i++) {
    Point3D next_point;
    if (i == loop_points.size()-1) {
      next_point = loop_points[0];
    }
    else {
      next_point = loop_points[i+1];
    }
    if (Line3D::on_line_segment(loop_points[i], next_point, test_point)) {
      return PointLoopClassify::PointOnEdge;
    }
  }
  // Now check if it is inside or outside, given that we know that the point is (a) on the same plane of the loop, and (b) not on the edge, and (c) not on the vertices.
  // From: http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
  // We draw a line from the point, and if it intersects with the loop an even number of times, then it is outside, if it intersects an odd number of times, it is inside.
  // The line to draw can be any vector that is coplanar to the loop's plane - the simple one is get a vector of two points within the loop.
  assert(loop_points[0] != loop_points[1]);
  Vector3D ray = Vector3D(loop_points[1] - loop_points[0]).unit();
  // Colinear line segments create problems for the algorith below, so we ignore them by creating a new list of points to calculate that exclude colinear edges.
  struct LoopPoint {
    Point3D point;
    Vector3D vector_to_next;
  };
  std::vector<LoopPoint> red_loop_points;
  red_loop_points.reserve(loop_points.size());
  for (size_t i=0; i < loop_points.size(); i++) {
    Point3D next_point;
    if (i == loop_points.size()-1) {
      next_point = loop_points[0];
    } else {
      next_point = loop_points[i+1];
    }
    Vector3D next_vector = next_point - loop_points[i];
    if (next_vector.unit() != ray.unit() && next_vector.unit() != -ray.unit()) {
      red_loop_points.push_back(LoopPoint{loop_points[i], next_vector});
    }
  }
  // Now we have a list of points with colinear points removed, find intersections with the line segments
  size_t num_intersections = 0;
  for (size_t i=0; i < red_loop_points.size(); i++) {
    Point3D intersection = Point3D::ray_line_intersection(red_loop_points[i].point, red_loop_points[i].vector_to_next, test_point, ray, false);
    if (!!intersection) {
      // There is a case where the ray intersection lies on a vertex of the loop. Whether this should be counted as an intersection of both lines connected to the loop or just once through depends on the orientation of the ray relative to the lines connected to that vertex
      if (intersection == red_loop_points[i].point) {
        // Get the directions of the lines connected to the vertex
        LoopPoint prev_point;
        if (i == 0) {
          prev_point = red_loop_points[red_loop_points.size()-1];
        }
        else {
          prev_point = red_loop_points[i-1];
        }
        Vector3D prev_cross = ray.cross(prev_point.vector_to_next);
        Vector3D next_cross = ray.cross(red_loop_points[i].vector_to_next);
        assert(fabs(next_cross.length()) > Vector3D::EPSILON);
        if (prev_cross.unit() != next_cross.unit()) {
          // The cross product direction is in opposite directions - ie the lines connected to the vertex are on the same side of the ray, therefore, count this as an additional intersection
          num_intersections++;
        }
      }
      else {
        num_intersections++;
      }
    }
  }
  if (num_intersections % 2 == 0) {
    // Even number of intersections
    return PointLoopClassify::PointOutside;
  }
  else {
    return PointLoopClassify::PointInside;
  }
}


} /* namespace CW */
