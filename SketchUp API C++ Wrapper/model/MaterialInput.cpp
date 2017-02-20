//
//  MaterialInput.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 08/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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
