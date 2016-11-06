//
//  Material.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include "Entity.hpp"
#include "MaterialInput.hpp"

#include <SketchUpAPI/model/material.h>
#include <SketchUpAPI/model/geometry_input.h>

namespace CW {

class Material :public Entity {
	private:
  SUMaterialRef m_material;
  public:
  Material();
  Material(SUMaterialRef material_ref);
  
  SUMaterialRef ref();
  
  operator SUMaterialRef() const;
  //operator SUMaterialRef&() const;
  operator SUMaterialRef*() const;
  
  /*
  * Returns a MaterialInput object built from the properties of this Material object.
  */
  MaterialInput input();
};

} /* namespace CW */

#endif /* Material_hpp */
