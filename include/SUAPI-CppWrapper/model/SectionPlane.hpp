//
//  SectionPlane.hpp
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

#ifndef SectionPlane_hpp
#define SectionPlane_hpp

#include <SketchUpAPI/model/section_plane.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

namespace CW {

// Forward declaration
class String;

class SectionPlane :public DrawingElement {
  private:
  static SUSectionPlaneRef create_section_plane();
  static SUSectionPlaneRef copy_reference(const SectionPlane& other);

  public:
  /**
  * @brief Constructs a NULL SectionPlane object.
  */
  SectionPlane();

    /**
  * @brief Constructs a SectionPlane object from a Plane3D object.
  * @param plane - the 3D plane to be set.
  */
  SectionPlane(const Plane3D& plane);

  /**
  * @brief Constructs a SectionPlane from an existing SUSectionPlaneRef.
  *   The plane is initialized as an XY plane and can be changed with
  *   the plane() setter.
  * @since SketchUp 2016, API 4.0
  * @param section_plane - SUSectionPlaneRef object.
  * @param attached - flag indicating whether the section plane is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  SectionPlane(SUSectionPlaneRef section_plane, bool attached = true);

  /** @brief Copy Constructor */
  SectionPlane(const SectionPlane& other);

  /** @brief Destructor */
  ~SectionPlane();

  /** @brief Copy assignment operator */
  SectionPlane& operator=(const SectionPlane& other);

  /**
  * @brief Returns the SUSectionPlaneRef object.
  */
  SUSectionPlaneRef ref() const;
  operator SUSectionPlaneRef() const;
  operator SUSectionPlaneRef*();

  /**
  * @brief Returns true if this is an invalid/null section plane.
  */
  bool operator!() const;

  /**
  * @brief Sets the plane of the section plane.
  * @since SketchUp 2016, API 4.0
  * @param plane - the 3D plane to be set.
  */
  void plane(const Plane3D& plane);

  /**
  * @brief Retrieves the plane of the section plane.
  * @since SketchUp 2016, API 4.0
  */
  Plane3D plane() const;

  /**
  * @brief Retrieves a boolean indicating whether or not the section plane
  *   is active.
  * @since SketchUp 2016, API 4.0
  */
  bool active() const;

  /**
  * @brief Sets the name of the section plane object. Assumed to be UTF-8
  *   encoded. An example of a name would be "South West Section" for the
  *   section on the south west side of a building.
  * @since SketchUp 2018, API 6.0
  * @param name - the name to be set.
  */
  void name(const String& name);
  void name(const std::string& name);

  /**
  * @brief Retrieves the name of the section plane object.
  * @since SketchUp 2018, API 6.0
  */
  String name() const;

  /**
  * @brief Sets the symbol of the section plane object. Assumed to be UTF-8
  *   encoded. The maximum number of characters is 3. For example, you might
  *   have several sections all named "South West Section" and use symbols to
  *   differentiate each section: "01", "02", "03".
  * @since SketchUp 2018, API 6.0
  * @param symbol - the symbol string to be set.
  */
  void symbol(const String& symbol);
  void symbol(const std::string& symbol);

  /**
  * @brief Retrieves the symbol of the section plane object.
  * @since SketchUp 2018, API 6.0
  */
  String symbol() const;

  /**
  * @brief Creates a copy of this section plane object, duplicating all properties.
  * @return a new unattached SectionPlane object with the same properties.
  * @throws std::logic_error if this section plane is null.
  */
  SectionPlane copy() const;
};

} /* namespace CW */

#endif /* SectionPlane_hpp */
