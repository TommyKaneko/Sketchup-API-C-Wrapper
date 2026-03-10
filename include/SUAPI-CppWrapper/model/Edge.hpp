//
//  Edge.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
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

#ifndef Edge_hpp
#define Edge_hpp

#include <SketchUpAPI/model/edge.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

namespace CW {

// Forward Declarations
class Vertex;
class Face;
class Color;

/**
 * @brief C++ wrapper for the SketchUp C API's SUEdgeRef object.
 *
 * An Edge represents a line segment in 3D space that connects two vertices.
 * Edges can be associated with faces and curves.
 */
class Edge :public DrawingElement {
  private:
  static SUEdgeRef create_edge(const Point3D& start, const Point3D& end);

  static SUEdgeRef copy_reference(const Edge& other);

  public:
  /**
   * @brief Constructor for null Edge object.
   */
  Edge();

  /**
   * @brief Construct an Edge from a vector of two points.
   * @param points where points[0] is the start vertex and points[1] is the end vertex.
   */
  Edge(const std::vector<Point3D>& points);

  /**
   * @brief Construct an Edge from two points.
   * @param start the start vertex position.
   * @param end the end vertex position.
   */
  Edge(const Point3D& start, const Point3D& end);

  /**
   * @brief Creates a new edge using vertex positions. Note that this will not
   *        join the new edge to existing vertices. Use GeometryInput for welding edges.
   * @param start the start vertex.
   * @param end the end vertex.
   */
  Edge(const Vertex& start, const Vertex& end);

  /**
   * @brief Constructs an Edge object from a pre-existing SUEdgeRef.
   * @param edge the SUEdgeRef to wrap.
   * @param attached true if the edge is already owned by a model (default: true).
   */
  Edge(SUEdgeRef edge, bool attached = true);

  /** @brief Copy Constructor */
  Edge(const Edge& other);

  /** @brief Destructor */
  ~Edge();

  /** @brief Copy assignment operator */
  Edge& operator=(const Edge& other);

  /**
   * @brief Creates an explicit copy of the Edge object, detached from the original.
   * @return a new Edge with the same properties, not attached to any model.
   * @throws std::logic_error if the edge is null.
   */
  Edge copy() const;

  /**
   * @brief Returns the SUEdgeRef object for the Edge.
   */
  SUEdgeRef ref() const;

  /**
   * @brief The class object can be converted to a SUEdgeRef without loss of data.
   */
  operator SUEdgeRef() const;

  /**
   * @brief NOT operator. Checks if the SUEdgeRef is invalid.
   * @return true if the edge is invalid.
   */
  bool operator!() const;

  /**
   * @brief Retrieves the color of the edge.
   * @return Color object assigned to the edge.
   * @throws std::logic_error if the edge is null.
   */
  Color color() const;

  /**
   * @brief Sets the color of the edge.
   * @param input_color the Color to set.
   * @throws std::logic_error if the edge is null.
   */
  void color(const Color& input_color);

  /**
   * @brief Retrieves the end vertex of the edge.
   * @return Vertex at the end of the edge.
   * @throws std::logic_error if the edge is null.
   */
  Vertex end() const;

  /**
   * @brief Retrieves the face objects associated with the edge.
   * @return vector of Face objects connected to this edge.
   * @throws std::logic_error if the edge is null.
   */
  std::vector<Face> faces() const;

  /**
   * @brief Retrieves the smooth flag of the edge.
   * @return true if the edge is smooth.
   * @throws std::logic_error if the edge is null.
   */
  bool smooth() const;

  /**
   * @brief Sets the smooth flag of the edge.
   * @param smooth true to make the edge smooth, false to make it hard.
   * @throws std::logic_error if the edge is null.
   */
  void smooth(bool smooth);

  /**
   * @brief Retrieves the soft flag of the edge.
   * @return true if the edge is soft.
   * @throws std::logic_error if the edge is null.
   */
  bool soft() const;

  /**
   * @brief Sets the soft flag of the edge.
   * @param soft true to soften the edge, false to harden it.
   * @throws std::logic_error if the edge is null.
   */
  void soft(bool soft);

  /**
   * @brief Retrieves the starting vertex of the edge.
   * @return Vertex at the start of the edge.
   * @throws std::logic_error if the edge is null.
   */
  Vertex start() const;

  /**
   * @brief Returns the vector between the start and end points.
   * @return Vector3D from start to end.
   * @throws std::logic_error if the edge is null.
   */
  Vector3D vector() const;
};

} /* namespace CW */
#endif /* Edge_hpp */
