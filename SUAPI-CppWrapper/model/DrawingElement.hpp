//
//  DrawingElement.hpp
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

#ifndef DrawingElement_hpp
#define DrawingElement_hpp

#include <stdio.h>

#include <SketchUpAPI/model/drawing_element.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"
namespace CW {

// Forward declarations:
class Entity;
class Layer;
class Material;

/*
* Drawing Element wrapper
*/
class DrawingElement :public Entity {
	protected:
  SUDrawingElementRef m_drawing_element;
  
  public:
 	/**
  * Most common constructor.
  * @param drawing_element - SUDrawingElementRef object to assign to the object.
  * @param attached - flag indicating if the entity should be treated as attached to another object @see CW::Entity
  */
  DrawingElement(SUDrawingElementRef drawing_element, bool attached = true);
  
 	/**
  * Copy constructor with an optional parameter for the entity reference.  SUDrawingElementRef objects cannot be created from this class, so the Ref object must be passed to this constructor from a derived class object.
  * @param other - DrawingElement object from which properties will be copied.
  * @param element_ref - SUDrawingElementRef object to assign to the copied object.
  */
  DrawingElement(const DrawingElement& other, SUDrawingElementRef element_ref = SU_INVALID);
  
  /**
  * Constructor representing a null object.
  */
  DrawingElement();
  
  /** Copy assignment operator */
  DrawingElement& operator=(const DrawingElement& other);
  
  /** Cast to native objects */
  operator SUDrawingElementRef() const;
  operator SUDrawingElementRef*();
  
  /**
  * Returns the bounding box of the drawing element.
  */
  BoundingBox3D bounds();
  
  /**
  * Copies all DrawingElement properties from the given element to this one.
  * @param element DrawingElement object from which to get properties
  * @return bool true for success, or false for a failure.
  */
  bool copy_properties_from(const DrawingElement& element);
  
  /*
  * Determine if the Drawingelement is casting shadows.
  * @return true if the Drawingelement is casting shadows, false if not.
  */
  bool casts_shadows() const;
  
  /*
  * Sets the Drawingelement to cast shadows.
  * @param bool settign for casting shadows.
  * @return true for successful, false for unsuccessful.
  */
  bool casts_shadows(bool casts_shadows);

  /*
  * Determine if the Drawingelement is hidden.
  * @return true if the Drawingelement is hidden.
  */
  bool hidden() const;
  
  /*
  * Set the hidden status for an element.
  * @param bool true if you want to hide the element, false if you do not want to hide the element.
  * @return true for successful, false for unsuccessful.
  */
  bool hidden(bool hidden);
  
  /*
  * Retrieves the Layer object of the drawing element.
  */
  Layer layer() const;
  
  /*
  * Sets the Layer for the drawing element
  * @param Layer object
  */
  bool layer(Layer& layer);

  /*
  * Retrieves the Material object of the drawing element.
  * @return Material object assigned to the drawing element.
  */
  Material material() const;
  
  /*
  * Sets the Material of the drawing element.
  * @param Material object to assign to the drawing element.
  * @return Material object assigned to the drawing element.
  */
  bool material(const Material& material);
  
  /*
  * Determine if the Drawingelement receives shadows.
  * @return true if the Drawingelement receives shadows, false if not.
  */
  bool receive_shadows() const;

  /*
  * Set the Drawingelement to receive shadows.
  * @param bool true if you want the element to receive shadows, false if you not.
  * @return true for successful, false for unsuccessful.
  */
  bool receive_shadows(bool receive_shadows);
};

} /* namespace CW */
#endif /* DrawingElement_hpp */
