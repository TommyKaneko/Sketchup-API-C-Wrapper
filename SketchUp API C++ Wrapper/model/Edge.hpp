//
//  Edge.hpp
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

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>

#include <SketchUpAPI/model/edge.h>

#include "Geometry.hpp"
#include "DrawingElement.hpp"

namespace CW {

// Forward Declarations
class Vertex;
class Face;
class Color;

/**
*
*/
class Edge :public DrawingElement {
	private:
  SUEdgeRef m_edge;
  
  static SUEdgeRef create_edge(const Point3D& start, const Point3D& end);

  static SUEdgeRef copy_reference(const Edge& other);
  
  public:
  /**
  * Constructor for null Edge object.
  */
  Edge();
  
  /*
  * Construct an Edge from a vector of two points.
  * @param points - where points[0] is the start vertex and points[1] is the end vertex.
  */
  Edge(const std::vector<Point3D>& points);

  /*
  * Construct an Edge from a vector of two points.
  * @param start - the start vertex
  * @param end - the end vertex.
  */
  Edge(const Point3D& start, const Point3D& end);
	
  /**
  * Creates new edge using vertices position.  Note that this will not join the new edge to existing vertices.  Use GeometryInput for welding edges.
  */
  Edge(const Vertex& start, const Vertex& end);
  
  /*
  * Edge constructor that essentially wraps around an already created SUFaceRef object.
  * @param SUEdgeRef pointer to the edge.
  * @param bool true if the edge should be released when this class object is destroyed.  False, if the release of the face object is handled elsewhere (use with caution).
  */
  Edge(SUEdgeRef edge, bool attached = true);
	
  /** Copy Constructor */
  Edge(const Edge& other);
  
  ~Edge();
  
  Edge& operator=(const Edge& other);
  
  
  /**
  * Returns SUEdgeRef object for the Edge.
  */
  SUEdgeRef ref() const;

  /*
  * The class object can be converted to a SUEdgeRef without loss of data.
  */
  operator SUEdgeRef() const;
  operator SUEdgeRef*();
  
  /*
  * Returns whether the class is a valid object.
  */
  // operator bool() const;
  /**
  * NOT operator.  Checks if the SUEdgeRef is valid.
  * @return true if the edge is invalid
  */
	bool operator!() const;
  
	/*
  * Returns the Color object assigned to the Edge
  */
  Color color() const;
  
  /*
  * Sets the color of the Edge.
  */
  bool color(const Color& input_color);
  
  /*
  * Return the vertex at the end of the Edge.
  */
  Vertex end() const;

  /*
  * Return the faces connected to this edge.
  */
  std::vector<Face> faces() const;
  
  /*
  * Gets the SU_RESULT of the create edge operation.
  * @return * SU_ERROR_NONE on success
            * SU_ERROR_NULL_POINTER_INPUT if start or end is NULL
            * SU_ERROR_NULL_POINTER_OUTPUT if edge is NULL
            * SU_ERROR_GENERIC if start and end specify the same position.
  */
  // SU_RESULT get_result() const;

  /*
  * Determine if the Edge is smooth.
  * @return true if the Edge is smooth.
  */
  bool smooth() const;
  
  /*
  * Set the smooth status for an Edge.
  * @param bool true to make the edge smooth, false to make the edge hard.
  * @return true for successful, false for unsuccessful.
  */
  bool smooth(bool smooth);
  
  /*
  * Determine if the Edge is soft.
  * @return true if the Edge is soft.
  */
  bool soft() const;
  
  /*
  * Set the soft status for an edge.
  * @param bool true if you want soften the edge, false if you do not want to soften the edge.
  * @return true for successful, false for unsuccessful.
  */
  bool soft(bool soft);

  /*
  * Return the vertex at the start of the Edge.
  */
  Vertex start() const;
  
  /*
  * Return Vector between the start and end points
  */
  Vector3D vector() const;
};

} /* namespace CW */
#endif /* Edge_hpp */
