//
//  ComponentInstance.hpp
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

#ifndef ComponentInstance_hpp
#define ComponentInstance_hpp

#include <SketchUpAPI/model/component_instance.h>

#include "SUAPI-CppWrapper/Transformation.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"

namespace CW {

// Forward declarations
class ClassificationInfo;
class DynamicComponentInfo;

/**
 * @brief Wrapper for SUComponentInstanceRef.
 *
 * Represents an instance of a component definition placed in the model.
 * Each instance has its own transformation, name, and attribute dictionaries.
 *
 * @see SUComponentInstanceRef in the SketchUp C API
 */
class ComponentInstance :public DrawingElement {
  friend class Group;
  protected:
  /**
   * @brief Creates a copy of the underlying SUComponentInstanceRef.
   * @param other The ComponentInstance to copy.
   * @return The copied SUComponentInstanceRef.
   */
  SUComponentInstanceRef copy_reference(const ComponentInstance& other);

  public:
  /**
   * @brief Constructs a null ComponentInstance.
   */
  ComponentInstance();

  /**
   * @brief Constructs a ComponentInstance from an existing SUComponentInstanceRef.
   * @param instance The native instance reference.
   * @param attached Whether the instance is already owned by a model.
   */
  ComponentInstance(SUComponentInstanceRef instance, bool attached = true);

  /**
   * @brief Copy constructor.
   * @param other The ComponentInstance to copy.
   */
  ComponentInstance(const ComponentInstance& other);

  /**
   * @brief Copy constructor for derived classes (Group).
   * @param other        The source ComponentInstance.
   * @param instance_ref The native reference for the derived copy.
   */
  ComponentInstance(const ComponentInstance& other, SUComponentInstanceRef instance_ref);

  /**
   * @brief Destructor. Releases the instance if it is not attached to a model.
   */
  ~ComponentInstance();

  /**
   * @brief Copy assignment operator.
   * @param other The ComponentInstance to assign from.
   * @return Reference to this object.
   */
  ComponentInstance& operator=(const ComponentInstance& other);

  /**
   * @brief Returns the wrapped SUComponentInstanceRef.
   * @return The raw SUComponentInstanceRef.
   */
  SUComponentInstanceRef ref() const;

  /** @brief Implicit conversion to SUComponentInstanceRef. */
  operator SUComponentInstanceRef() const;

  /** @brief Implicit conversion to SUComponentInstanceRef*. */
  operator SUComponentInstanceRef*();

  /**
   * @brief Retrieves the transformation applied to this instance.
   *
   * The transform is relative to the parent component. If the parent is
   * the root component of a model, it is relative to absolute coordinates.
   *
   * @return The Transformation applied to this instance.
   * @throws std::logic_error If the instance is null.
   * @see SUComponentInstanceGetTransform
   */
  Transformation transformation() const;

  /**
   * @brief Sets the transformation applied to this instance.
   * @param tranform The Transformation to apply.
   * @throws std::logic_error If the instance is null.
   * @see SUComponentInstanceSetTransform
   */
  void transformation(const Transformation& tranform);

  /**
   * @brief Retrieves the component definition of this instance.
   * @return The ComponentDefinition.
   * @throws std::logic_error If the instance is null.
   * @see SUComponentInstanceGetDefinition
   */
  ComponentDefinition definition() const;

  /**
   * @brief Retrieves the name of this instance.
   * @return The name as a String.
   * @throws std::logic_error If the instance is null.
   * @see SUComponentInstanceGetName
   */
  String name() const;

  /**
   * @brief Sets the name of this instance.
   * @param string The name to set (UTF-8 encoded).
   * @throws std::logic_error If the instance is null.
   * @see SUComponentInstanceSetName
   */
  void name(const String& string);

  /**
   * @brief Creates a DynamicComponentInfo object for this instance.
   *
   * The caller takes ownership of the returned object.
   *
   * @return DynamicComponentInfo object (null if the instance is not a dynamic component).
   * @throws std::logic_error If the instance is null.
   * @since SketchUp 2016, API 4.0
   * @see SUComponentInstanceCreateDCInfo
   */
  DynamicComponentInfo create_dc_info() const;

  /**
   * @brief Creates a ClassificationInfo object for this instance.
   *
   * The caller takes ownership of the returned object.
   *
   * @return ClassificationInfo object (null if the instance is not classified).
   * @throws std::logic_error If the instance is null.
   * @since SketchUp 2017, API 5.0
   * @see SUComponentInstanceCreateClassificationInfo
   */
  ClassificationInfo create_classification_info() const;

};

} /* namespace CW */
#endif /* ComponentInstance_hpp */
