//
//  ComponentDefinition.hpp
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

#ifndef ComponentDefinition_hpp
#define ComponentDefinition_hpp

#include <SketchUpAPI/model/component_definition.h>

#include "SUAPI-CppWrapper/model/DrawingElement.hpp"

namespace CW {

// Forward declarations:
class Entities;
class ComponentInstance;
class Group;
class Behavior;
class String;
class Model;
class Opening;

/**
 * @brief Wrapper for SUComponentDefinitionRef.
 *
 * A component definition specifies the geometry and metadata for a reusable
 * component. Instances of a definition share the same geometry but each have
 * their own transformation, name, and attribute dictionaries.
 *
 * @see SUComponentDefinitionRef in the SketchUp C API
 */
class ComponentDefinition :public DrawingElement {
  private:

  static SUComponentDefinitionRef create_definition();

  /**
   * @brief Copies a ComponentDefinition reference.
   *
   * If the other ComponentDefinition has been attached to a model, returns
   * the same SUComponentDefinitionRef. Otherwise copies geometry into a new
   * definition.
   *
   * @param other The ComponentDefinition to copy.
   * @return The copied or shared SUComponentDefinitionRef.
   */
  static SUComponentDefinitionRef copy_reference(const ComponentDefinition& other);

  public:
  /**
   * @brief Constructs an empty ComponentDefinition, ready to add to a model.
   */
  ComponentDefinition();

  /**
   * @brief Constructs a ComponentDefinition from an existing SUComponentDefinitionRef.
   * @param definition The SUComponentDefinitionRef to wrap.
   * @param attached   Whether the definition is already owned by a model.
   */
  ComponentDefinition(SUComponentDefinitionRef definition, bool attached = true);

  /**
   * @brief Copy constructor.
   * @param other The ComponentDefinition to copy.
   */
  ComponentDefinition(const ComponentDefinition& other);

  /**
   * @brief Destructor. Releases the definition if it has not been added to a model.
   */
  ~ComponentDefinition();

  /**
   * @brief Copy assignment operator.
   * @param other The ComponentDefinition to assign from.
   * @return Reference to this object.
   */
  ComponentDefinition& operator=(const ComponentDefinition& other);

  /**
   * @brief Returns the wrapped SUComponentDefinitionRef.
   * @return The raw SUComponentDefinitionRef.
   */
  SUComponentDefinitionRef ref() const;

  /** @brief Implicit conversion to SUComponentDefinitionRef. */
  operator SUComponentDefinitionRef() const;

  /** @brief Implicit conversion to SUComponentDefinitionRef*. */
  operator SUComponentDefinitionRef*();

  /**
   * @brief Creates a new ComponentInstance from this definition.
   * @return A new unattached ComponentInstance.
   * @throws std::logic_error If the definition is null.
   */
  ComponentInstance create_instance() const;

  /**
   * @brief Retrieves the entities in the definition.
   * @return The Entities container for this definition.
   * @throws std::logic_error If the definition is null.
   */
  Entities entities() const;

  /**
   * @brief Retrieves the name of the component definition.
   * @return The name as a String.
   * @throws std::logic_error If the definition is null.
   * @see SUComponentDefinitionGetName
   */
  String name() const;

  /**
   * @brief Sets the name of the component definition.
   *
   * If the requested name already belongs to another definition in the model,
   * a unique name will be generated automatically.
   *
   * @param name The name to set (UTF-8 encoded).
   * @throws std::logic_error If the definition is null.
   * @see SUComponentDefinitionSetName
   */
  void name(String name);

  /**
   * @brief Returns whether this definition is of a group.
   * @return True if the definition type is SUComponentType_Group.
   * @throws std::logic_error If the definition is null.
   * @see SUComponentDefinitionGetType
   */
  bool is_group() const;

  /**
   * @brief Retrieves the behavior of this component definition.
   * @return The Behavior object.
   * @throws std::logic_error If the definition is null.
   * @see SUComponentDefinitionGetBehavior
   */
  Behavior behavior() const;

  /**
   * @brief Sets the component behavior of this definition.
   * @param behavior The Behavior to set.
   * @throws std::logic_error If the definition is null.
   * @see SUComponentDefinitionSetBehavior
   */
  void behavior(const Behavior& behavior) const;

  /**
   * @brief Retrieves the total number of instances of this definition.
   *
   * Takes into account the full hierarchy of the model. The count is
   * influenced by adding/removing instances of other definitions which
   * contain an instance of this definition.
   *
   * @return The total instance count across the hierarchy.
   * @since SketchUp 2017, API 5.0
   * @see SUComponentDefinitionGetNumUsedInstances
   */
  size_t num_used_instances() const;

  /**
   * @brief Retrieves the number of unique instances of this definition.
   *
   * The returned count represents the number of instances of this
   * definition in the model's root plus the number of instances contained
   * in other definitions.
   *
   * @return The unique instance count.
   * @since SketchUp 2017, API 5.0
   * @see SUComponentDefinitionGetNumInstances
   */
  size_t num_instances() const;

  /**
   * @brief Retrieves the instances of this definition.
   * @return A vector of ComponentInstance objects.
   * @throws std::logic_error If the definition is null.
   * @since SketchUp 2017, API 5.0
   * @see SUComponentDefinitionGetInstances
   */
  std::vector<ComponentInstance> instances() const;

  /**
   * @brief Retrieves the number of openings from this definition.
   * @return The number of openings.
   * @throws std::logic_error If the definition is null.
   * @since SketchUp 2016, API 4.0
   * @see SUComponentDefinitionGetNumOpenings
   */
  size_t num_openings() const;

  /**
   * @brief Retrieves the openings from this definition.
   * @return A vector of Opening objects.
   * @throws std::logic_error If the definition is null.
   * @since SketchUp 2016, API 4.0
   * @see SUComponentDefinitionGetOpenings
   */
  std::vector<Opening> openings() const;

  /** @brief Hash function for use with unordered_map. */
  friend std::hash<CW::ComponentDefinition>;

};


/**
 * @brief Wrapper for SUComponentBehavior.
 *
 * Describes how a component behaves in a SketchUp model, such as how it
 * glues to surfaces, whether it always faces the camera, etc.
 *
 * @see SUComponentBehavior
 */
class Behavior {
private:
  SUComponentBehavior m_behavior;

public:
  /**
   * @brief Constructs a Behavior from an SUComponentBehavior struct.
   * @param behavior The raw SUComponentBehavior.
   */
  Behavior(SUComponentBehavior behavior);

  /** @brief How the component should snap to the surface where it's placed. */
  SUComponentBehavior::SUSnapToBehavior &snap = m_behavior.component_snap;

  /**
   * @brief Returns the wrapped SUComponentBehavior struct.
   * @return The raw SUComponentBehavior.
   */
  SUComponentBehavior ref() const;

  /** @brief Whether the component creates an opening when placed on a surface. */
  bool &cuts_opening = m_behavior.component_cuts_opening;

  /** @brief Whether the component visually revolves to face the camera. */
  bool &always_face_camera = m_behavior.component_always_face_camera;

  /** @brief Whether the component always casts a shadow as if facing the sun. */
  bool &shadows_face_sun = m_behavior.component_shadows_face_sun;

  /** @brief Bitmask indicating which scale tool handles are hidden. */
  size_t &no_scale_mask = m_behavior.component_no_scale_mask;
};

} /* namespace CW */


namespace std {
  template <> struct hash<CW::ComponentDefinition>
  {
    size_t operator()(const CW::ComponentDefinition& k) const
    {
      static const size_t shift = static_cast<size_t>(log2(1 + sizeof(CW::ComponentDefinition)));
      return reinterpret_cast<size_t>(k.m_entity.ptr) >> shift;
    }
  };

}

#endif /* ComponentDefinition_hpp */
