//
//  Entity.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include <SketchUpAPI/model/entity.h>

#include "AttributeDictionary.hpp"

namespace CW {

/*
* Entity object wrapper
*/
class Entity {
  protected:
  SUEntityRef m_entity;
  SU_RESULT m_create_result;
  
  public:
  /*
  * Creates a new Entity object.
  */
  Entity(SUEntityRef entity);
  
  /*
  * Creates a wrapper object for an existing SUEntityRef object.
  */
  Entity(SUEntityRef &entity);
  
  ~Entity();

  /*
  * The class object can be converted to a SUEntityRef without loss of data.
  */
  operator SUEntityRef();
  operator SUEntityRef*();
  
  /*
  * The attribute_dictionaries method is used to retrieve the AttributeDictionaries collection attached to the entity.
  * @return vector of AttributeDictionary objects associated with the entity. If no AttributeDictionary objects are associated with the entity, an empty vector will be returned.
  */
  std::vector<AttributeDictionary>	attribute_dictionaries() const;
  
  /*
  * Retrieves an attribute dictionary object with a given name that is attached to an Entity.
  *
  */
  AttributeDictionary attribute_dictionary(const std::string name) const;
  
  /*
  * Copies attributes from another Entity object to this one.
  * @param entity object to get the attributes from.
  * @param bool true to delete existing attributes, false to retain and overwrite the values. // TODO: as the C API does not currentlty allow you to delete attributes, this flag can have no effect.
  * @return true for success, false for failure.
  */
  bool copy_attributes_from(Entity entity /*, bool clear_existing = false*/);
  
  /*
  * The delete_attribute method is used to delete an attribute from an entity.
  * @param AttributeDictionary& object in which to find the key.
  * @param std::string with the name of the key of the attribute.
  */
  /*
  * TODO: deleting attributes is not possible with the current C API.  It could be used, however, to store a list of deleted attributes, so if this object is copied, the deleted attributes are not copied over to the new object.
  void delete_attribute(AttributeDictionary &dict, std::string key);
  */
  
  /*
  * Determines if your entity is still valid (not deleted by another script, for example.)
  */
  /*
  * TODO: as deleting an entity is not currently possible with the C API, this function cannot be used.
  bool valid();
  */
  
  /*
  * Retrieve a unique ID assigned to an entity.
  * @return int32_t key for the Entity object
  */
  int32_t entityID() const;
  
  
  /*
  * Retrieves the value of an attribute in the entity's attribute dictionary.
  * @param dict_name string name of the attribute dictionary, or AttributeDictionary object.
  * @param std::string attribute key.
  * @param default_value (optional) default TypedValue to return if no attribute is found
  * @return value TypedValue of the attribute.  If no attribute found, the default value passed is returned.
  */
  TypedValue get_attribute(const std::string dict_name, const std::string key, const TypedValue default_value = "") const;
  TypedValue get_attribute(const AttributeDictionary &dict, const std::string key, const TypedValue default_value = "") const;
  
  /*
  // TODO: C API does not currently allow traversing upwards
  parent()
  */
  
  /*
  * Sets the value of an attribute in the given AttributeDictionary object.
  * @param AttributeDictionary object or string name of AttributeDictionary object that the attribute is in.
  * @param std::string attribute key.
  * @param std::string value to set
  * @return true on success, false on failure
  */
  bool set_attribute(const std::string dict_name, const std::string key, const TypedValue value);
  bool set_attribute(AttributeDictionary &dict, const std::string key, const TypedValue value);
  
  /*
  * Returns the type of the entity. See enum SURefType.
  */
  enum SURefType entity_type() const;

  
};

} /* namespace CW */

#endif /* Entity_hpp */
