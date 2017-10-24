//
//  Entity.cpp
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

#include "SUAPI-CppWrapper/model/Entity.hpp"

#include <cassert>
#include "SUAPI-CppWrapper/model/AttributeDictionary.hpp"


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
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::attribute_dictionaries(): Entity is null");
  }
  size_t num_dicts = 0;
  SUResult res = SUEntityGetNumAttributeDictionaries(m_entity, &num_dicts);
  assert(res == SU_ERROR_NONE);
  SUAttributeDictionaryRef* dicts_ref = new SUAttributeDictionaryRef[num_dicts];
  res = SUEntityGetAttributeDictionaries(m_entity, num_dicts, &dicts_ref[0], &num_dicts);
  assert(res == SU_ERROR_NONE);
  std::vector<AttributeDictionary> dicts(num_dicts);
  for (size_t i=0; i < num_dicts; i++) {
    dicts[i] = AttributeDictionary(dicts_ref[i]);
  }
  delete dicts_ref;
  return dicts;
}

AttributeDictionary Entity::attribute_dictionary(const std::string& name) const {
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::attribute_dictionary(): Entity is null");
  }
  char const *c_name = name.c_str();
  SUAttributeDictionaryRef dict_ref = SU_INVALID;
  SUResult res = SUEntityGetAttributeDictionary(m_entity, &c_name[0], &dict_ref);
  if (res == SU_ERROR_NONE) {
  	return AttributeDictionary(dict_ref);
  }
  // Otherwise return null object
  return AttributeDictionary();
}

bool Entity::copy_attributes_from(const Entity& entity) {
	if (!(*this) || !entity) {
  	throw std::logic_error("CW::Entity::copy_attributes_from(): Entity is null");
  }
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
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::entityID(): Entity is null");
  }
  int32_t entity_id = SU_INVALID;
  SUEntityGetID(m_entity, &entity_id);
  return entity_id;
}

TypedValue Entity::get_attribute(const std::string& dict_name, const std::string& key) const {
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::get_attribute(): Entity is null");
  }
  TypedValue default_value;
  return get_attribute(dict_name, key, default_value);
}

TypedValue Entity::get_attribute(const std::string& dict_name, const std::string& key, const TypedValue& default_value) const {
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::get_attribute(): Entity is null");
  }
  AttributeDictionary dict = attribute_dictionary(dict_name);
  return get_attribute(dict, key, default_value);
}

TypedValue Entity::get_attribute(const AttributeDictionary& dict, const std::string& key) const {
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::get_attribute(): Entity is null");
  }
  TypedValue default_value;
  return get_attribute(dict, key, default_value);
}

TypedValue Entity::get_attribute(const AttributeDictionary& dict, const std::string& key, const TypedValue& default_value) const {
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::get_attribute(): Entity is null");
  }
  return dict.get_attribute(key, default_value);
}

/*
// TODO: C API does not currently allow traversing upwards
parent()
*/


bool Entity::set_attribute(const std::string& dict_name, const std::string& key, const TypedValue& value) {
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::set_attribute(): Entity is null");
  }
  AttributeDictionary dict = attribute_dictionary(dict_name);
  return set_attribute(dict, key, value);
}

bool Entity::set_attribute(AttributeDictionary& dict, const std::string& key, const TypedValue& value) {
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::set_attribute(): Entity is null");
  }
  return dict.set_attribute(key, value);
}


enum SURefType Entity::entity_type() const{
	if (!(*this)) {
  	throw std::logic_error("CW::Entity::entity_type(): Entity is null");
  }
  return SUEntityGetType(m_entity);
}

/*
bool Entity::operator==(const Entity& entity) const {
  if (m_entity.ptr == entity.m_entity.ptr) {
  	return true;
  }
  return false;
  //return entityID() == entity.entityID();
}
*/

bool operator==(const Entity& lhs, const Entity& rhs) {
	if (!lhs || !rhs) {
  	if (!lhs && !rhs) {
    	return true;
    }
    return false;
  }
  
  return lhs.m_entity.ptr == rhs.m_entity.ptr;
}

bool operator!=(const Entity& lhs, const Entity& rhs) {
	return !(lhs == rhs);
}

} /* namespace CW */
