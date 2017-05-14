//
//  Entity.cpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "Entity.hpp"

#include <cassert>
#include "AttributeDictionary.hpp"


namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
Entity::Entity():
	m_entity(SU_INVALID),
  m_attached(false)
{}


Entity::Entity(SUEntityRef entity, bool attached):
	m_entity(entity),
  m_attached(attached)
{}


Entity::Entity(const Entity& other, SUEntityRef entity_ref):
	m_entity(entity_ref),
	m_attached(other.m_attached)
{}


Entity::~Entity() {
	// Entity objects cannot release themselves.
}

/*******************
** Public Methods **
********************/
Entity& Entity::operator=(const Entity& other) {
	m_attached = other.m_attached;
  if (SUIsInvalid(other.m_entity)) {
 		m_entity = other.m_entity;
  }
  else if (!other.m_attached) {
    this->copy_attributes_from(other);
  }
  return (*this);
}


Entity::operator SUEntityRef() const {
	return m_entity;
}


Entity::operator SUEntityRef*() {
	return &m_entity;
}


bool Entity::operator!() const {
	return SUIsInvalid(m_entity);
}



void Entity::attached(bool attach) {
	m_attached = attach;
}


std::vector<AttributeDictionary>	Entity::attribute_dictionaries() const {
  size_t num_dicts = 0;
  SU_RESULT res = SUEntityGetNumAttributeDictionaries(m_entity, &num_dicts);
  assert(res == SU_ERROR_NONE);
  SUAttributeDictionaryRef dicts_ref[num_dicts];
  res = SUEntityGetAttributeDictionaries(m_entity, num_dicts, &dicts_ref[0], &num_dicts);
  assert(res == SU_ERROR_NONE);
  std::vector<AttributeDictionary> dicts(num_dicts);
  for (size_t i=0; i < num_dicts; i++) {
    dicts[i] = AttributeDictionary(dicts_ref[i]);
  }
  return dicts;
}

AttributeDictionary Entity::attribute_dictionary(const std::string& name) const {
  char const *c_name = name.c_str();
  SUAttributeDictionaryRef dict_ref = SU_INVALID;
  SU_RESULT res = SUEntityGetAttributeDictionary(m_entity, &c_name[0], &dict_ref);
  if (res == SU_ERROR_NONE) {
  	return AttributeDictionary(dict_ref);
  }
  // Otherwise return null object
  return AttributeDictionary();
}

bool Entity::copy_attributes_from(const Entity& entity) {
  std::vector<AttributeDictionary> atts_from = entity.attribute_dictionaries();
  for (size_t i=0; i < atts_from.size(); i++) {
    std::vector<std::string> from_keys = atts_from[i].get_keys();
    for (size_t j=0; j < from_keys.size(); j++) {
      set_attribute(atts_from[i].get_name(), from_keys[j], atts_from[i].get_value(from_keys[j]));
    }
  }
  // TODO do some checking.
  return true;
}

/* TODO: deleting attributes is not possible with the current C API.  It could be used, however, to store a list of deleted attributes, so if this object is copied, the deleted attributes are not copied over to the new object.
void delete_attribute(AttributeDictionary &dict, std::string key);
*/

/* TODO: as deleting an entity is not currently possible with the C API, this function cannot be used.
bool valid();
*/

int32_t Entity::entityID() const{
  int32_t entity_id = SU_INVALID;
  SUEntityGetID(m_entity, &entity_id);
  return entity_id;
}

TypedValue Entity::get_attribute(const std::string& dict_name, const std::string& key) const {
  TypedValue default_value;
  return get_attribute(dict_name, key, default_value);
}

TypedValue Entity::get_attribute(const std::string& dict_name, const std::string& key, const TypedValue& default_value) const {
  AttributeDictionary dict = attribute_dictionary(dict_name);
  return get_attribute(dict, key, default_value);
}

TypedValue Entity::get_attribute(const AttributeDictionary& dict, const std::string& key) const {
  TypedValue default_value;
  return get_attribute(dict, key, default_value);
}

TypedValue Entity::get_attribute(const AttributeDictionary& dict, const std::string& key, const TypedValue& default_value) const {
  return dict.get_attribute(key, default_value);
}

/*
// TODO: C API does not currently allow traversing upwards
parent()
*/


bool Entity::set_attribute(const std::string& dict_name, const std::string& key, const TypedValue& value) {
  AttributeDictionary dict = attribute_dictionary(dict_name);
  return set_attribute(dict, key, value);
}
bool Entity::set_attribute(AttributeDictionary& dict, const std::string& key, const TypedValue& value) {
  return dict.set_attribute(key, value);
}


enum SURefType Entity::entity_type() const{
  return SUEntityGetType(m_entity);
}


bool Entity::operator==(const Entity& entity) const {
  if (m_entity.ptr == entity.m_entity.ptr) {
  	return true;
  }
  return false;
  //return entityID() == entity.entityID();
}

} /* namespace CW */