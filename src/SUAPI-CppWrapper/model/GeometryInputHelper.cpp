
//
//  GeometryInputHelper.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2025 Tom Kaneko
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
#include <iostream>

#include "SUAPI-CppWrapper/model/GeometryInputHelper.hpp"

#include <SketchupAPI/sketchup_info.h>

#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/LoopInput.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/MaterialInput.hpp"

namespace CW {

/***
 * MaterialDictionary Class
 */

MaterialDictionary::MaterialDictionary(Model* target_model):
  MaterialDictionary(target_model, target_model->num_materials())
{}


MaterialDictionary::MaterialDictionary(Model* target_model, size_t initial_capacity):
  m_target_model(target_model)
{
  m_material_map.reserve(initial_capacity);
}


bool MaterialDictionary::add_reference(const Material& key, const Material& value) {
  bool success =  m_material_map.insert({key,value}).second;
  return success;
}


Material MaterialDictionary::get_reference(const Material& key) const {
  std::unordered_map<Material, Material>::const_iterator found_material = m_material_map.find(key);
  if (found_material == m_material_map.end()) {
    throw std::logic_error("CW::MaterialDictionary::get_reference(): material was not found in the Dictionary - ensure target model is loaded with all the materials needed, and build the mapping of MaterialDictionary prior to using this method.");
  }
  return found_material->second;
}


/***
 * LayerDictionary Class
 */
LayerDictionary::LayerDictionary(Model* target_model):
  LayerDictionary(target_model, target_model->num_layers())
{}


LayerDictionary::LayerDictionary(Model* target_model, size_t initial_capacity):
  m_target_model(target_model)
{
  m_layer_map.reserve(initial_capacity);
}


bool LayerDictionary::add_reference(const Layer& key, const Layer& value) {
  bool success =  m_layer_map.insert({key,value}).second;
  return success;
}


Layer LayerDictionary::get_reference(const Layer& key) const {
  std::unordered_map<Layer, Layer>::const_iterator found_layer = m_layer_map.find(key);
  if (found_layer == m_layer_map.end()) {
    throw std::logic_error("CW::LayerDictionary::get_reference(): layer was not found in the Dictionary - ensure target model is loaded with all the layers needed, and build the mapping of LayerDictionary prior to using this method.");
  }
  return found_layer->second;
}


/***
 * GeometryInputPlus Class
 */

GeometryInputPlus::GeometryInputPlus(Model* target_model):
  GeometryInput(),
  m_target_model(target_model),
  m_material_dict(target_model),
  m_layer_dict(target_model)
{}


GeometryInputPlus::GeometryInputPlus(const GeometryInputPlus& other):
  GeometryInput(other),
  m_target_model(other.m_target_model),
  m_material_dict(other.m_material_dict),
  m_layer_dict(other.m_layer_dict)
{}


GeometryInputPlus& GeometryInputPlus::operator=(const GeometryInputPlus& other) {
  GeometryInput::operator=(other);
  m_target_model = other.m_target_model;
  m_material_dict = other.m_material_dict;
  m_layer_dict = other.m_layer_dict;
  return (*this);
}


Model* GeometryInputPlus::target_model() const {
  return m_target_model;
}


void GeometryInputPlus::load_materials(const std::vector<Material>& materials) {
  // Check to see if each material is in the model already
  std::vector<Material> target_materials = m_target_model->materials();
  std::vector<Material> materials_to_add; materials_to_add.reserve(materials.size());
  for (const Material& material : materials) {
    bool matched = false;
    for (const Material& target_material : target_materials) {
      if (material == target_material) {
        // We can directly add the material to the map
        m_material_dict.add_reference(material, target_material);
        matched = true;
        break;
      }
      else if (material.name() == target_material.name()) {
        // match with name
        m_material_dict.add_reference(material, target_material);
        matched = true;
        break;
      }
    }
    if (!matched) {
      // Create a new material in the target model
      materials_to_add.push_back(material);
    }
  }
  if (materials_to_add.size() == 0) return;
  // Add the materials that do not exist in the model.
  m_target_model->add_materials(materials_to_add, false);
  target_materials = m_target_model->materials(); // Refresh the list of materials.
  for (const Material& material : materials_to_add) {
    for (const Material& target_material : target_materials) {
      if (material == target_material) {
        // We can directly add the material to the map
        m_material_dict.add_reference(material, target_material);
        break;
      }
      else if (material.name() == target_material.name()) {
        // match with name
        m_material_dict.add_reference(material, target_material);
        break;
      }
    }
  }
  return;
}

void GeometryInputPlus::load_materials(MaterialDictionary material_dictionary) {
  m_material_dict = material_dictionary;
}

void GeometryInputPlus::load_layers(const std::vector<Layer>& layers) {
  // Check to see if each layer is in the model already
  std::vector<Layer> target_layers = m_target_model->layers();
  std::vector<Layer> layers_to_add; layers_to_add.reserve(layers.size());
  for (const Layer& layer : layers) {
    bool matched = false;
    for (const Layer& target_layer : target_layers) {
      if (layer == target_layer) {
        // We can directly add the layer to the map
        m_layer_dict.add_reference(layer, target_layer);
        matched = true;
        break;
      }
      else if (layer.name() == target_layer.name()) {
        // match with name
        m_layer_dict.add_reference(layer, target_layer);
        matched = true;
        break;
      }
    }
    if (!matched) {
      // Create a new layer in the target model
      layers_to_add.push_back(layer);
    }
  }
  if (layers_to_add.size() == 0) return;
  // Add the layers that do not exist in the model.
  m_target_model->add_layers(layers_to_add, false);
  target_layers = m_target_model->layers(); // Refresh the list of layers.
  for (const Layer& layer : layers_to_add) {
    for (const Layer& target_layer : target_layers) {
      if (layer == target_layer) {
        // We can directly add the layer to the map
        m_layer_dict.add_reference(layer, target_layer);
        break;
      }
      else if (layer.name() == target_layer.name()) {
        // match with name
        m_layer_dict.add_reference(layer, target_layer);
        break;
      }
    }
  }
  return;
}

void GeometryInputPlus::load_layers(LayerDictionary layer_dictionary) {
  m_layer_dict = layer_dictionary;
}


size_t GeometryInputPlus::add_face(const Face &face, bool copy_material_layer) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInputPlus::add_face(): GeometryInput is null");
  }
  if(!face) {
    throw std::invalid_argument("CW::GeometryInputPlus::add_face(): Face argument is null");
  }
  LoopInput outer_loop_input;
  // Add outer loop
  std::vector<Point3D> outer_points = face.outer_loop().points();
  if(outer_points.size() < 3) {
    throw std::logic_error("CW::GeometryInputPlus::add_face(): face has less than 3 points in outer loop");
  }
  std::vector<Edge> outer_edges = face.outer_loop().edges();
  size_t first_vertex_index;
  for (size_t i=0; i < outer_points.size(); ++i) {
    size_t v_index = this->add_vertex(outer_points[i]);
    outer_loop_input.add_vertex_index(v_index);
    if (i==0) first_vertex_index = v_index;
    if (outer_edges[i].hidden()) {
      outer_loop_input.set_edge_hidden(i, true);
    }
    if (outer_edges[i].smooth()) {
      outer_loop_input.set_edge_smooth(i, true);
    }
    if (outer_edges[i].soft()) {
      outer_loop_input.set_edge_soft(i, true);
    }
    if (!!outer_edges[i].material()) {
      outer_loop_input.set_edge_material(i, m_material_dict.get_reference(outer_edges[i].material()));
    }
    if (!!outer_edges[i].layer()) {
      outer_loop_input.set_edge_layer(i, m_layer_dict.get_reference(outer_edges[i].layer()));
    }
  }
  outer_loop_input.add_vertex_index(first_vertex_index); // Close the loop TODO: not strictly necessary, and messes with the m_edge_num count...

  size_t added_face_index = GeometryInput::add_face(outer_loop_input);
  // Add inner loops
  std::vector<Loop> inner_loops = face.inner_loops();
  for (size_t i=0; i < inner_loops.size(); ++i) {
    LoopInput inner_loop_input;
    std::vector<Point3D> inner_points = inner_loops[i].points();
    std::vector<Edge> inner_edges = inner_loops[i].edges();
    for (size_t j=0; j < inner_points.size(); ++j) {
      size_t v_index = this->add_vertex(inner_points[j]);
      inner_loop_input.add_vertex_index(v_index);
      if (j==0) first_vertex_index = v_index;
      if (inner_edges[j].hidden()) {
        inner_loop_input.set_edge_hidden(j, true);
      }
      if (inner_edges[j].smooth()) {
        inner_loop_input.set_edge_smooth(j, true);
      }
      if (inner_edges[j].soft()) {
        inner_loop_input.set_edge_soft(j, true);
      }
      if (!!inner_edges[j].material()) {
        inner_loop_input.set_edge_material(j, m_material_dict.get_reference(inner_edges[j].material()));
      }
      if (!!inner_edges[j].layer()) {
        inner_loop_input.set_edge_layer(j, m_layer_dict.get_reference(inner_edges[j].layer()));
      }
    }
    inner_loop_input.add_vertex_index(first_vertex_index); // Close the loop TODO: not strictly necessary, and messes with the m_edge_num count...
    this->face_add_inner_loop(added_face_index, inner_loop_input);
  }
  if (copy_material_layer) {
    // Add layer
    Layer face_layer = face.layer();
    if (!!face_layer) {
      this->GeometryInput::face_layer(added_face_index, m_layer_dict.get_reference(face_layer));
      face_layer.attached(true);
    }
    #if SketchUpAPI_VERSION_MAJOR < 2021
    // Old way of setting the material
    Material front_mat = face.material();
    if (!!front_mat) {
      MaterialInput front_material_input(front_mat);
      this->face_front_material(added_face_index, front_material_input);
    }
    Material back_mat = face.back_material();
    if (!!back_mat) {
      MaterialInput back_material_input(face.back_material());
      assert(m_target_model->material_exists(back_material_input.material()));
      this->face_back_material(added_face_index, back_material_input);
    }
    #else
    // New way of setting the material with MaterialInputPosition
    if (!!face.material()) {
      MaterialPositionInput material_input = face.material_position_front();
      // Replace material with one in the target model
      material_input.material(this->material_reference(face.material()));
      this->face_front_material_position(added_face_index, material_input);
    }
    if (!!face.back_material()) {
      MaterialPositionInput material_input = face.material_position_back();
      material_input.material(this->material_reference(face.back_material()));
      this->face_back_material_position(added_face_index, material_input);
    }
    #endif
    // TODO: create a way to add attributes to the faces.
  }
  return added_face_index;
}


size_t GeometryInputPlus::add_faces(const std::vector<Face>& faces, bool copy_material_layer) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInputPlus::add_faces(): GeometryInput is null");
  }
  size_t index = 0;
  for (size_t i=0; i < faces.size(); ++i) {
    index = add_face(faces[i]);
  }
  return index;
}


size_t GeometryInputPlus::add_edge(const Edge &edge, const Material& material, const Layer& layer) {
  if(!edge) {
    throw std::invalid_argument("CW::GeometryInputPlus::add_edge(): Edge argument is null");
  }
  if (SU_API_VERSION_MAJOR < 5) {
    // Edges can only be added since SU2017
    return 0;
  }
  Point3D start_point = edge.start().position();
  size_t start_index = this->add_vertex(start_point);
  Point3D end_point = edge.end().position();
  size_t end_index = this->add_vertex(end_point);
  size_t added_edge_index = GeometryInput::add_edge(start_index, end_index);

  // Add other information about the edge
  if (edge.hidden()) {
    this->edge_hidden(added_edge_index, true);
  }
  if (edge.smooth()) {
    this->edge_smooth(added_edge_index, true);
  }
  if (edge.soft()) {
    this->edge_soft(added_edge_index, true);
  }
  if (!!material) {
    this->edge_material(added_edge_index, m_material_dict.get_reference(material));
  }
  if (!!layer) {
    this->edge_layer(added_edge_index, m_layer_dict.get_reference(layer));
  }
  return added_edge_index;
}


size_t GeometryInputPlus::add_edges(const std::vector<Edge>& edges) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInputPlus::add_edges(): GeometryInput is null");
  }
  size_t index = 0;
  for (size_t i=0; i < edges.size(); ++i) {
    index = add_edge(edges[i]);
  }
  return index;
}


size_t GeometryInputPlus::add_edges(const std::vector<Edge>& edges, const std::vector<Material>& materials, const std::vector<Layer>& layers) {
  if(!(*this)) {
    throw std::logic_error("CW::GeometryInputPlus::add_edges(): GeometryInput is null");
  }
  if(edges.size() != materials.size() || edges.size() != layers.size()) {
    throw std::logic_error("CW::GeometryInputPlus::add_edges(): size of layers and materials vector must match the size of edges");
  }
  size_t edge_index = 0;
  for (size_t i=0; i < edges.size(); ++i) {
    edge_index = this->add_edge(edges[i], materials[i], layers[i]);
  }
  return edge_index;
}


void GeometryInputPlus::add_entities(const Entities& entities) {
  // Add materials
  std::vector<Material> materials = entities.model().materials();
  this->load_materials(materials);
  // Add layers
  std::vector<Layer> layers = entities.model().layers();
  this->load_layers(layers);
  // Add faces
  std::vector<Face> faces = entities.faces();
  this->add_faces(faces, true);
  // Add edges
  std::vector<Edge> edges = entities.edges();
  this->add_edges(edges);
}


#if SketchUpAPI_VERSION_MAJOR >= 2021
void GeometryInputPlus::face_front_material_position(size_t face_index, const MaterialPositionInput& material_input) {
  Material material = material_input.material();
  if (material.model() != *this->m_target_model) {
    throw std::logic_error("CW::GeometryInputPlus::face_front_material_position(): material does not reference a material in the target model");
  }
  GeometryInput::face_front_material_position(face_index, material_input);
}

void GeometryInputPlus::face_back_material_position(size_t face_index, const MaterialPositionInput& material_input) {
  Material material = material_input.material();
  if (material.model() != *this->m_target_model) {
    throw std::logic_error("CW::GeometryInputPlus::face_back_material_position(): material does not reference a material in the target model");
  }
  GeometryInput::face_back_material_position(face_index, material_input);
}
#endif


Material GeometryInputPlus::material_reference(const Material& material) const {
  Material reference = m_material_dict.get_reference(material);
  return reference;
}


Layer GeometryInputPlus::layer_reference(const Layer& layer) const {
  Layer reference = m_layer_dict.get_reference(layer);
  return reference;
}

} /* namespace CW */


