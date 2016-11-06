//
//  DrawingElement.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef DrawingElement_hpp
#define DrawingElement_hpp

#include <stdio.h>
#include "Entity.hpp"
#include "Layer.hpp"
#include "Material.hpp"
#include <SketchUpAPI/model/drawing_element.h>
namespace CW {

/*
* Drawing Element wrapper
*/
class DrawingElement :public Entity {
	protected:
  SUDrawingElementRef m_drawing_element;
  
  public:
  DrawingElement(SUDrawingElementRef drawing_element);
  
  /**
  * Copies all DrawingElement properties from the given element to this one.
  * @param element DrawingElement object from which to get properties
  * @return bool true for success, or false for a failure.
  */
  bool copy_properties_from(DrawingElement element);
  
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
  bool layer(Layer layer);

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
  bool material(const Material material);
  
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
