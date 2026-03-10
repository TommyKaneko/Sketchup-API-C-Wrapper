//
//  Entity.hpp
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

#ifndef Entity_hpp
#define Entity_hpp

#include <string>
#include <vector>
#include <cmath>

#include <SketchUpAPI/model/entity.h>

#include "SUAPI-CppWrapper/model/TypedValue.hpp"

namespace CW {

// Forward Declarations
class AttributeDictionary;
class Model;
class Entities;

/**
 * @brief Entity object wrapper.
 *
 * This is the base class for all SketchUp entities. Entities are basically
 * anything that can be contained in a model, including DrawingElements such
 * as Edges, SectionPlanes, Groups, etc. and entities that relate to those
 * DrawingElements, such as Loops, Layers, etc.
 *
 * The methods below are available on all subclasses. For example, an Edge's
 * parent class is DrawingElement, and a DrawingElement's parent class is
 * Entity. Therefore an Edge has all of the methods defined in DrawingElement
 * and Entity.
 */
class Entity {

  friend class RubyAPI;
  friend class InstancePath;

  protected:
  /**
   * @brief The C SUEntityRef that this class wraps.
   */
  SUEntityRef m_entity;

  /**
   * @brief Indicates whether the Entity has been attached to a model.
   */
  bool m_attached;

  /**
   * @brief Constructor representing a null object.
   */
  Entity();

  /**
   * @brief Creates a new Entity object.
   * @param entity SUEntityRef from the SketchUp C API.
   * @param attached Flag indicating whether the entity is attached to a
   *                 model. Defaults to true.
   */
  Entity(SUEntityRef entity, bool attached = true);

  /**
   * @brief Copy constructor with an optional parameter for the entity
   *        reference.
   *
   * SUEntityRef objects cannot be created from this class, so the Ref object
   * must be passed to this constructor from a derived class object.
   * @param other Entity object from which properties will be copied.
   * @param entity_ref SUEntityRef object to assign to the copied object.
   */
  Entity(const Entity& other, SUEntityRef entity_ref = SU_INVALID);

  /**
   * @brief Destructor.
   *
   * The C++ wrapper deals with releasing objects, so the user does not have
   * to keep track of memory allocations.
   */
  virtual ~Entity();

  /**
   * @brief Copy assignment operator.
   */
  Entity& operator=(const Entity& other);

  /**
   * @brief Copies properties of the entity from another entity.
   * @param entity The source entity to copy properties from.
   * @return true on success, false on failure.
   */
  bool copy_properties_from(const Entity& entity);

  public:
  /**
   * @brief Returns a copy of the wrapped SUEntityRef.
   * @note The Entity object still manages the lifetime of the reference, so
   *       the SUEntityRef could become invalid.
   */
  operator SUEntityRef() const;

  /**
   * @brief Returns a pointer to the wrapped SUEntityRef.
   * @note The Entity object still manages the lifetime of the reference, so
   *       the SUEntityRef could become invalid.
   */
  operator SUEntityRef*();

  /**
   * @brief Returns the wrapped SUEntityRef.
   * @note The Entity object still manages the lifetime of the reference, so
   *       the SUEntityRef could become invalid.
   */
  SUEntityRef ref() const;

  /**
   * @brief Returns true if the entity is attached to another object.
   */
  bool attached() const;

  /**
   * @brief Sets the attached flag for the entity.
   * @param attach true to indicate the entity has been attached to a model,
   *               false to indicate it has not.
   */
  void attached(bool attach);

  /**
   * @brief Retrieves the attribute dictionaries attached to the entity.
   * @return vector of AttributeDictionary objects associated with the entity.
   *         If no dictionaries are associated, an empty vector is returned.
   */
  std::vector<AttributeDictionary> attribute_dictionaries() const;

  /**
   * @brief Retrieves an attribute dictionary with the given name. If a
   *        dictionary with the given name does not exist, one is added to
   *        the entity.
   * @param name The name of the AttributeDictionary.
   * @return AttributeDictionary object with the given name.
   */
  AttributeDictionary attribute_dictionary(const std::string& name) const;

  /**
   * @brief Adds an attribute dictionary to the entity. The given dictionary
   *        must not belong to another entity.
   * @param dict The AttributeDictionary to add.
   * @throws std::invalid_argument if the dictionary's name is empty or
   *         reserved for internal use.
   * @throws std::logic_error if a dictionary with the same name already
   *         exists on the entity.
   */
  void add_dictionary(AttributeDictionary& dict);

  /**
   * @brief Copies attributes from another Entity object to this one.
   * @param entity Object to copy the attributes from.
   * @return true on success, false on failure.
   */
  bool copy_attributes_from(const Entity& entity);

  /**
   * @brief Checks if the entity is valid.
   * @note This function does not check if the entity has been deleted.
   * @see operator!()
   */
  virtual bool is_valid() const;

  /**
   * @brief Returns true if this entity is not valid. Alias of !is_valid().
   */
  virtual bool operator!() const;

  /**
   * @brief Retrieves the unique ID assigned to an entity.
   * @return The entity ID.
   */
  int32_t entityID() const;

  /**
   * @brief Retrieves the value of an attribute in the entity's attribute
   *        dictionary.
   * @param dict_name The name of the attribute dictionary.
   * @param key The attribute key.
   * @param default_value Default TypedValue to return if no attribute is
   *                      found. Defaults to an empty TypedValue.
   * @return The TypedValue of the attribute, or the default value if not
   *         found.
   */
  TypedValue get_attribute(const std::string& dict_name, const std::string& key, const TypedValue& default_value = TypedValue()) const;

  /**
   * @brief Retrieves the value of an attribute in the given attribute
   *        dictionary.
   * @param dict AttributeDictionary object to search.
   * @param key The attribute key.
   * @param default_value Default TypedValue to return if no attribute is
   *                      found. Defaults to an empty TypedValue.
   * @return The TypedValue of the attribute, or the default value if not
   *         found.
   */
  TypedValue get_attribute(const AttributeDictionary &dict, const std::string& key, const TypedValue& default_value = TypedValue()) const;

  /**
   * @brief Sets the value of an attribute in the given attribute dictionary.
   * @param dict_name The name of the AttributeDictionary.
   * @param key The attribute key.
   * @param value The TypedValue to set.
   * @return true on success, false on failure.
   */
  void set_attribute(const std::string& dict_name, const std::string& key, const TypedValue& value);

  /**
   * @brief Sets the value of an attribute in the given attribute dictionary.
   * @param dict The AttributeDictionary object.
   * @param key The attribute key.
   * @param value The TypedValue to set.
   * @return true on success, false on failure.
   */
  void set_attribute(AttributeDictionary& dict, const std::string& key, const TypedValue& value);

  /**
   * @brief Returns the concrete type of the entity.
   * @return The SURefType of the entity.
   */
  enum SURefType entity_type() const;

  /**
   * @brief Retrieves the model object associated with the entity.
   * @throws std::logic_error if the entity is not associated with a model.
   * @return The Model object that holds this entity.
   */
  Model model() const;

  /**
   * @brief Retrieves the parent Entities object that contains the entity.
   * @throws std::logic_error if the entity is not contained by an Entities
   *         object.
   * @return The parent Entities object.
   */
  Entities parent() const;

  /**
   * @brief Retrieves the persistent ID of the entity.
   * @note Only a subset of entity types support persistent IDs.
   * @return The persistent ID.
   */
  int64_t persistent_id() const;

  /**
   * @brief Equality operator for two entity objects.
   */
  friend bool operator==(const Entity& lhs, const Entity& rhs);

  /**
   * @brief Non-equality operator for two entity objects.
   */
  friend bool operator!=(const Entity& lhs, const Entity& rhs);

  /**
   * @brief Hash function for use with unordered_map.
   */
  friend std::hash<CW::Entity>;
};

} /* namespace CW */

namespace std {
  template <> struct hash<CW::Entity>
  {
    size_t operator()(const CW::Entity& k) const
    {
      static const size_t shift = static_cast<size_t>(log2(1 + sizeof(CW::Entity)));
      return reinterpret_cast<size_t>(k.m_entity.ptr) >> shift;
    }
  };

}

#endif /* Entity_hpp */
