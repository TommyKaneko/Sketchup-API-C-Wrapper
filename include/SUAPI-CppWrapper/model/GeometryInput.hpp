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
#include "SUAPI-CppWrapper/model/GeometryInputHelper.hpp"

namespace CW {

// Forward declarations:
class Model;
class Entities;
class Face;
class Edge;
class Material;
class MaterialInput;
class MaterialPositionInput;
class Layer;
class LoopInput;
class Loop;

/**
* Geometry Input class is an abstraction of Sketchup C API's SUGeometryInputRef object.  It allows a much easier way for programmers to build Sketchup geometry within this class, before exporting it into a built SUGeometryInputRef object.
* See below for an example of how GeometryInput operates:
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
   * Store a lookup table of materials that can be applied to geometry, given the target_model given.
   */
  MaterialDictionary m_material_dict;

  /**
   * Store a lookup table of layers that can be applied to geometry, given the target_model given.
   */
  LayerDictionary m_layer_dict;

  // GeometryInput objects require that the target model (for inputting information) be known, to ensure that materials and layers assigned to geometry exists in the target model.
  Model* m_target_model;

  size_t m_vertex_count = 0;

  // Tracks the number of GeometrryInput objects have been allocated, to allow the destructor to release an object only at the right time.
  static std::unordered_map<SUGeometryInputRef, size_t> num_objects_;

  /**
  * Creates and returns new SUGeometryInputRef object, used for initializing m_geometry_input. The SUGeometryInputRef object must be released with SUGeometryInputRelease() before this class is destroyed.
  */
  static SUGeometryInputRef create_geometry_input();

public:
  /**
  * Creates a valid, but empty GeometryInput object.
  * @param target_model - model which will receive this GeometryInput object.  This is required to ensure that Layers(Tags) and Materials that are added to the model are checked for validity.
  */
  //GeometryInput(SUModelRef target_model);
  GeometryInput(Model* target_model);

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
   * @brief Prepares the GeometryInput object's materials, and if required, add materials to the target model.
   * @param materials
   */
  void load_materials(const std::vector<Material>& materials);

    /**
   * @brief Prepares the GeometryInput object's materials, and if required, add materials to the target model.
   * @param materials
   */
  void load_layers(const std::vector<Layer>& layers);

  /**
  * Returns the number of faces have been input into this GeometryInput object.
  */
  size_t num_faces() const;

  /**
  * Adds a face to the Geometry Input object.
  * @param face - the face object to be copied into the GeometryInput object.  Note that materials and layers will NOT be copied.  This must be done manually using face_front_material(), face_back_material(), and face_layer() methods.
  * @param copy_material_layer - (optional) when true, materials and layers will be copied to the GeometryInput object.  Set to false if you are copying faces from another model - you will have to add a material manually.
  * @return index to the added face.
  */
  size_t add_face(const Face &face, bool copy_material_layer = true);

  /**
  * Adds a face to the Geometry Input object using the safer LoopInput method, which can deal with inner loops.
  * @param loops - vector of loops.  The first loop in the vector is the outer loop, and all subsequent loops are the inner loops.
  * @return index to the added face.
  */
  size_t add_face(const std::vector<Loop>& loops);

  size_t add_faces(const std::vector<Face>& faces, bool copy_material_layer = true);

  /**
  * Adds an Edge to the Geometry Input object.
  * @param edge - edge object from which a copy will be created in the GeometryInput object
  * @param material - optionally apply a material to the edge (the material from the edge object will not be applied directly from the edge object)
  * @param layer - optionally apply a layer to the edge (the layer from the edge object will not be applied directly from the edge object)
  * @return index to the added edge.
  */
  size_t add_edge(const Edge &edge, const Material& material = Material(), const Layer& layer = Layer());
  size_t add_edges(const std::vector<Edge>& edges);

  /**
   * @brief Adds an Edge with Material and Layer assigned to it.  The number of edges and number of materials and layers must be equal, so that each edge can be associated with a material/layer
   * @return size_t - index of the last added edge
   */
  size_t add_edges(const std::vector<Edge>& edges, const std::vector<Material>& materials, const std::vector<Layer>& layers);

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
   * Helper methods that copies all entities from an Entities object into this GeometryInput object.
   * Note: it will not copy nested ComponentDefinitions or Groups.
   */
  void add_entities(const Entities& entities);

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
   * @brief Returns a material reference from the target model associated with the given material. This is a helper function to help the user add only materials that are in the target model.
   *
   */
  Material material_reference(const Material& material) const;

  /**
   * @brief Returns a layer reference from the target model associated with the given material.  This is a helper function to help the user add only layers that are in the target model.
   *
   */
  Layer layer_reference(const Layer& layer) const;

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
