//
//  Layer.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Layer_hpp
#define Layer_hpp

#include <stdio.h>
#include "Entity.hpp"

#include <SketchUpAPI/model/layer.h>

namespace CW {

class Layer :public Entity {
	private:
  SULayerRef m_layer;
  public:
  Layer();
  Layer(SULayerRef layer_ref, bool release_on_destroy = false);
  
  SULayerRef ref();
  
  operator SULayerRef();
  operator SULayerRef*();
};

} /* namespace CW */

#endif /* Layer_hpp */
