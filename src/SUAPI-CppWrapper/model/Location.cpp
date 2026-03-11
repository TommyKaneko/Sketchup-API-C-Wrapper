//
//  Location.cpp
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

#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Location.hpp"

#include <cassert>
#include <stdexcept>

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
Location::Location():
  m_location(SU_INVALID)
{}


Location::Location(SULocationRef location):
  m_location(location)
{}


/*******************
** Public Methods **
********************/
SULocationRef Location::ref() const {
  return m_location;
}


bool Location::operator!() const {
  return SUIsInvalid(m_location);
}


std::pair<double, double> Location::lat_long() const {
  if (!(*this)) {
    throw std::logic_error("CW::Location::lat_long(): Location is null");
  }
  double latitude = 0.0;
  double longitude = 0.0;
  SUResult res = SULocationGetLatLong(m_location, &latitude, &longitude);
  assert(res == SU_ERROR_NONE); _unused(res);
  return std::make_pair(latitude, longitude);
}


void Location::set_lat_long(double latitude, double longitude) {
  if (!(*this)) {
    throw std::logic_error("CW::Location::set_lat_long(): Location is null");
  }
  SUResult res = SULocationSetLatLong(m_location, latitude, longitude);
  if (res == SU_ERROR_INVALID_INPUT) {
    throw std::invalid_argument("CW::Location::set_lat_long(): latitude must be [-90,90] and longitude [-180,180]");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Location::set_north_angle(double north_angle) {
  if (!(*this)) {
    throw std::logic_error("CW::Location::set_north_angle(): Location is null");
  }
  SUResult res = SULocationSetNorthAngle(m_location, north_angle);
  if (res == SU_ERROR_INVALID_INPUT) {
    throw std::invalid_argument("CW::Location::set_north_angle(): north_angle must be [0,360]");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
}

} /* namespace CW */
