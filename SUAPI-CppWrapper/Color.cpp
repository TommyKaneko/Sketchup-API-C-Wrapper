//  Color.cpp
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

#include "SUAPI-CppWrapper/Color.hpp"

namespace CW {


Color::Color():
  m_color(SUColor{}),
  red(m_color.red),
  green(m_color.green),
  blue(m_color.blue),
  alpha(m_color.alpha)
{}

Color::Color(SUColor color_input):
  m_color(color_input),
  red(m_color.red),
  green(m_color.green),
  blue(m_color.blue),
  alpha(m_color.alpha)
{}

SUColor Color::ref() const {
  return m_color;
}

Color::operator SUColor() const {
  return ref();
}

bool operator ==(const Color &lhs, const Color &rhs) {
  if (lhs.m_color.alpha == rhs.m_color.alpha &&
      lhs.m_color.red == rhs.m_color.red &&
      lhs.m_color.blue == rhs.m_color.blue &&
      lhs.m_color.green == rhs.m_color.green)  {
    return true;
  }
  return false;
}


} /* namespace CW */
