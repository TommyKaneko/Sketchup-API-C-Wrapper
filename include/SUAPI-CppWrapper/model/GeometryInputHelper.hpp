//
//  GeometryInputHelper.hpp
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

#ifndef GeometryInputHelper_hpp
#define GeometryInputHelper_hpp

#include <vector>
#include <unordered_map>

#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"

namespace CW {

/**
 * @brief MaterialDictionary class is a way to retrieve a material that is already attached to a target model, given the associated material input.
 *
 */
class MaterialDictionary {
  friend class GeometryInput;
  private:
  std::unordered_map<Material, Material> m_material_map;
  Model* m_target_model;

  MaterialDictionary(Model* target_model);
  MaterialDictionary(Model* target_model, size_t initial_capacity);

  bool add_reference(const Material& key, const Material& value);

  Material get_reference(const Material& key) const;


};

/**
 * @brief LayerDictionary class is a way to retrieve a material that is already attached to a target model, given the associated material input.
 *
 */
class LayerDictionary {
  friend class GeometryInput;
  private:
  std::unordered_map<Layer, Layer> m_layer_map;
  Model* m_target_model;

  LayerDictionary(Model* target_model);
  LayerDictionary(Model* target_model, size_t initial_capacity);

  bool add_reference(const Layer& key, const Layer& value);

  Layer get_reference(const Layer& key) const;


};

} // namespace CW

#endif /* GeometryInputHelper_hpp */
