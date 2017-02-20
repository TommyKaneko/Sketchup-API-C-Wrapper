//
//  Vertex.hpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
