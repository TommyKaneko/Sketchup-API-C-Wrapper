//
//  Loop.cpp
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

#include "Loop.hpp"

#include "LoopInput.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"

#include <cassert>
#include <math.h>

namespace CW {

Loop::Loop(SULoopRef loop):
  m_loop(loop)
{}


Loop::Loop():
	m_loop(SU_INVALID)
{}


bool Loop::operator!() const {
	return SUIsInvalid(m_loop);
}


LoopInput Loop::loop_input() const {
	std::vector<Edge> edges = this->edges();
  return LoopInput(this->edges());
}


std::vector<Edge> Loop::edges() const {
  assert(!!(*this));
  size_t count = 0;
  SU_RESULT res = SULoopGetNumVertices(m_loop, &count);
  assert(res == SU_ERROR_NONE);
  SUEdgeRef edge_array[count];
  res = SULoopGetEdges(m_loop, count, &edge_array[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Edge> edges;
  edges.reserve(count);
  for (size_t i=0; i < count; ++i) {
    edges.push_back(Edge(edge_array[i]));
  }
  return edges;
}


std::vector<Vertex> Loop::vertices() const {
  assert(!!(*this));
  size_t count = 0;
  SU_RESULT res = SULoopGetNumVertices(m_loop, &count);
  assert(res == SU_ERROR_NONE);
  SUVertexRef verts_array[count];
  res = SULoopGetVertices(m_loop, count, &verts_array[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Vertex> vertices;
  vertices.reserve(count);
  for (size_t i=0; i < count; ++i) {
    vertices.push_back(Vertex(verts_array[i]));
  }
  return vertices;
}

std::vector<Point3D> Loop::points() const {
	std::vector<Vertex> verts = vertices();
  std::vector<Point3D> points;
  points.reserve(verts.size());
  for (size_t i=0; i < verts.size(); ++i) {
  	points.push_back(verts[i].position());
  }
  return points;
}

PointLoopClassify Loop::classify_point(const Point3D& point) const {
	// TODO:
  assert(false);
}


size_t Loop::size() const {
  size_t count = 0;
  SU_RESULT res = SULoopGetNumVertices(m_loop, &count);
  assert(res == SU_ERROR_NONE);
	return count;
}


SULoopRef Loop::ref() const {
	return m_loop;
}
  
  
PointLoopClassify Loop::classify_point(const std::vector<Point3D>& loop_points, const Point3D& test_point) {
	assert(loop_points.size() > 2);
  // http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
  // First check that the test point is on the plane
  Plane3D loop_plane = Plane3D::plane_from_loop(loop_points);
  if (!loop_plane) {
  	return PointLoopClassify::PointNotOnPlane;
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
