//
//  LineStyles.hpp
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

#ifndef LineStyles_hpp
#define LineStyles_hpp

#include <string>
#include <vector>
#include <SketchUpAPI/model/line_styles.h>

namespace CW {

// Forward declarations
class LineStyle;
class String;

/**
 * @brief C++ wrapper for SULineStylesRef.
 *
 * LineStyles is a manager object that provides access to the line styles
 * available in a model. It is obtained from Model and cannot be created
 * or released independently.
 *
 * @see SULineStylesRef
 * @since SketchUp 2019, API 7.0
 */
class LineStyles {
  private:
  SULineStylesRef m_line_styles;

  public:
  /**
  * @brief Constructs a NULL LineStyles object.
  */
  LineStyles();

  /**
  * @brief Constructs a LineStyles from an existing SULineStylesRef.
  * @param line_styles - the SULineStylesRef obtained from the model.
  */
  LineStyles(SULineStylesRef line_styles);

  /**
  * @brief Returns the SULineStylesRef object.
  */
  SULineStylesRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null line styles manager.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the number of line styles.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the line styles manager is null.
  */
  size_t size() const;

  /**
  * @brief Retrieves the names of all line styles.
  * @since SketchUp 2019, API 7.0
  * @throws std::logic_error if the line styles manager is null.
  */
  std::vector<String> names() const;

  /**
  * @brief Retrieves a line style by name.
  * @since SketchUp 2019, API 7.0
  * @param name - the name of the line style to find.
  * @return LineStyle object (may be null if not found).
  * @throws std::logic_error if the line styles manager is null.
  */
  LineStyle line_style_by_name(const std::string& name) const;
};

} /* namespace CW */

#endif /* LineStyles_hpp */
