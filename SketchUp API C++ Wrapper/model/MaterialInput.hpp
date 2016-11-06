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

class MaterialInput {
	private:
  SUMaterialInput m_material_input;
  bool m_release_on_destroy;
  
  public:
  MaterialInput();
  MaterialInput(SUMaterialInput material_input);
  
  SUMaterialInput ref() { return m_material_input;}
};

} /* namespace CW */

#endif /* MaterialInput_hpp */
