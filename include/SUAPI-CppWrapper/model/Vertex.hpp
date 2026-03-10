//
//  Vertex.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
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

#include <SketchUpAPI/model/vertex.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/Geometry.hpp"

namespace CW {

/**
 * @brief C++ wrapper for the SketchUp C API's SUVertexRef object.
 *
 * A Vertex is a point in 3D space that is associated with edges, faces, and loops.
 */
class Vertex :public Entity {
  private:  
  static SUVertexRef copy_reference(const Vertex& other);
  
  public:
  /** @brief Constructor for null Vertex value */
  Vertex();

  /**
   * @brief Constructs a Vertex from a pre-existing SUVertexRef.
   * @param vertex the SUVertexRef to wrap.
   */
  Vertex(SUVertexRef vertex);

  /** @brief Copy constructor */
  Vertex(const Vertex& other);
  
  /** @brief Destructor */
  ~Vertex();

  /** @brief Copy assignment operator */
  Vertex& operator=(const Vertex& other);
  
  /**
   * @brief Retrieves the position of the vertex.
   * @return Point3D position of the vertex.
   * @throws std::logic_error if the vertex is null.
   */
  Point3D position() const;
  
  /**
   * @brief Returns the SUVertexRef object for the Vertex.
   */
  SUVertexRef ref() const;

  /**
   * @brief Conversion operator to SUVertexRef.
   */
  operator SUVertexRef() const;

  /**
   * @brief Conversion operator to SUVertexRef pointer.
   */
  operator SUVertexRef*();
  
  /**
   * @brief Conversion operator to SUPoint3D. Returns the vertex position.
   */
  operator SUPoint3D() const;

  /**
   * @brief Conversion operator to Point3D. Returns the vertex position.
   */
  operator Point3D() const;
};

} /* namespace CW */
#endif /* Vertex_hpp */
