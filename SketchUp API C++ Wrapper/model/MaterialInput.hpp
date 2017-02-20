//
//  MaterialInput.hpp
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

#ifndef MaterialInput_hpp
#define MaterialInput_hpp

#include <stdio.h>

#include <SketchUpAPI/model/geometry_input.h>

namespace CW {

// Forward declarations
class Material;

class MaterialInput {
	private:
  SUMaterialInput m_material_input;
	
  /**
  * Creates a SUMaterialInput struct, with default values derived from the Material object.
  */
  static SUMaterialInput create_material_input(Material material);
  
  public:
  //MaterialInput();
  MaterialInput(Material material);
  MaterialInput(SUMaterialInput material_input);
  
  SUMaterialInput ref();
};

} /* namespace CW */

#endif /* MaterialInput_hpp */
