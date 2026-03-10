//
//  GeometryInputHelper.hpp
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

#ifndef GeometryInputHelper_hpp
#define GeometryInputHelper_hpp

#include <vector>
#include <unordered_map>

#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/GeometryInput.hpp"

namespace CW {

// Forward declarations:
class Model;
class Entities;
class Face;
class Edge;
class MaterialInput;
class MaterialPositionInput;
class Loop;
class LoopInput;

/**
 * @brief MaterialDictionary class is a way to retrieve a material that is already attached to a target model, given the associated material input.
 *
 */
class MaterialDictionary {
  friend class GeometryInputPlus;
  private:
  std::unordered_map<Material, Material> m_material_map;
  Model* m_target_model;

  public:
  MaterialDictionary(Model* target_model);
  MaterialDictionary(Model* target_model, size_t initial_capacity);

  bool add_reference(const Material& key, const Material& value);

  Material get_reference(const Material& key) const;


};

/**
 * @brief LayerDictionary class is a way to retrieve a layer that is already attached to a target model, given the associated layer input.
 *
 */
class LayerDictionary {
  friend class GeometryInputPlus;
  private:
  std::unordered_map<Layer, Layer> m_layer_map;
  Model* m_target_model;

  public:
  LayerDictionary(Model* target_model);
  LayerDictionary(Model* target_model, size_t initial_capacity);

  bool add_reference(const Layer& key, const Layer& value);

  Layer get_reference(const Layer& key) const;


};


/**
 * @brief GeometryInputPlus is a derived class of GeometryInput that adds material and layer management.
 * It provides higher-level operations for copying faces and edges with their materials and layers,
 * ensuring that materials and layers are properly added to the target model.
 */
class GeometryInputPlus : public GeometryInput {
  friend class Entities;

private:
  /**
   * Store a lookup table of materials that can be applied to geometry, given the target_model given.
   */
  MaterialDictionary m_material_dict;

  /**
   * Store a lookup table of layers that can be applied to geometry, given the target_model given.
   */
  LayerDictionary m_layer_dict;

  // GeometryInputPlus objects require that the target model (for inputting information) be known, to ensure that materials and layers assigned to geometry exists in the target model.
  Model* m_target_model;

public:
  /**
  * Creates a valid, but empty GeometryInputPlus object.
  * @param target_model - model which will receive this GeometryInput object.  This is required to ensure that Layers(Tags) and Materials that are added to the model are checked for validity.
  */
  GeometryInputPlus(Model* target_model);

  /** Copy Constructor **/
  GeometryInputPlus(const GeometryInputPlus& other);

  /** Destructor */
  ~GeometryInputPlus() override = default;

  /**
  * Copy assignment operator.
  */
  GeometryInputPlus& operator=(const GeometryInputPlus& other);

  /**
   * @brief Returns the target model associated with this GeometryInputPlus object.
   */
  Model* target_model() const;

  /**
   * @brief Prepares the GeometryInputPlus object's materials, and if required, add materials to the target model.
   * @param materials
   */
  void load_materials(const std::vector<Material>& materials);

  /**
   * @brief Loads a pre-built MaterialDictionary into the GeometryInputPlus object's material dictionary explicitly.
   * @param material_dictionary
   */
  void load_materials(MaterialDictionary material_dictionary);

  /**
   * @brief Prepares the GeometryInputPlus object's layers, and if required, add layers to the target model.
   * @param layers
   */
  void load_layers(const std::vector<Layer>& layers);

  /**
   * @brief Loads a pre-built LayerDictionary into the GeometryInputPlus object's layer dictionary explicitly.
   * @param layer_dictionary
   */
  void load_layers(LayerDictionary layer_dictionary);

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

  // Bring the base class add_face(LoopInput&) into scope
  using GeometryInput::add_face;

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

  // Bring the base class add_edge(size_t, size_t) into scope
  using GeometryInput::add_edge;

  /**
   * @brief Adds an Edge with Material and Layer assigned to it.  The number of edges and number of materials and layers must be equal, so that each edge can be associated with a material/layer
   * @return size_t - index of the last added edge
   */
  size_t add_edges(const std::vector<Edge>& edges, const std::vector<Material>& materials, const std::vector<Layer>& layers);

  /**
   * Helper methods that copies all entities from an Entities object into this GeometryInput object.
   * Note: it will not copy nested ComponentDefinitions or Groups.
   */
  void add_entities(const Entities& entities);

  #if SketchUpAPI_VERSION_MAJOR >= 2021
  /**
  * Sets the front material of a face in the geometry input, with model validation.
  * @since Sketchup API 2020.1, API 9.1
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_front_material_position(size_t face_index, const MaterialPositionInput& material_input);

  /**
  * Sets the back material of a face in the geometry input, with model validation.
  * @since Sketchup API 2020.1, API 9.1
  * @param face_index - Index of the face to receive the material.
  * @param material_input - The material input to set.
  */
  void face_back_material_position(size_t face_index, const MaterialPositionInput& material_input);
  #endif

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
};

} // namespace CW

#endif /* GeometryInputHelper_hpp */
