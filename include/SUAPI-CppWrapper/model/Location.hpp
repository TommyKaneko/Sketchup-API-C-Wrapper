//
//  Location.hpp
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

#ifndef Location_hpp
#define Location_hpp

#include <utility>
#include <SketchUpAPI/model/location.h>

namespace CW {

/**
 * @brief C++ wrapper for SULocationRef.
 *
 * Represents the geographic location information of a model, including
 * latitude, longitude, and north angle. Obtained from Model and cannot
 * be created or released independently.
 *
 * @see SULocationRef
 * @since SketchUp 2013, API 1.0
 */
class Location {
  private:
  SULocationRef m_location;

  public:
  /**
  * @brief Constructs a NULL Location object.
  */
  Location();

  /**
  * @brief Constructs from an existing SULocationRef.
  * @param location - the SULocationRef obtained from the model.
  */
  Location(SULocationRef location);

  /**
  * @brief Returns the underlying SULocationRef.
  */
  SULocationRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null location.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the latitude and longitude of this location.
  * @return A pair of (latitude, longitude) in degrees.
  * @throws std::logic_error if the location is null.
  * @since SketchUp 2013, API 1.0
  */
  std::pair<double, double> lat_long() const;

  /**
  * @brief Sets the latitude and longitude of this location.
  * @param latitude  - latitude in degrees [-90, 90].
  * @param longitude - longitude in degrees [-180, 180].
  * @throws std::logic_error if the location is null.
  * @throws std::invalid_argument if latitude or longitude is out of range.
  * @since SketchUp 2013, API 1.0
  */
  void set_lat_long(double latitude, double longitude);

  /**
  * @brief Sets the north angle of this location.
  * @param north_angle - the north angle in degrees [0, 360].
  * @throws std::logic_error if the location is null.
  * @throws std::invalid_argument if north_angle is out of range.
  * @since SketchUp 2013, API 1.0
  */
  void set_north_angle(double north_angle);
};

} /* namespace CW */

#endif /* Location_hpp */
