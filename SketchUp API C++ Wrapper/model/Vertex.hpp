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
  
  static SUVertexRef copy_reference(const Vertex& other);
  
  public:
  /** Constructor for null Vertex value */
  Vertex();

  Vertex(SUVertexRef vertex);

	/** Copy constructor */
  Vertex(const Vertex& other);
  
  /** Destructor */
  ~Vertex();

  /** Copy assignment operator */
  Vertex& operator=(const Vertex& other);
  
  /**
  * Returns the position of the vertex.
  */
  Point3D position() const;
  
  /*
  * The class object can be converted to a SUVertexRef without loss of data.
  */
  SUVertexRef ref() const;
  operator SUVertexRef() const;
  operator SUVertexRef*();
  
  /**
  * Returns the position of the vertex. An aliaas of method poistion();
  */
  operator SUPoint3D() const;
  operator Point3D() const;
};

} /* namespace CW */
#endif /* Vertex_hpp */
