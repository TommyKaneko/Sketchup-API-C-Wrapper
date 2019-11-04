//
//  Opening.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2019 Tom Kaneko
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

#ifndef Opening_h
#define Opening_h

#include <stdio.h>

#include <SketchUpAPI/model/opening.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"


namespace CW {

/*
* Opening wrapper
*/
class Opening {
  private:
  
  // Smart pointer used to keep track of copies of this object.
  std::shared_ptr<SUOpeningRef> m_opening;
  
  public:
  /** Constructor for null Opening value */
  Opening();

  /** Unsafe Constructor **/
  Opening(SUOpeningRef opening);
  
  /** Safe Constructor **/
  Opening(std::shared_ptr<SUOpeningRef> opening);

  /** Copy constructor */
  Opening(const Opening& other);
  
  /** Destructor */
  ~Opening();

  /** Copy assignment operator */
  Opening& operator=(const Opening& other);
  
  /*
  * Test for invalid object
  */
  bool operator!() const;
  
  /**
  * Retrieves the number of points of an opening.
  * @since SketchUp 2014, API 2.0
  * @return The number of points.
  */
  size_t get_num_points() const;
  
  /**
  * Retrieves the points of an opening object.
  * @since SketchUp 2014, API 2.0
  * @return The points in the opening.
  */
  std::vector<Point3D> get_points() const;
  
};

}
#endif /* Opening_h */
