//
//  Loop.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Loop_hpp
#define Loop_hpp

#include <stdio.h>
#include "DrawingElement.hpp"
#include "GeometryInput.hpp"

#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/model/geometry_input.h>
#include <SketchUpAPI/model/loop.h>

namespace CW {

class Loop {
  private:
  SULoopRef m_loop;
  
  public:
  /*
  * Creates a Loop object from the SULoopRef.
  * @param SULoopRef object that is already attached to a SUFaceRef
  */
  Loop(SULoopRef loop);

  /*
  * Returns the LoopInput object for this loop. A SULoopInputRef will be created using the values of the original SULoopRef object.
  */
  LoopInput loop_input();
  
  /**
  * Returns the Edges in the Loop
  */
  std::vector<Edge> edges();

  /**
  * Returns the Vertices in the Loop
  */
  std::vector<Vertex> vertices();
};

} /* namespace CW */
#endif /* Loop_hpp */
