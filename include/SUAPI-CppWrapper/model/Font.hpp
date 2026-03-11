//
//  Font.hpp
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

#ifndef Font_hpp
#define Font_hpp

#include <SketchUpAPI/model/font.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class String;

/**
 * @brief C++ wrapper for SUFontRef.
 *
 * A Font object represents a font used by text and dimension entities.
 * Font objects are read-only — they are obtained from Text or Dimension
 * entities and cannot be created or released directly.
 *
 * @see SUFontRef
 * @since SketchUp 2018, API 6.0
 */
class Font :public Entity {
  public:
  /**
  * @brief Constructs a NULL Font object.
  */
  Font();

  /**
  * @brief Constructs a Font from an existing SUFontRef.
  * @since SketchUp 2018, API 6.0
  * @param font - SUFontRef object.
  * @param attached - flag indicating whether the font is owned by
  *   a parent object (e.g. a Text or Dimension entity).
  */
  Font(SUFontRef font, bool attached = true);

  /**
  * @brief Returns the SUFontRef object.
  */
  SUFontRef ref() const;
  operator SUFontRef() const;
  operator SUFontRef*();

  /**
  * @brief Returns true if this is an invalid/null font.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the font face name (e.g. "Arial", "Helvetica").
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the font is null.
  */
  String face_name() const;

  /**
  * @brief Retrieves the font point size.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the font is null.
  */
  size_t point_size() const;

  /**
  * @brief Retrieves whether the font is bold.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the font is null.
  */
  bool bold() const;

  /**
  * @brief Retrieves whether the font is italic.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the font is null.
  */
  bool italic() const;

  /**
  * @brief Retrieves whether the font uses world size (true) or screen size
  *   (false).
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the font is null.
  */
  bool use_world_size() const;

  /**
  * @brief Retrieves the world size of the font in inches when use_world_size()
  *   is true.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the font is null.
  */
  double world_size() const;
};

} /* namespace CW */

#endif /* Font_hpp */
