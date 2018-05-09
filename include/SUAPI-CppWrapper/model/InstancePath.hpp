//
//  InstancePath.hpp
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

#ifndef InstancePath_hpp
#define InstancePath_hpp

#include <stdio.h>

#include <SketchUpAPI/model/instancepath.h>

namespace CW {

// Forward declarations
class ComponentInstance;
class Transformation;
class Entity;
class DrawingElement;
class String;


/*
* InstancePath wrapper
*/
class InstancePath {
  private:
  SUInstancePathRef m_instance_path;
  
  static SUInstancePathRef create_instance_path();

  /**
  * Creates a SUInstancePathRef derived from an existing InstancePath object.
  * @param other - InstancePath object to derive the new SUInstancePathRef object from
  * @return
  */
  static SUInstancePathRef copy_reference(const InstancePath& other);
  
  public:
  /** Constructor for empty InstancePath value */
  InstancePath();

  InstancePath(SUInstancePathRef instance_path);

  /** Copy constructor */
  InstancePath(const InstancePath& other);
  
  /** Destructor */
  ~InstancePath();

  /** Copy assignment operator */
  InstancePath& operator=(const InstancePath& other);
  
  /*
  * The class object can be converted to a SUInstancePathRef without loss of data.
  */
  SUInstancePathRef ref() const;
  operator SUInstancePathRef() const;
  operator SUInstancePathRef*();

  /**
  * Pushes a ComponentInstance to an InstancePath.
  * @param instance The component instance object.
  * @return reference to this InstancePath
  */
  InstancePath& push(const ComponentInstance& instance);

  /**
  * Pops the last ComponentInstance from an InstancePath.
  * @return reference to this InstancePath
  */
  InstancePath& pop();

  /**
  * Sets an Entity to an InstancePath.
  * @param entity - The entity to be set as a leaf in instance path.
  * @return reference to this InstancePath
  */
  InstancePath& set_leaf(const Entity& entity);

  /**
  * Gets a path depth for InstancePath. It only counts the component instances in the path, so the leaf node is not counted.
  * @return The depth of instance path object.
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetPathDepth is not const
  */
  size_t depth() const;

  /**
  * Gets the full path depth (including the leaf) for InstancePath.
  * @return The full depth of instance path object.
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetFullDepth is not const
  */
  size_t full_depth() const;
  
  /**
  * Gets the transform for InstancePath.
  * @return The transform from instance path
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetTransform is not const
  */
  Transformation total_transformation() const;

  /**
  * Gets the transform up to depth level for InstancePath.
  * @param depth - The depth for getting transforms up to.
  * @return The transform from instance path.
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetTransformAtDepth is not const
  */
  Transformation transformation_at_depth(size_t depth) const;

  /**
  * Gets a component instance up to path depth level.
  * @param depth - The depth for getting drawing element up to.
  * @return The component instance on the instance path at that depth.
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetInstanceAtDepth is not const
  */
  ComponentInstance instance_at_depth(size_t depth) const;


  /**
  * Gets a leaf from an instance path as an entity object.
  * @return The leaf entity from an instance path.
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetLeafAsEntity is not const
  */
  Entity leaf_entity() const;

  /**
  * Gets a leaf from an entity path as a drawing element object.
  * @return The leaf DrawingElement from an instance path.
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetLeaf is not const
  */
  DrawingElement leaf() const;

  /**
  * Validates an instance path.
  * @return Whether the instance path is valid or not.
  * TODO: This method should be const, but the C API has an error where SUInstancePathIsValid is not const
  */
  bool valid() const;
  
  /**
  * Checks if an instance path is empty.
  * @return Whether the instance path is empty or not.
  * TODO: This method should be const, but the C API has an error where SUInstancePathIsEmpty is not const
  */
  bool empty() const;

  /**
  * Checks if instance path contains a particular entity.
  @param entity - The entity object.
  @return Whether the instance path contains the entity or not.
  * TODO: This method should be const, but the C API has an error where SUInstancePathContains is not const
  */
  bool contains(const Entity& entity) const;
  
  /**
  * Retrieves the full persistent id for a given instance path.
  * @return A String of the persistent id (or its full depth?).
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetPersistentID is not const
  */
  String persistent_id() const;

  /**
  * Retrieves the persistent id of an entity up to depth level in a given instance path.
  * @param depth - The depth for getting persistent id up to.
  * @return The persistent id.
  * TODO: This method should be const, but the C API has an error where SUInstancePathGetPersistentIDAtDepth is not const
  */
  String persistent_id_at_depth(size_t depth) const;

};

} /* namespace CW */

#endif /* InstancePath_hpp */
