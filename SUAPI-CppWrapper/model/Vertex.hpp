//
//  Vertex.hpp
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
#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>

#include <SketchUpAPI/model/vertex.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"

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
