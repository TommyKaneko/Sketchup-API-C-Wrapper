//
//  Layer.hpp
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

#ifndef Layer_hpp
#define Layer_hpp

#include <stdio.h>
#include "SUAPI-CppWrapper/model/Entity.hpp"

#include <SketchUpAPI/model/layer.h>

namespace CW {
// Forward declarations
class String;

class Layer :public Entity {
  private:  
  static SULayerRef create_layer();
  /**
  * Creates a SULayerRef derived from an existing Layer object.
  * @param other - Layer object to derive the new SULayerRef object from
  * @return if the Layer object is already attached to a model, its SULayerRef object will be returned. If the Material object has not been attached to a model, a new SULayerRef object will be created. Bear in mind all properties will not be copied in the latter case
  */
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
  
  /**
  * Returns a copy of the Layer object, which is not attached to a model.
  */
  Layer copy() const;

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
