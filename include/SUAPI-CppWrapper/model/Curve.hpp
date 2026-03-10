//
//  Curve.hpp
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

#ifndef Curve_hpp
#define Curve_hpp

#include <vector>

#include <SketchUpAPI/model/curve.h>

#include "SUAPI-CppWrapper/model/Edge.hpp"

namespace CW {

/**
 * @brief C++ wrapper for the SketchUp C API's SUCurveRef object.
 *
 * A Curve represents a series of connected Edge objects that form a smooth path.
 */
class Curve :public Entity {
  private:
  SUCurveType m_curve_type;
  SUResult m_create_result;
  
  static SUCurveRef create_curve(std::vector<Edge>& edges, SUResult &result);
  
  public:
  /**
   * @brief Creates a Curve from an array of Edge objects.
   * @param edges vector of Edge objects forming the curve.
   * @param curve_type the type of curve (default: SUCurveType_Simple).
   */
  Curve(std::vector<Edge> edges, SUCurveType curve_type = SUCurveType_Simple);

  /**
   * @brief Constructs a Curve from a pre-existing SUCurveRef.
   * @param curve the SUCurveRef to wrap.
   * @param curve_type the type of curve.
   */
  Curve(SUCurveRef curve, SUCurveType curve_type);
  
  /** @brief Destructor */
  ~Curve();
  
  /**
   * @brief Returns the SUCurveRef object for this curve.
   */
  SUCurveRef ref() const;
  
  /**
   * @brief Retrieves the edges that make up the curve.
   * @return vector of Edge objects.
   * @throws std::logic_error if the curve is null.
   */
  std::vector<Edge> get_edges() const;

  /**
   * @brief Retrieves the type of curve.
   * @return SUCurveType value.
   * @throws std::logic_error if the curve is null.
   */
  SUCurveType get_type();
  
  /**
   * @brief Returns the SUResult from when the Curve was created.
   * @return SUResult indicating the creation status.
   */
  SUResult get_result() const;
  
  /**
   * @brief Returns whether the Curve is valid (was created successfully).
   */
  operator bool() const;
  
  /**
   * @brief NOT operator. Checks if the Curve is invalid.
   * @return true if the curve is invalid.
   */
  bool operator!() const;
  
};

} /* namespace CW */

#endif /* Curve_hpp */
