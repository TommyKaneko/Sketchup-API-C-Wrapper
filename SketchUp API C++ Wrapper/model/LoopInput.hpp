//
//  LoopInput.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 08/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef LoopInput_hpp
#define LoopInput_hpp

#include <stdio.h>
#include <vector>

#include "Edge.hpp"
#include "GeometryInput.hpp"

#include <SketchUpAPI/model/geometry_input.h>

namespace CW {

class LoopInput {
	private:
  SULoopInputRef m_loop_input;
  bool m_release_on_destroy;
  
  std::vector<Edge> m_edges;
  std::vector<Curve> m_edges;
  
  static SULoopInputRef create_loop_input_ref();
  
  public:
  
  /**
  * Creates a LoopInput object, which will store the properties of the sequence of Edge objects.
  */
  LoopInput(std::vector<Edge> edges);

  /*
  * Creates a Loop object which is used as a LoopInputRef object (i.e. it is not attached to a Face yet)
  * @param vector of SUPoint3D objects, tracing the outline of the loop.
  */
  LoopInput(std::vector<Point3D> points);
  
  LoopInput();
  LoopInput(SULoopInputRef loop_input, bool release_on_destroy);
  
  /*
  * Returns the stored SULoopInputRef object.
  */
  SULoopInputRef ref();

  /*
  * Returns the stored m_edges array.
  */
  std::vector<Edge> get_edges();
  
  /*
  * Returns an array of vertices that represent the loop.
  */
  std::vector<Point3D> get_vertices();
  
  /*
  * Adds the loop to the given 
  */
  SU_RESULT add_to_geometry_input(GeometryInput &geom_input);
  
  /*
  * Adds a series of vertices that represent the loop.
  */
  LoopInput& add_vertices(std::vector<Point3D> points);

  /*
  * Adds an edge on the end of the Loop
  */
  LoopInput& add_edge(Edge edge);
  LoopInput& add_edges(std::vector<Edge> edges);
  
  LoopInput& add_curve(Curve curve);
  LoopInput& add_curves(std::vector<Curve> curves);

};

} /* namespace CW */

#endif /* LoopInput_hpp */
