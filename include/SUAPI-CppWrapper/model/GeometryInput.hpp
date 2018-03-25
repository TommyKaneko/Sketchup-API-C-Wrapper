//
//  GeometryInput.hpp
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

#ifndef GeometryInput_hpp
#define GeometryInput_hpp

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <array>

#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/model/entities.h>

#include "SUAPI-CppWrapper/Geometry.hpp"

namespace CW {

// Forward declarations:
class Entities;
class Face;
class Edge;
class Material;
class MaterialInput;
class Layer;
class LoopInput;

/**
* Geometry Input class is an abstraction of Sketchup C API's SUGeometryInputRef object.  It allows a much easier way for programmers to build Sketchup geometry within this class, before exporting it into a built SUGeometryInputRef object.
* See below for an example of GeometryInput operates:
GeometryInput geom_input;
Entities entities; // This is the entities object that we wish to add the geometry to.
std::vector<SUPoint3D> outer_loop{SUPoint3D(0.0,0.0,0.0), SUPoint3D(1.0,0.0,0.0), SUPoint3D(0.0,1.0,0.0)};
std::vector<std::vector<SUPoint3D>> inner_loops{{SUPoint3D(0.1,0.1,0.0), SUPoint3D(0.1,0.2,0.0), SUPoint3D(0.2,0.3,0.0)}};
GIFace* face = geom_input.add_face(outer_loop, inner_loops); // GeometryInput handles the interface with Sketchup C API when creating geometry.
SUResult result = entities.fill(geom_input); // the geometry must be output into a SUEntitiesRef object.

*/
class GeometryInput {
  friend class Entities;
  
  private:
  SUGeometryInputRef m_geometry_input;

  /**
  * As Geometry Input does not always copy all information about an Entity to an Entities object, things like Attributes must be copied into the new Geometry after SUEntitiesFill() is called.  So it is necessary to store information about each entity that was added below, to allow this.
  */
  /**
  * Faces that have been added to the GeometryInput object.  The pairings are with the added face index.  Only faces that require additional properties after the Entities::fill() operation are added here.
  */
  std::vector<std::pair<size_t, Face>> m_faces;
  std::vector<std::pair<size_t, Edge>> m_edges;
  //std::vector<std::vector<Edge*>> m_curves; // curves are simply collections of Edges.
  
  size_t m_vertex_index = 0;
  size_t m_num_faces = 0;
  size_t m_num_edges = 0;
   /**
  * Creates and returns new SUGeometryInputRef object, used for initializing m_geometry_input. The SUGeometryInputRef object must be released with SUGeometryInputRelease() before this class is destroyed.
  */
  static SUGeometryInputRef create_geometry_input();


  /*
  * Adds a constructed LoopInput object into the GeometryInput object.
  * @see add_face() for the details on how this is used.
  */
  //SUResult add_loop(LoopInput &loop);

  //std::vector<SUPoint3D> m_vertices;
  /**
  * Faces are stored as a series of nested vectors of vertices.
  * 1st level - array of faces
  * 2nd level - array of loops within the face.  The first loop is the outer loop.  All subsequent arrays represent inner loops.
  * 3rd level - array of indexes that refer to the vertices in the m_vertices array.
  */
  //std::vector<std::vector<std::vector<size_t>>> m_faces;

  /**
  * Edges are stored a series of nested vectors of vertices.
  * 1st level - array of edges/curves
  * 2nd level - array of indexes that refer to the vertices in the m_vertices array, which represent edges/curves (curves have more than two vertices)
  */
  //std::vector<std::vector<size_t>> m_edges;
  
  public:
  /**
  * Creates a valid, but empty GeometryInput object.
  */
  GeometryInput();
  
  /** Copy Constructor **/
  GeometryInput(const GeometryInput& other);
  
  /** Destructor  */
  ~GeometryInput();
  
  /**
  * Copy assignment operator.
  */
  GeometryInput& operator=(const GeometryInput& other);
   
  /**
  * Returns Raw SUGeometryInputRef that is stored.
  */
  SUGeometryInputRef ref() const;

  /**
  * Returns true if the objet is valid, false otherwise.
  */
  bool operator!() const;
  
  /**
  * Returns the number of faces have been input into this GeometryInput object.
  */
  size_t num_faces() const;
  
  /**
  * Returns Face objects stored in this object.  The vector returns a pairing of the added face's index in the GeometryInput object with the Face object.
  */
  std::vector<std::pair<size_t, Face>> faces() const;
  
  /**
  * Adds a vertex to the Input object, and returns the index of the added Vertex.
  * @param vertex Point3D object.
  * @return index of the vertex that was added.
  */
  //size_t add_vertex(Point3D vertex);
  //std::vector<size_t> add_vertices(std::vector<Point3D> vertices);
  
  /**
  * Adds a face to the Geometry Input object.
  * @return index to the added face.
  */
  size_t add_face(const Face &face);
  size_t add_faces(const std::vector<Face>& faces);
  
  /*
  Face add_face(const std::vector<Loop> &loops);
  Face add_face(const std::vector<SUPoint3D> &outer_loop);
  Face add_face(const std::vector<Edge*> &outer_edges);
  */
  
  /**
  * Adds an Edge to the Geometry Input object.
  * @return index to the added edge.
  */
  size_t add_edge(const Edge &edge);
  size_t add_edges(const std::vector<Edge>& edges);
  //size_t add_line(Edge &edge);
  //Edge add_line(const SUPoint3D start, const SUPoint3D end);
  //Edge add_edge(const SUPoint3D start, const SUPoint3D end);

  //std::vector<Edge*> add_edges(std::vector<Edge> &edges);
  //std::vector<Edge*> add_edges(const std::vector<SUPoint3D> &points);

  //std::vector<Edge*> add_curve(const std::vector<SUPoint3D> &points);
  
  /**
  * Returns true if no geometry has been added to this object.
  */
  bool empty() const;
  
  /**
  * Adds a vertex to the GeometryInput object.
  * @param point - the Point3D location of the vertex to add.
  * @return the index number of the added vertex
  */
  size_t add_vertex(const Point3D& point);
  
  /**
  * Sets all vertices of a geometry input object. Any existing vertices will be overridden.
  * @param points - vector of points to set as the vertices
  */
  void set_vertices(const std::vector<SUPoint3D>& points);
  void set_vertices(const std::vector<Point3D>& points);
  
  /**
  * Adds an edge to a geometry input object. This method is intended for specifying edges which are not associated with loop inputs. For specifying edge properties on a face use the SULoopInput interface. More...
  * @param vertex0_index - vertex index of start point of edge.
  * @param vertex1_index - vertex index of end point of edge.
  * @return index of added edge
  */
  size_t add_edge(size_t vertex0_index, size_t vertex1_index);
  
  /**
  * Sets the hidden flag of an edge in a geometry input object which is not associated with a loop input.
  * @param edge_index - index of the edge to set the hidden flag.
  * @param hidden - the flag to set.
  */
  void edge_hidden(size_t edge_index, bool hidden);
  
  /**
  * Sets the soft flag of an edge in a geometry input object which is not associated with a loop input.
  * @since SketchUp 2017, API 5.0
  * @param edge_index - The zero-based index of the edge which is not associated with a loop input.
  * @param soft - The flag to set.
  */
  void edge_soft(size_t edge_index, bool soft);

  /**
  *Sets the smooth flag of an edge in a geometry input object which is not associated with a loop input.
  * @since SketchUp 2017, API 5.0
  * @param edge_index - The zero-based index of the edge which is not associated with a loop input.
  * @param smooth - The flag to set.
  */
  void edge_smooth(size_t edge_index, bool smooth);
  
  /**
  * Sets the material of an edge in the geometry input.
  * @since SketchUp 2017, API 5.0
  * @param edge_index - Index of the edge to set the material.
  * @param material - The material to be set.
  */
  void edge_material(size_t edge_index, const Material& material);
  
  /**
  *  Sets the layer of an edge in the geometry input.
  * @since SketchUp 2017, API 5.0
  * @param edge_index - Index of the edge to set the layer.
  * @param layer - The layer to be set.
  */
  void edge_layer(size_t edge_index, const Layer& layer);
  
  /*
  // TODO:
  size_t add_curve(const std::vector<size_t>& edge_indices);
SU_RESULT   SUGeometryInputAddCurve (SUGeometryInputRef geom_input, size_t num_edges, const size_t edge_indices[], size_t *added_curve_index)
   Adds a curve to a geometry input object. This method is intended for specifying curves which are not associated with loop inputs. For specifying curves on faces use the SULoopInput interface. More...
  */
  
  /*
  // TODO:
  std::pair<size_t, size_t> add_arc_curve(size_t start_point, size_t end_point, const struct SUPoint3D *center, const struct SUVector3D *normal, size_t num_segments);
SU_RESULT   SUGeometryInputAddArcCurve (SUGeometryInputRef geom_input, size_t start_point, size_t end_point, const struct SUPoint3D *center, const struct SUVector3D *normal, size_t num_segments, size_t *added_curve_index, size_t *control_edge_index)
   Adds an arccurve to a geometry input object. In addition to adding an arccurve to the geometry input this method will append num_segments edges to the geometry's edge collection where control_edge_index is the index of the first new edge. Also, num_segments-1 vertices along the arc will be appended to the geometry's collection of verttices. In order to include an arccurve in a loop the user only needs add the arccurve's points to a loop using SULoopInputAddVertexIndex. More...
   */
  
  /**
  * Adds a face to a geometry input object with a given outer loop for the face.
  * @param outer_loop - The outer loop to be set for the face. If the function succeeds (i.e. returns SU_ERROR_NONE), this loop will be deallocated.
  * @return returns the index of the added face.
  */
  size_t add_face(LoopInput& loop_input);
  
  /**
  * Sets a flag in the geometry input that, when true, will create a face by reversing the orientations of all of its loops.
  * @param face_index - Index of the face to be reversed.
  * @param reverse - The given reverse flag.
  */
  void face_reverse(size_t face_index, bool reverse);
  
  /**
  * Sets the layer of a face in the geometry input.
  * @param face_index - Index of the face to be reversed.
  * @param layer - The layer to be set.
  */
  void face_layer(size_t face_index, const Layer& layer);
  
  /**
  * Adds an inner loop to a face in the geometry input.
  * @param face_index - Index of the face to receive the inner loop.
  * @param loop_input - The inner loop to be added. If the function succeeds (i.e. returns SU_ERROR_NONE), this loop will be deallocated.
  */
  void face_add_inner_loop(size_t face_index, LoopInput& inner_loop);
  
  /**
  * Sets the front material of a face in the geometry input.
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_front_material(size_t face_index, MaterialInput& material_input);
  
  /**
  * Sets the back material of a face in the geometry input.
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_back_material(size_t face_index, MaterialInput& material_input);

  /**
  * Sets a flag in the geometry input that, when true, will create a hidden face.
  * @since SketchUp 2017, API 5.0
  * @param face_index - Index of the face to be hidden.
  * @param hidden - The given hidden flag.
  */
  void face_hidden(size_t face_index, bool hidden);

  /**
  * Returns all the various geometry counts.
  * @since SketchUp 2018, API 6.0
  * @return array of counts:
  *   - [0] - vertices_count - The total count of vertices.
  *   - [1] - faces_count  The total count of faces.
  *   - [2] - edge_count  The total count of edges.
  *   - [3] - curve_count  The total count of curves.
  *   - [4] - arc_count  The total count of arcs.
  */
  std::array<size_t, 5> counts() const;
  
};


} /* namespace CW */

#endif /* GeometryInput_hpp */
