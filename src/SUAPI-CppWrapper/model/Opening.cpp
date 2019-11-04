//
//  Opening.cpp
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Opening.hpp"

#include <cassert>
#include <stdexcept>

namespace CW {
/******************************
** Constructors / Destructor **
*******************************/
Opening::Opening()
  : m_opening(std::shared_ptr<SUOpeningRef>SU_INVALID)
{}


Opening::Opening(SUOpeningRef ref)
  : m_opening(std::shared_ptr<SUOpeningRef>(&ref))
{}


Opening::Opening(std::shared_ptr<SUOpeningRef> opening)
  : m_opening(opening)
{}


Opening::Opening(const Opening& other)
  : m_opening(!other ? std::shared_ptr<SUOpeningRef>SU_INVALID : other.m_opening)
{}


Opening::~Opening()
{
  if (!!(*this) && m_opening.use_count() < 2) {
    SU_RESULT res = SUOpeningRelease(m_opening.get());
    if (res == SU_ERROR_INVALID_INPUT) {
      throw std::logic_error("CW::Opening::~Opening: Opening reference in invalid.  The code may point to dereferenced pointer.");
    }
  }
}


bool Opening::operator!() const {
  return SUIsInvalid(*m_opening.get());
}


Opening& Opening::operator=(const Opening& other) {
  // Destroy this object if there are no other references
  if (!!(*this) && m_opening.use_count() < 2) {
    SU_RESULT res = SUOpeningRelease(m_opening.get());
    if (res == SU_ERROR_INVALID_INPUT) {
      throw std::logic_error("CW::Opening::operator=(): Opening reference is invalid.  The code may point to dereferenced pointer.");
    }
  }
  m_opening = other.m_opening;
  return (*this);
}


/*******************
** Public Methods **
********************/

size_t Opening::get_num_points() const {
  if (!(*this)) {
    throw std::logic_error("CW::Opening::get_num_points(): Opening is invalid");
  }
  size_t count = 0;
  SU_RESULT res = SUOpeningGetNumPoints(*m_opening.get(), &count);
  if (res == SU_ERROR_NULL_POINTER_OUTPUT) {
    throw std::logic_error("CW::Opening::get_num_points(): points or count is NULL");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}
  

std::vector<Point3D> Opening::get_points() const {
  if (!(*this)) {
    throw std::logic_error("CW::Opening::get_points(): Opening is invalid");
  }
  size_t count = this->get_num_points();
  std::vector<SUPoint3D> points_array(count, SU_INVALID);
  size_t count_returned = 0;
  SU_RESULT res = SUOpeningGetPoints(*m_opening.get(), count, points_array.data(), &count_returned);
  if (res == SU_ERROR_NULL_POINTER_OUTPUT) {
    throw std::logic_error("CW::Opening::get_points(): points or count is NULL");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  assert(count_returned == count);
  std::vector<Point3D> points;
  points.reserve(count_returned);
  for (SUPoint3D p : points_array) {
    points.push_back(p);
  }
  return points;
}

} // end namespace CW
