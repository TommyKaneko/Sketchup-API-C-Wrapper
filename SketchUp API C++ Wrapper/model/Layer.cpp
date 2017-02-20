//
//  Layer.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "Layer.hpp"

#include <cassert>
#include "String.hpp"


namespace CW {
/***************************
** Private Static Methods **
****************************/
SULayerRef Layer::create_layer() {
  SULayerRef layer = SU_INVALID;
  SU_RESULT res = SULayerCreate(&layer);
  assert(res == SU_ERROR_NONE);
  return layer;
}

SULayerRef Layer::copy_reference(const Layer& other) {
	if (other.m_attached) {
  	return other.m_layer;
  }
	SULayerRef new_layer = create_layer();
  return new_layer;
}

/******************************
** Constructors / Destructor **
*******************************/
Layer::Layer():
  Entity(SULayerToEntity(create_layer()), false),
  m_layer(SULayerFromEntity(m_entity))
{}


Layer::Layer(SULayerRef layer_ref, bool attached):
  Entity(SULayerToEntity(layer_ref), attached),
  m_layer(layer_ref)
{}


Layer::Layer(const Layer& other):
	Entity(SULayerToEntity(copy_reference(other))),
  m_layer(SULayerFromEntity(m_entity))
{
	if (!other.m_attached && SUIsValid(other.m_layer)) {
		this->name(other.name());
    // TODO: Set layer material
  }
}


Layer::~Layer() {
  if (!m_attached && SUIsValid(m_layer)) {
    SU_RESULT res = SULayerRelease(&m_layer);
    assert(res == SU_ERROR_NONE);
  }
}


/*******************
** Public Methods **
********************/
Layer& Layer::operator=(const Layer& other) {
  if (!m_attached && SUIsValid(m_layer)) {
    SU_RESULT res = SULayerRelease(&m_layer);
    assert(res == SU_ERROR_NONE);
  }
  m_layer = copy_reference(other);
  m_entity = SULayerToEntity(m_layer);
  Entity::operator=(other);
  if (!other.m_attached) {
    this->name(other.name());
  }
  return (*this);
}


SULayerRef Layer::ref() const {
  return m_layer;
}

Layer::operator SULayerRef() const {
  return ref();
}

Layer::operator SULayerRef*() {
  return &m_layer;
}


bool Layer::operator!() const {
	if (SUIsInvalid(m_layer)) {
  	return true;
  }
  String name;
  SU_RESULT res = SULayerGetName(m_layer, name);
  if (res == SU_ERROR_NULL_POINTER_OUTPUT) {
  	return true;
  }
  // Also if it is valid and name is empty.
  /*if (name() == String()) {
  	return true;
  }*/
  return false;
}


String Layer::name() const {
	String string;
	SU_RESULT res = SULayerGetName(m_layer, string);
  assert(res == SU_ERROR_NONE);
  return string;
}


void Layer::name(const String& string) {
  SU_RESULT res = SULayerSetName(m_layer, string.std_string().c_str());
	assert(res == SU_ERROR_NONE);
}


void Layer::name(const std::string& string) {
	SU_RESULT res = SULayerSetName(m_layer, string.c_str());
	assert(res == SU_ERROR_NONE);
}


} /* namespace CW */