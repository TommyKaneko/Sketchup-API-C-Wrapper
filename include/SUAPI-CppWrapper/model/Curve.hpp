//
//  Curve.hpp
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

#ifndef Curve_hpp
#define Curve_hpp

#include <stdio.h>
#include <vector>

#include <SketchUpAPI/model/curve.h>

#include "SUAPI-CppWrapper/model/Edge.hpp"

namespace CW {

class Curve :public Entity {
  private:
  SUCurveType m_curve_type;
  SUResult m_create_result;
  
  static SUCurveRef create_curve(std::vector<Edge>& edges, SUResult &result);
  
  public:
  /**
  * A curve is created from an array of Edge objects.
  * @param edges - a vector array of Edge objects.
  * @param curve_type (optional) the type of curve
  */
  Curve(std::vector<Edge> edges, SUCurveType curve_type = SUCurveType_Simple);
  Curve(SUCurveRef curve, SUCurveType curve_type);
  
  ~Curve();
  
  /**
  * Returns SUCurveRef object of this curve.
  */
  SUCurveRef ref() const;
  
  /**
  * Returns the series of Edge objects in the Curve.
  */
  std::vector<Edge> get_edges() const;

  /**
  * Returns the type of Curve.
  */
  SUCurveType get_type();
  
  /*
  * Returns the SUResult object form when the Curve was created.
  * @return  * SU_ERROR_NONE on success
            * SU_ERROR_NULL_POINTER_INPUT if edges is NULL
            * SU_ERROR_OUT_OF_RANGE if len is 0
            * SU_ERROR_NULL_POINTER_OUTPUT if curve is NULL
            * SU_ERROR_OVERWRITE_VALID if curve already references a valid object
            * SU_ERROR_GENERIC if edge array contains an invalid edge, if the edges in the array are not connected, if any of the edges are associated with a face object, or the edges describe a loop
  */
  SUResult get_result() const;
  
  /*
  * Returns whether the class is a valid object.
  */
  operator bool() const;
  
  /**
  * NOT operator.  Checks if the CurveRef is valid.
  * @return true if the curve is invalid
  */
  bool operator!() const;
  
};

} /* namespace CW */

#endif /* Curve_hpp */
