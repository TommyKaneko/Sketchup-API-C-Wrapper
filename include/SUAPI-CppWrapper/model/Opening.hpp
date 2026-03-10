//
//  Opening.hpp
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

#ifndef Opening_h
#define Opening_h

#include <SketchUpAPI/model/opening.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"


namespace CW {

/**
 * @brief C++ wrapper for the SketchUp C API's SUOpeningRef object.
 *
 * An Opening represents a hole cut in a face by an attached component instance or group.
 * @since SketchUp 2014, API 2.0
 */
class Opening {
  private:
  
  /// Smart pointer used to keep track of copies of this object.
  std::shared_ptr<SUOpeningRef> m_opening;
  
  public:
  /** @brief Constructor for null Opening value */
  Opening();

  /**
   * @brief Constructs an Opening from a raw SUOpeningRef.
   * @param opening the SUOpeningRef to wrap.
   */
  Opening(SUOpeningRef opening);
  
  /**
   * @brief Constructs an Opening from a shared pointer to SUOpeningRef.
   * @param opening shared pointer managing the SUOpeningRef lifetime.
   */
  Opening(std::shared_ptr<SUOpeningRef> opening);

  /** @brief Copy constructor */
  Opening(const Opening& other);
  
  /** @brief Destructor */
  ~Opening();

  /** @brief Copy assignment operator */
  Opening& operator=(const Opening& other);
  
  /**
   * @brief NOT operator. Tests if the Opening is invalid.
   * @return true if the opening is invalid.
   */
  bool operator!() const;
  
  /**
   * @brief Retrieves the number of points of the opening.
   * @return the number of points.
   * @throws std::logic_error if the opening is invalid.
   */
  size_t get_num_points() const;
  
  /**
   * @brief Retrieves the points of the opening.
   * @return vector of Point3D objects.
   * @throws std::logic_error if the opening is invalid.
   */
  std::vector<Point3D> get_points() const;
  
};

}
#endif /* Opening_h */
