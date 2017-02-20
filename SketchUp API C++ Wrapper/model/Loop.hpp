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
#include <vector>
//#include "DrawingElement.hpp"
//#include "Edge.hpp"

//#include <SketchUpAPI/geometry.h>
//#include <SketchUpAPI/model/geometry_input.h>
#include <SketchUpAPI/model/loop.h>

namespace CW {

// Forward Declarations
class LoopInput;
class Edge;
class Vertex;
class Point3D;

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
  LoopInput loop_input() const;
  
  /**
  * Returns the Edges in the Loop
  */
  std::vector<Edge> edges() const;

  /**
  * Returns the Vertices in the Loop
  */
  std::vector<Vertex> vertices() const;
  
  /**
  * Returns the points representing the vertices in the Loop
  */
  std::vector<Point3D> points() const;
  
  /**
  * Returns the SULoopRef object stored in this loop.
  */
  SULoopRef ref() const;
  
};

} /* namespace CW */
#endif /* Loop_hpp */
