//
//  MaterialInput.cpp
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/MaterialInput.hpp"

#include "SUAPI-CppWrapper/model/Material.hpp"

namespace CW {

//MaterialInput::MaterialInput():{}

SUMaterialInput MaterialInput::create_material_input(Material material)
{
  SUMaterialInput material_input{};
  material_input.num_uv_coords = 0; // TODO
  //material_input.uv_coords;
  //material_input.vertex_indices;
  material_input.material = material.ref();
  return material_input;
}


MaterialInput::MaterialInput(Material material):
  MaterialInput(create_material_input(material))
{}


MaterialInput::MaterialInput(SUMaterialInput material_input):
  m_material_input(material_input)
{}


SUMaterialInput MaterialInput::ref()
{
  return m_material_input;
}


} /* namespace CW */
