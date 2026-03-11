//
//  Styles.hpp
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

#ifndef Styles_hpp
#define Styles_hpp

#include <string>
#include <vector>
#include <SketchUpAPI/model/styles.h>

namespace CW {

// Forward declarations
class Style;
class Scene;

/**
 * @brief C++ wrapper for SUStylesRef.
 *
 * A manager of the styles collection in a SketchUp model.
 * This object is obtained from the Model and cannot be created or released
 * independently.
 *
 * @see SUStylesRef
 * @since SketchUp 2017, API 5.0
 */
class Styles {
  private:
  SUStylesRef m_styles;

  public:
  /**
  * @brief Constructs a NULL Styles manager.
  */
  Styles();

  /**
  * @brief Constructs a Styles wrapper from an existing SUStylesRef.
  * @param styles - the SUStylesRef to wrap.
  */
  Styles(SUStylesRef styles);

  /**
  * @brief Returns the underlying SUStylesRef.
  */
  SUStylesRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null styles manager.
  */
  bool operator!() const;

  /**
  * @brief Adds a new style from a file. Optionally activates it.
  * @since SketchUp 2017, API 5.0
  * @param path - file path to the .style file.
  * @param activate - whether to activate the new style.
  * @throws std::logic_error if the Styles is null.
  */
  void add_style(const std::string& path, bool activate = false);

  /**
  * @brief Retrieves the number of styles.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the Styles is null.
  */
  size_t size() const;

  /**
  * @brief Retrieves all styles.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the Styles is null.
  */
  std::vector<Style> styles() const;

  /**
  * @brief Retrieves the active style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the Styles is null.
  */
  Style active_style() const;

  /**
  * @brief Retrieves the selected style.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the Styles is null.
  */
  Style selected_style() const;

  /**
  * @brief Sets the selected style.
  * @since SketchUp 2019.2, API 7.1
  * @param style - the style to select.
  * @throws std::logic_error if the Styles is null.
  */
  void selected_style(const Style& style);

  /**
  * @brief Retrieves a style by its GUID string.
  * @since SketchUp 2017, API 5.0
  * @param guid - the GUID string.
  * @throws std::logic_error if the Styles is null.
  */
  Style style_by_guid(const std::string& guid) const;

  /**
  * @brief Retrieves a style by its file path.
  * @since SketchUp 2017, API 5.0
  * @param path - the file path.
  * @throws std::logic_error if the Styles is null.
  */
  Style style_by_path(const std::string& path) const;

  /**
  * @brief Returns whether the active style has been changed.
  * @since SketchUp 2017, API 5.0
  * @throws std::logic_error if the Styles is null.
  */
  bool active_style_changed() const;

  /**
  * @brief Applies a style to a scene.
  * @since SketchUp 2017, API 5.0
  * @param style - the style to apply.
  * @param scene - the scene to apply the style to.
  * @throws std::logic_error if the Styles is null.
  */
  void apply_style_to_scene(const Style& style, const Scene& scene);

  /**
  * @brief Removes a style. The style will be released.
  * @since SketchUp 2019.2, API 7.1
  * @param style - the style to remove. Will be invalidated.
  * @throws std::logic_error if the Styles is null.
  */
  void remove_style(Style& style);
};

} /* namespace CW */

#endif /* Styles_hpp */
