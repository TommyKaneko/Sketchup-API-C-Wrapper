//  Color.hpp
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

#ifndef Color_hpp
#define Color_hpp

#include <SketchUpAPI/color.h>

namespace CW {

/**
 * @brief Wrapper for SUColor, representing an RGBA color value.
 *
 * Provides direct access to red, green, blue, and alpha components
 * via public reference members, and conversion to/from SUColor.
 *
 * @see https://extensions.sketchup.com/developer_center/sketchup_c_api/
 */
class Color {
  private:
  SUColor m_color;

  public:
  SUByte& red;   ///< Red component (0-255).
  SUByte& green; ///< Green component (0-255).
  SUByte& blue;  ///< Blue component (0-255).
  SUByte& alpha; ///< Alpha component (0-255, where 255 is fully opaque).

  /** @brief Default constructor – creates a black, fully opaque color. */
  Color();

  /**
   * @brief Constructs a Color from a raw SUColor struct.
   * @param color_input the SUColor to wrap.
   */
  Color(SUColor color_input);

  /**
   * @brief Returns the underlying SUColor struct.
   * @return the raw SUColor value.
   */
  SUColor ref() const;

  /** @brief Conversion operator to SUColor. */
  operator SUColor() const;

  /**
   * @brief Compares two Color objects for equality.
   * @return true if all RGBA components are equal.
   */
  friend bool operator ==(const Color &lhs, const Color &rhs);

};

} /* namespace CW */

#endif /* Color_hpp */
