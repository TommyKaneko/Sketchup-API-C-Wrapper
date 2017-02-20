//
//  MaterialInput.cpp
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

#include "MaterialInput.hpp"

#include "Material.hpp"

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
