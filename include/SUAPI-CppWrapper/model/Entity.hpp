//
//  Entity.hpp
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

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>

#include <SketchUpAPI/model/entity.h>

#include "SUAPI-CppWrapper/model/TypedValue.hpp"

namespace CW {

// Forward Declarations
class AttributeDictionary;
class Model;
class Entities;

/*
* @brief Entity object wrapper.
*
* This is the base class for all SketchUp entities. Entities are basically anything that can be contained in a model, including Drawingelements such as Edges, SectionPlanes, Groups, etc. and entities that relate to those Drawingelements, such as Loops, Layers, etc.
*
* Keep in mind that the methods below are available on all subclasses. For example, an Edge's parent class is Drawingelement, and a Drawingelement's parent class is Entity. Therefore an Edge has all of the methods defined in Drawingelement and Entity.
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
  * @brief Constructor representing a null objject.
  */
  Entity();

  /**
  * @brief Creates a new Entity object.
  * @param entity - SUEntityRef from SU C API
  * @param attached - (optional) flag indicating whether the entity is attached to a model.  Defaults to true.
  */
  Entity(SUEntityRef entity, bool attached = true);

  /**
  * @brief Copy constructor with an optional parameter for the entity reference.
  *
  * SUEntityRef objects cannot be created from this class, so the Ref object must be passed to this constructor from a derived class object.
  * @param other - Entity object from which properties will be copied.
  * @param entity_ref - (optional) SUEntityRef object to assign to the copied object.
  */
  Entity(const Entity& other, SUEntityRef entity_ref = SU_INVALID);

  /**
  * @brief Destructor
  *
  * The C++ wrapper deals with releasing objects, so user does not have to keep track of memory allocations.
  */
  virtual ~Entity();

  /** @brief Copy assignment operator */
  Entity& operator=(const Entity& other);

  /**
  * @brief Copies properties fo the entity from another entity.
  */
  bool copy_properties_from(const Entity& entity);

  public:
  /*
  * @brief Returns a copy of the wrapped SUEntityRef. USE WITH CAUTION.
  *
  * Note that the Entity object still manages the lifetime of the reference, so the SUEntityRef could become invalid.
  */
  operator SUEntityRef() const;

  /*
  * @brief Returns a pointer to the wrapped SUEntityRef. USE WITH CAUTION.
  *
  * Note that the Entity object still manages the lifetime of the reference, so the SUEntityRef could become invalid.
  */
  operator SUEntityRef*();

  /*
  * @brief Returns the wrapped SUEntityRef. USE WITH CAUTION.
  *
  * Note that the Entity object still manages the lifetime of the reference, so the SUEntityRef could become invalid.
  */
  SUEntityRef ref() const;

  /**
  * @brief Returns true if the entity is attached to another object.
  */
  bool attached() const;

  /**
  * @brief Method lets the object know that it has been attached to a model.  This is important as it will let the object know that it does not need to "release" the object.
  * @param attach - (optional) true to let the object know that it has been attached to a model.  False to let the object know that it has not been attached.
  */
  void attached(bool attach);

  /**
  * @brief Returns the AttributeDictionaries collection attached to the entity.
  * @return vector of AttributeDictionary objects associated with the entity. If no AttributeDictionary objects are associated with the entity, an empty vector will be returned.
  */
  std::vector<AttributeDictionary> attribute_dictionaries() const;

  /**
  * @brief Returns an attribute dictionary object with a given name that is attached to an Entity.
  * @param name - string representing the name of the AttributeDictionary
  * @return AttributeDictionary object with the given name.
  */
  AttributeDictionary attribute_dictionary(const std::string& name) const;

  /**
  * @brief Adds the AttributeDictionary to the Entity.
  * @param dict - AttributeDictionary to add
  * @return true for success, false for failure.
  */
  bool add_dictionary(AttributeDictionary& dict);

  /**
  * @brief Copies attributes from another Entity object to this one.
  * @param entity - object to get the attributes from.
  * @param bool true to delete existing attributes, false to retain and overwrite the values. // TODO: as the C API does not currentlty allow you to delete attributes, this flag can have no effect.
  * @return true for success, false for failure.
  */
  bool copy_attributes_from(const Entity& entity /*, bool clear_existing = false*/);

  /**
  * The delete_attribute method is used to delete an attribute from an entity.
  * @param AttributeDictionary& object in which to find the key.
  * @param std::string with the name of the key of the attribute.
  */
  /*
  * TODO: deleting attributes is not possible with the current C API.  It could be used, however, to store a list of deleted attributes, so if this object is copied, the deleted attributes are not copied over to the new object.
  void delete_attribute(AttributeDictionary &dict, std::string key);
  */

  /**
  * @brief Checks if the entity is valid.
  *
  * Note that this function does not check if the entity has been deleted. @see operator!().
  */
  virtual bool is_valid() const;

  /**
  * @brief Returns true if this entity is not valid. Alias of !is_valid().
  */
  virtual bool operator!() const;

  /**
  * @brief Retrieve a unique ID assigned to an entity.
  * @return int32_t key for the Entity object.
  */
  int32_t entityID() const;

  /**
  * @brief Retrieves the value of an attribute in the entity's attribute dictionary.
  * @param dict_name - string name of the attribute dictionary.
  * @param key - the attribute key.
  * @param default_value - (optional) default TypedValue to return if no attribute is found. Defaults to an empty TypedValue
  * @return value TypedValue of the attribute.  If no attribute found, the default value passed is returned.
  */
  TypedValue get_attribute(const std::string& dict_name, const std::string& key, const TypedValue& default_value = TypedValue()) const;

  /**
  * @brief Retrieves the value of an attribute in the entity's attribute dictionary.
  * @param dict - AttributeDictionary object to search.
  * @param key - the attribute key.
  * @param default_value - (optional) default TypedValue to return if no attribute is found. Defaults to an empty TypedValue
  * @return value TypedValue of the attribute.  If no attribute found, the default value passed is returned.
  */
  TypedValue get_attribute(const AttributeDictionary &dict, const std::string& key, const TypedValue& default_value = TypedValue()) const;

  /**
  // TODO: C API does not currently allow traversing upwards
  parent()
  */

  /**
  * @brief Sets the value of an attribute in the given AttributeDictionary object.
  * @param dict_name - string name of AttributeDictionary object that the attribute is in.
  * @param key - std::string attribute key.
  * @param value - TypedValue to set.
  * @return true on success, false on failure
  */
  bool set_attribute(const std::string& dict_name, const std::string& key, const TypedValue& value);

  /**
  * @brief Sets the value of an attribute in the given AttributeDictionary object.
  * @param dict - AttributeDictionary object that the attribute is in.
  * @param key - std::string attribute key.
  * @param value - TypedValue to set.
  * @return true on success, false on failure
  */
  bool set_attribute(AttributeDictionary& dict, const std::string& key, const TypedValue& value);

  /**
  * @brief Returns the type of the entity. See enum SURefType.
  */
  enum SURefType entity_type() const;

  /**
  * Returns the model object that holds this entity.
  */
  Model model() const;

  /**
  * Returns the parent Entities object.
  */
  Entities parent() const;

  /**
  * Returns the persistent ID of the entity.  If the entity is not an entity that can have a persistent ID, the method will fail.
  */
  int64_t persistent_id() const;

  /**
  * Returns whether these objects are the same.
  */
  //bool operator==(const Entity& entity) const;

  /**
  * @brief Equality operator for two entity objects
  */
  friend bool operator==(const Entity& lhs, const Entity& rhs);

  /**
  * @brief Non-equality operator for two entity objects
  */
  friend bool operator!=(const Entity& lhs, const Entity& rhs);

  /**
  * Hash function for use wht unordered_map
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
