//
//  ComponentDefinition.hpp
//
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

#ifndef ComponentDefinition_hpp
#define ComponentDefinition_hpp

#include <stdio.h>

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
* This class represents a component definition.
*/
class ComponentDefinition :public DrawingElement {
  private:

  static SUComponentDefinitionRef create_definition();

  /**
  * Copies a ComponentDefinition object into a new SUComponentDefinitionRef.  If the other ComponentDefinition object has been attached to a model already, it will return the same SUComponentDefinitionRef object that it points to.
  */
  static SUComponentDefinitionRef copy_reference(const ComponentDefinition& other);

  public:
  /**
  * Constructor creates empty Component Definition, ready to add to a model.
  */
  ComponentDefinition();

  /**
  * Constructor creates Component Definition from existing SUComponentDefinitionRef.
  */
  ComponentDefinition(SUComponentDefinitionRef definition, bool attached = true);

  /**
  * Copy constructor.
  */
  ComponentDefinition(const ComponentDefinition& other);

  /**
  * Destructor will release the definition object if it had not been added to a model.
  */
  ~ComponentDefinition();

  /** Copy assignment operator */
  ComponentDefinition& operator=(const ComponentDefinition& other);

  /** Cast to native objects */
  /**
  * Returns the SUComponentDefinitionRef that this class wraps.
  */
  SUComponentDefinitionRef ref() const;
  operator SUComponentDefinitionRef() const;
  operator SUComponentDefinitionRef*();

  /**
  * Creates a new ComponentInstance from this ComponentDefinition.
  */
  ComponentInstance create_instance() const;

  /**
  * Creates a new Group from this ComponentDefinition. If the definition is not a Group definition, the method will fail. TODO: requires testing
  *
  * This method is not possible to implement with the current API (2017)
  */
  //Group create_group() const;

  /**
  * Gets the entities in the definition.
  */
  Entities entities() const;

  /**
  * Gets the name of the component.
  */
  String name() const;

  /**
  * Sets the name of the component.
  */
  bool name(String name);

  /**
  * Returns whether this definition is of a group.
  */
  bool is_group() const;

  /**
  * Returns the Behavior object of this component.
  */
  Behavior behavior() const;
  void behavior(const Behavior& behavior) const;

  /**
  * Retrieves the total number of instances of the provided definition. This method takes into account the full hierarchy of the model. Therefore, the count is influenced by adding/removing instances of other definitions which contain an instance of this definition. Users should not use this function to determine the count to be passed to SUComponentDefinitionGetInstances specifying the number of instances to be retrieved.
  */
  size_t num_used_instances() const;

  /**
  * Retrieves the number of unique instances of the provided definition. The returned count represents the number of instances of this definition in the model's root plus the number instances of this definition contained in other definitions.
  */
  size_t num_instances() const;

  /**
  * Returns the instances of this ComponentDefinition.
  */
  std::vector<ComponentInstance> instances() const;

  /**
  * Retrieves the number of openings from the component definition.
  * @since SketchUp 2016, API 4.0
  **/
  size_t num_openings() const;

  /**
  * Retrieves the openings from the component definition.
  * @since SketchUp 2016, API 4.0
  **/
  std::vector<Opening> openings() const;

  /**
  * Hash function for use with unordered_map
  */
  friend std::hash<CW::ComponentDefinition>;

};


class Behavior {
private:
  SUComponentBehavior m_behavior;

public:
  Behavior(SUComponentBehavior behavior);

  SUComponentBehavior::SUSnapToBehavior &snap = m_behavior.component_snap;

  SUComponentBehavior ref() const;

  bool &cuts_opening = m_behavior.component_cuts_opening;

  bool &always_face_camera = m_behavior.component_always_face_camera;

  bool &shadows_face_sun = m_behavior.component_shadows_face_sun;

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
