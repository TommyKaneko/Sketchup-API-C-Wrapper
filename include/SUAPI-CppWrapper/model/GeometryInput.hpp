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
#include <unordered_map>

#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/model/entities.h>

#include "SUAPI-CppWrapper/Geometry.hpp"

namespace CW {

// Forward declarations:
class Entities;
class Material;
class MaterialInput;
class MaterialPositionInput;
class Layer;
class LoopInput;

/**
* Geometry Input class is a minimal abstraction of Sketchup C API's SUGeometryInputRef object.
* It provides thin wrappers around the C API functions for building geometry.
* For higher-level operations involving materials and layers, see GeometryInputPlus.
*/
class GeometryInput {
  friend class Entities;

private:
  SUGeometryInputRef m_geometry_input;

  size_t m_vertex_count = 0;

  // Tracks the number of GeometryInput objects have been allocated, to allow the destructor to release an object only at the right time.
  static std::unordered_map<SUGeometryInputRef, size_t> num_objects_;

  /**
  * Creates and returns new SUGeometryInputRef object, used for initializing m_geometry_input. The SUGeometryInputRef object must be released with SUGeometryInputRelease() before this class is destroyed.
  */
  static SUGeometryInputRef create_geometry_input();

public:
  /**
  * Creates a valid, but empty GeometryInput object.
  */
  GeometryInput();

  /** Copy Constructor **/
  GeometryInput(const GeometryInput& other);

  /** Destructor (virtual to allow derived classes) */
  virtual ~GeometryInput();

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

  /**
  * Adds a curve to a geometry input object. This method is intended for specifying curves which are not associated with loop inputs. For specifying curves on faces use the SULoopInput interface.
  * @since SketchUp 2017, API 5.0
  * @param num_edges - The number of edges to be used in the curve.
  * @param edge_indices - The edge indices to be used in defining the curve.
  * @param  added_curve_index  (optional) If not NULL, returns the index of the added curve.
  */
  size_t add_curve(const std::vector<size_t>& edge_indices);

  /**
  * Adds an arccurve to a geometry input object. In addition to adding an arccurve to the geometry input this method will append num_segments edges to the geometry's edge collection where control_edge_index is the index of the first new edge. Also, num_segments-1 vertices along the arc will be appended to the geometry's collection of verttices. In order to include an arccurve in a loop the user only needs add the arccurve's points to a loop using SULoopInputAddVertexIndex.
  * @since SketchUp 2017 M2, API 5.2
  * @param start_point - The index of the vertex at the start of the arc.
  * @param end_point - The index of the vertex at the end of the arc.
  * @param center - The center point of the arc's circle.
  * @param normal - The normal vector of the arc plane.
  * @param num_segments - The number of edges for the arc.
  * @return pair of size_t values:
      - first - added_curve_index - the index of the added curve.
  *   - second - control_edge_index - the index of the the arc's control edge which can be used to set the arc's edge properties.
  */
  std::pair<size_t, size_t> add_arc_curve(size_t start_point, size_t end_point, const Point3D& center, const Vector3D& normal, size_t num_segments);

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
  * @deprecated - @since Sketchup API 2020.1 - This struct is made obsolete by the newer SUMaterialPositionInput and left in place only for compatibility.
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_front_material(size_t face_index, MaterialInput& material_input);

  /**
  * Sets the back material of a face in the geometry input.
  * @deprecated - @since Sketchup API 2020.1 - This struct is made obsolete by the newer SUMaterialPositionInput and left in place only for compatibility.
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_back_material(size_t face_index, MaterialInput& material_input);

  #if SketchUpAPI_VERSION_MAJOR >= 2021
  /**
  * Sets the front material of a face in the geometry input.
  * @since Sketchup API 2020.1, API 9.1
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_front_material_position(size_t face_index, const MaterialPositionInput& material_input);

  /**
  * Sets the back material of a face in the geometry input.
  * @since Sketchup API 2020.1, API 9.1
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_back_material_position(size_t face_index, const MaterialPositionInput& material_input);
  #endif

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

  /**
  * Hash function for use with unordered_map
  */
  friend std::hash<SUGeometryInputRef>;
};



} /* namespace CW */

namespace std {
  template <> struct hash<SUGeometryInputRef>
  {
    size_t operator()(const SUGeometryInputRef& k) const
    {
      static const size_t shift = (size_t)log2(1 + sizeof(SUGeometryInputRef));
      return (size_t)(k.ptr) >> shift;
    }
  };
}


#endif /* GeometryInput_hpp */
