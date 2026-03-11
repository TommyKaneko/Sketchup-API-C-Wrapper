//
//  Text.hpp
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

#ifndef Text_hpp
#define Text_hpp

#include <SketchUpAPI/model/text.h>
#include <SketchUpAPI/model/arrow_type.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Color.hpp"
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

namespace CW {

// Forward declarations
class String;
class Font;
class InstancePath;

/**
 * @brief C++ wrapper for SUTextRef.
 *
 * A Text object represents a text annotation (screen text or leader text)
 * placed in a SketchUp model. Text entities are drawing elements and can
 * be attached to points in space or to specific entities via an instance path.
 *
 * @see SUTextRef
 * @since SketchUp 2018, API 6.0
 */
class Text :public DrawingElement {
  private:
  static SUTextRef create_text();
  static SUTextRef copy_reference(const Text& other);

  public:
  /**
  * @brief Constructs a NULL Text object.
  */
  Text();

  /**
  * @brief Constructs a Text from an existing SUTextRef.
  * @since SketchUp 2018, API 6.0
  * @param text - SUTextRef object.
  * @param attached - flag indicating whether the text is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  Text(SUTextRef text, bool attached = true);

  /** @brief Copy Constructor */
  Text(const Text& other);

  /** @brief Destructor */
  ~Text();

  /** @brief Copy assignment operator */
  Text& operator=(const Text& other);

  /**
  * @brief Returns the SUTextRef object.
  */
  SUTextRef ref() const;
  operator SUTextRef() const;
  operator SUTextRef*();

  /**
  * @brief Returns true if this is an invalid/null text.
  */
  bool operator!() const;

  /**
  * @brief Sets the text string content. Assumed to be UTF-8 encoded.
  * @since SketchUp 2018, API 6.0
  * @param text_string - the text content to set.
  * @throws std::logic_error if the text is null.
  */
  void string(const String& text_string);
  void string(const std::string& text_string);

  /**
  * @brief Retrieves the text string content.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the text is null.
  */
  String string() const;

  /**
  * @brief Sets the font of the text object.
  * @since SketchUp 2018, API 6.0
  * @param font - the Font to set.
  * @throws std::logic_error if the text is null.
  * @throws std::invalid_argument if the font is invalid.
  */
  void font(const Font& font);

  /**
  * @brief Retrieves the font of the text object.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the text is null.
  */
  Font font() const;

  /**
  * @brief Sets the leader type of the text object.
  * @since SketchUp 2018, API 6.0
  * @param leader - the leader type to set. Valid values are:
  *   SUTextLeaderType_None, SUTextLeaderType_ViewBased,
  *   SUTextLeaderType_PushPin.
  * @throws std::logic_error if the text is null.
  * @throws std::out_of_range if the leader type is not valid.
  */
  void leader_type(SUTextLeaderType leader);

  /**
  * @brief Retrieves the leader type of the text object.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the text is null.
  */
  SUTextLeaderType leader_type() const;

  /**
  * @brief Sets the arrow type on the text leader.
  * @since SketchUp 2018, API 6.0
  * @param arrow - the arrow type to set. Valid values are:
  *   SUArrowNone, SUArrowSlash, SUArrowDot, SUArrowClosed, SUArrowOpen.
  * @throws std::logic_error if the text is null.
  * @throws std::out_of_range if the arrow type is not valid.
  */
  void arrow_type(SUArrowType arrow);

  /**
  * @brief Retrieves the arrow type of the text leader.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the text is null.
  */
  SUArrowType arrow_type() const;

  /**
  * @brief Sets the connection point of the text to an arbitrary position
  *   in model space. This is the simple form that connects to a point
  *   without an entity reference.
  * @since SketchUp 2018, API 6.0
  * @param point - the 3D point in model space.
  * @throws std::logic_error if the text is null.
  */
  void point(const Point3D& point);

  /**
  * @brief Sets the connection point of the text to a position on an entity.
  * @since SketchUp 2018, API 6.0
  * @param point - the 3D point in model space (may be ignored for vertex/
  *   guide point leaves).
  * @param path - the instance path to the entity.
  * @throws std::logic_error if the text is null.
  * @throws std::invalid_argument if the path refers to an invalid instance path.
  */
  void point(const Point3D& point, const InstancePath& path);

  /**
  * @brief Retrieves the connection point of the text and optionally the
  *   associated instance path.
  * @since SketchUp 2018, API 6.0
  * @return the connection point in model space.
  * @throws std::logic_error if the text is null.
  */
  Point3D point() const;

  /**
  * @brief Sets the leader vector of the text object (the direction from
  *   the connection point to the text).
  * @since SketchUp 2018, API 6.0
  * @param vector - the leader vector to set.
  * @throws std::logic_error if the text is null.
  */
  void leader_vector(const Vector3D& vector);

  /**
  * @brief Retrieves the leader vector of the text object.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the text is null.
  */
  Vector3D leader_vector() const;

  /**
  * @brief Sets the color of the text.
  * @since SketchUp 2018, API 6.0
  * @param color - the color to set.
  * @throws std::logic_error if the text is null.
  */
  void color(const Color& color);

  /**
  * @brief Retrieves the color of the text.
  * @since SketchUp 2018, API 6.0
  * @throws std::logic_error if the text is null.
  */
  Color color() const;

  /**
  * @brief Sets the screen position for text with no leader. The position
  *   is specified as a fraction of the screen dimensions.
  * @since SketchUp 2019, API 7.0
  * @param percent_x - x position from 0.0 to 1.0 (fraction of screen width).
  * @param percent_y - y position from 0.0 to 1.0 (fraction of screen height).
  * @throws std::logic_error if the text is null.
  * @throws std::out_of_range if a leader exists, or if percentages are not
  *   between 0 and 1.
  */
  void screen_position(double percent_x, double percent_y);

  /**
  * @brief Retrieves the screen position for text with no leader.
  * @since SketchUp 2019, API 7.0
  * @return a pair containing (percent_x, percent_y) as fractions of screen
  *   width and height.
  * @throws std::logic_error if the text is null or has a leader.
  */
  std::pair<double, double> screen_position() const;

  /**
  * @brief Creates a copy of this text object, duplicating all properties.
  * @return a new unattached Text object with the same properties.
  * @throws std::logic_error if this text is null.
  */
  Text copy() const;
};

} /* namespace CW */

#endif /* Text_hpp */
