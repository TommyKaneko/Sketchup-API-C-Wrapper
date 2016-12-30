//
//  Vertex.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>

#include <SketchUpAPI/model/vertex.h>

#include "Entity.hpp"
#include "Geometry.hpp"

namespace CW {

/*
* Vertex wrapper
*/
class Vertex :public Entity {
	private:
  SUVertexRef m_vertex;
  
  static SUEntityRef entity_from_vertex(const SUVertexRef vertex);
  
  public:
  Vertex(SUVertexRef vertex);
  
  /**
  * Returns the position of the vertex.
  */
  Point3D position() const;
  
  /*
  * The class object can be converted to a SUVertexRef without loss of data.
  */
  operator SUVertexRef();
  operator SUVertexRef*();
  
  /**
  * Returns the position of the vertex. An aliaas of method poistion();
  */
  operator SUPoint3D() const;
  operator Point3D() const;
};

} /* namespace CW */
#endif /* Vertex_hpp */
