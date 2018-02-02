//
//  ComponentInstance.hpp
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

#ifndef ComponentInstance_hpp
#define ComponentInstance_hpp

#include <stdio.h>

#include <SketchUpAPI/model/component_instance.h>

#include "SUAPI-CppWrapper/Transformation.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"

namespace CW {

class ComponentInstance :public DrawingElement {
  protected:
  SUComponentInstanceRef m_instance;
  
  /** Creates a copy of the ComponentInstance */
  SUComponentInstanceRef copy_reference(const ComponentInstance& other);

  public:
  ComponentInstance(SUComponentInstanceRef instance, bool attached = true);
  
  /** Copy constructor */
  ComponentInstance(const ComponentInstance& other);
  
  /** Copy constructor for derived classes (Group) */
  ComponentInstance(const ComponentInstance& other, SUComponentInstanceRef instance_ref);
  
  /** Destructor */
  ~ComponentInstance();

  /** Copy assignment operator */
  ComponentInstance& operator=(const ComponentInstance& other);
  

  
  /**
  * Returns the raw SUComponentInstance object.
  */
  SUComponentInstanceRef ref();

  /** Cast to native objects */
  operator SUComponentInstanceRef() const;
  operator SUComponentInstanceRef*();
  
  /**
  * Returns the Transformation object applied to this instance.
  */
  Transformation transformation() const;
  
  /**
  * Set the transformation applied to this Component Instance
  */
  void transformation(const Transformation& tranform);

  /**
  * Returns the Component Definition object of this instance.
  */
  ComponentDefinition definition() const;

  /**
  * Returns the name of this instance.
  */
  String name() const;

  /**
  * Sets the name of this instance.
  */
  void name(const String& string);
  
};

} /* namespace CW */
#endif /* ComponentInstance_hpp */
