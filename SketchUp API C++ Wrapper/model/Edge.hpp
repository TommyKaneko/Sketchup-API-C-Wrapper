//
//  Edge.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>

#include <SketchUpAPI/model/edge.h>

#include "Geometry.hpp"
#include "DrawingElement.hpp"
#include "Vertex.hpp"
#include "Color.hpp"
namespace CW {

class Edge :public DrawingElement {
	private:
  SUEdgeRef m_edge;
  bool m_release_on_destroy; // Indicates whether the SUEdgeRef should be released on destruction of the object.
	SU_RESULT m_create_result;
  
  static SUEdgeRef create_edge(std::vector<SUPoint3D> points, SU_RESULT &result);
  
  public:
  
  /*
  * Face constructor, which takes an array of points representing the outer loop, and an array of arrays of points representing the inner loops of the face.  A new SUFaceRef object will be created within this class and handled internally.
  */
  Edge(std::vector<SUPoint3D> points);
  Edge(SUPoint3D start, SUPoint3D end);
  
  /*
  * Face constructor that essentially wraps around an already created SUFaceRef object.
  * @param SUFaceRef* pointer to the face.
  * @param bool true if the face should be released when this class object is destroyed.  False, if the release of the face object is handled elsewhere (use with caution).
  */
  Edge(SUEdgeRef edge, bool release_on_destroy = false);

  ~Edge();
  
  
  /**
  * Returns SUEdgeRef object for the Edge.
  */
  SUEdgeRef ref() const;

  /*
  * The class object can be converted to a SUEdgeRef without loss of data.
  */
  operator SUEdgeRef();
  operator SUEdgeRef*();
  
  /*
  * Returns whether the class is a valid object.
  */
  operator bool() const;
  /**
  * NOT operator.  Checks if the SUEdgeRef is valid.
  * @return true if the edge is invalid
  */
	bool operator!() const;

	/*
  * Returns the Color object assigned to the Edge
  */
  Color color() const;
  
  /*
  * Sets the color of the Edge.
  */
  bool color(Color input_color);
  
  /*
  * Return the vertex at the end of the Edge.
  */
  Vertex end() const;

  /*
  * Gets the SU_RESULT of the create edge operation.
  * @return * SU_ERROR_NONE on success
            * SU_ERROR_NULL_POINTER_INPUT if start or end is NULL
            * SU_ERROR_NULL_POINTER_OUTPUT if edge is NULL
            * SU_ERROR_GENERIC if start and end specify the same position.
  */
  SU_RESULT get_result() const;

  /*
  * Determine if the Edge is smooth.
  * @return true if the Edge is smooth.
  */
  bool smooth() const;
  
  /*
  * Set the smooth status for an Edge.
  * @param bool true to make the edge smooth, false to make the edge hard.
  * @return true for successful, false for unsuccessful.
  */
  bool smooth(bool smooth);
  
  /*
  * Determine if the Edge is soft.
  * @return true if the Edge is soft.
  */
  bool soft() const;
  
  /*
  * Set the soft status for an edge.
  * @param bool true if you want soften the edge, false if you do not want to soften the edge.
  * @return true for successful, false for unsuccessful.
  */
  bool soft(bool soft);

  /*
  * Return the vertex at the start of the Edge.
  */
  Vertex start() const;
  
  /*
  * Return Vector between the start and end points
  */
  Vector3D vector() const;
};

} /* namespace CW */
#endif /* Edge_hpp */
