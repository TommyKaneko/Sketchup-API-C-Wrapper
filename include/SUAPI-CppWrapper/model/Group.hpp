//
//  Group.hpp
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

#ifndef Group_hpp
#define Group_hpp

#include <SketchUpAPI/model/group.h>

#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"

namespace CW {

// Forward Declarations
class Entities;
class Transformation;
class ComponentDefinition;
class String;

/**
 * @brief Wrapper for SUGroupRef.
 *
 * A group is a special type of component instance that is always unique.
 * Groups inherit from ComponentInstance and provide direct access to their
 * entities, name, and transformation.
 *
 * @note Groups must always be attached to a parent. There is no release
 *       function for groups in the C API.
 *
 * @see SUGroupRef in the SketchUp C API
 */
class Group :public ComponentInstance {
  private:
  /**
   * @brief Creates a new empty SUGroupRef.
   * @return The newly created SUGroupRef.
   */
  static SUGroupRef create_group();
  
  /**
   * @brief Copies a Group reference.
   * @param other The Group to copy.
   * @return The copied SUGroupRef.
   */
  static SUGroupRef copy_reference(const Group& other);

  public:
  /**
   * @brief Constructs a new, empty Group object.
   */
  Group();
  
  /**
   * @brief Constructs a Group from an existing SUGroupRef.
   * @param group    The native SUGroupRef to wrap.
   * @param attached Whether the group is already owned by a model.
   */
  Group(SUGroupRef group, bool attached = true);
  
  /**
   * @brief Constructs a Group from a ComponentInstance.
   *
   * The instance must be of type SURefType_Group. If it is not, an
   * assertion will fail.
   *
   * @param instance The ComponentInstance to convert.
   */
  Group(const ComponentInstance& instance);
  
  /**
   * @brief Copy constructor.
   * @param other The Group to copy.
   */
  Group(const Group& other);
  
  /**
   * @brief Destructor.
   */
  ~Group();

  /**
   * @brief Copy assignment operator.
   * @param other The Group to assign from.
   * @return Reference to this object.
   */
  Group& operator=(const Group& other);

  /**
   * @brief Returns the wrapped SUGroupRef.
   * @return The raw SUGroupRef.
   */
  SUGroupRef ref() const;

  /**
   * @brief Implicit conversion to SUGroupRef.
   */
  operator SUGroupRef() const;

  /** @brief Implicit conversion to SUGroupRef*. */
  operator SUGroupRef*();
  
  /**
   * @brief Retrieves the component definition of this group.
   * @return The ComponentDefinition.
   * @throws std::logic_error If the group is null.
   * @since SketchUp 2014, API 2.0
   * @see SUGroupGetDefinition
   */
  ComponentDefinition definition() const;
  
  /**
   * @brief Retrieves the entities of the group.
   * @return The Entities container.
   * @throws std::logic_error If the group is null.
   * @see SUGroupGetEntities
   */
  Entities entities() const;
  
  /**
   * @brief Retrieves the name of the group.
   * @return The name as a String.
   * @throws std::logic_error If the group is null.
   * @see SUGroupGetName
   */
  String name() const;
  
  /**
   * @brief Sets the name of this group.
   * @param string The name to set (UTF-8 encoded).
   * @throws std::logic_error If the group is null.
   * @see SUGroupSetName
   */
  void name(const String& string);
  
  /**
   * @brief Retrieves the transformation applied to this group.
   *
   * The transform is relative to the parent component. If the parent is
   * the root component of a model, it is relative to absolute coordinates.
   *
   * @return The Transformation.
   * @throws std::logic_error If the group is null.
   * @see SUGroupGetTransform
   */
  Transformation transformation() const;

  /**
   * @brief Sets the transformation of this group.
   * @param transform The Transformation to apply.
   * @throws std::logic_error If the group is null.
   * @see SUGroupSetTransform
   */
  void transformation(const Transformation& transform);
  
};

} /* namespace CW */

#endif /* Group_hpp */
