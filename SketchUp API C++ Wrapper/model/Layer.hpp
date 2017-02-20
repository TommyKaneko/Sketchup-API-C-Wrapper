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
// Forward declarations
class String;

class Layer :public Entity {
	private:
  SULayerRef m_layer;
  
  static SULayerRef create_layer();
  static SULayerRef copy_reference(const Layer& other);
  
  public:
  /**
  * Constructs a NULL layer.
  */
  Layer();
  
  /**
  * Constructs a Layer object from a pre existing SULayerRef object.
  */
  Layer(SULayerRef layer_ref, bool attached = true);
	
  /** Copy Constructor */
  Layer(const Layer& other);
  
  /** Destructor */
  ~Layer();
  
  /** Copy assignment operator */
  Layer& operator=(const Layer& other);
	
  /**
  * Returns the SULayerRef object that this class wraps
  */
  SULayerRef ref() const;
  operator SULayerRef() const;
  operator SULayerRef*();
  
  /**
  * Returns whether this is a NULL layer or not.
  */
	bool operator!() const;
  //bool operator!() const;
  
  /**
  * Get the name of the layer.
  */
  String name() const;
  
  /**
  * Set the name of the layer.
  */
  void name(const String& string);
  void name(const std::string& string);
  
};

} /* namespace CW */

#endif /* Layer_hpp */
