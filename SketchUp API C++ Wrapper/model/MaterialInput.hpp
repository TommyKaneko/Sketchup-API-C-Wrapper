//
//  MaterialInput.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 08/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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
