//
//  Loop.hpp
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

#ifndef Loop_hpp
#define Loop_hpp

#include <stdio.h>
#include <vector>
//#include "SUAPI-CppWrapper/DrawingElement.hpp"
//#include "SUAPI-CppWrapper/Edge.hpp"

//#include <SketchUpAPI/geometry.h>
//#include <SketchUpAPI/model/geometry_input.h>
#include <SketchUpAPI/model/loop.h>

namespace CW {

// Forward Declarations
class LoopInput;
class Edge;
class Vertex;
class Point3D;

/**
* PointLoopClassify is used to describe where a point sits on a loop object.
*/
enum class PointLoopClassify {
	PointOutside,
  PointInside,
  PointOnVertex,
  PointOnEdge,
  PointNotOnPlane,
  PointUnknown // indicates an error
};

/**
* A face is made up of an outer loop and inner loops.
*/
class Loop {
  private:
  SULoopRef m_loop;
  
  public:
  /*
  * Creates a Loop object from the SULoopRef.
  * @param SULoopRef object that is already attached to a SUFaceRef
  */
  Loop(SULoopRef loop);
  
  /**
  * Creates an invalid Loop object.
  */
  Loop();
  
  /**
  * Tests for valid object.
  */
  bool operator!() const;

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
  * Determine where on the loop a point lies.  @see PointLoopClassify.
  * @param point - the Point3D object ot check.
  */
  PointLoopClassify classify_point(const Point3D& point) const;
  
  /**
  * Returns the number of edges/vertices in the loop.
  */
  size_t size() const;
  
  /**
  * Returns the SULoopRef object stored in this loop.
  */
  SULoopRef ref() const;
  
  /**
  * Returns whether a point is within a loop, given by the vector of points.
  * @param loop_points - a vector of points representing the vertices of a loop.
  * @param test_point - the point to test within the loop.
  * @return PointLoopClassify object describing the location of the point relative to the loop.
  */
  static PointLoopClassify classify_point(const std::vector<Point3D>& loop_points, const Point3D& test_point);
  
};

} /* namespace CW */
#endif /* Loop_hpp */
