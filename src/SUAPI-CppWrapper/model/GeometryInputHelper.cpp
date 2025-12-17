
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

#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"

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


} /* namespace CW */


