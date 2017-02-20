//
//  Layer.hpp
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
