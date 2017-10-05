//
//  Layer.cpp
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

#include "SUAPI-CppWrapper/model/Layer.hpp"

#include <cassert>
#include "SUAPI-CppWrapper/String.hpp"


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
	if (other.m_attached || SUIsInvalid(other.m_layer)) {
  	return other.m_layer;
  }
	SULayerRef new_layer = create_layer();
  return new_layer;
}

/******************************
** Constructors / Destructor **
*******************************/
Layer::Layer():
  Entity(SU_INVALID, false),
  m_layer(SU_INVALID)
{}


Layer::Layer(SULayerRef layer_ref, bool attached):
  Entity(SULayerToEntity(layer_ref), attached),
  m_layer(layer_ref)
{}


Layer::Layer(const Layer& other):
	Entity(SULayerToEntity(copy_reference(other)), other.m_attached),
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
	if (!other.m_attached && SUIsValid(other.m_layer)) {
		this->name(other.name());
    // TODO: Set layer material
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
  return false;
}


String Layer::name() const {
  if(!(*this)) {
  	throw std::logic_error("CW::Layer::name(): Layer is null");
  }
	String string;
	SU_RESULT res = SULayerGetName(m_layer, string);
  assert(res == SU_ERROR_NONE);
  return string;
}


void Layer::name(const String& string) {
  if(!(*this)) {
  	throw std::logic_error("CW::Layer::name(): Layer is null");
  }
  SU_RESULT res = SULayerSetName(m_layer, string.std_string().c_str());
	assert(res == SU_ERROR_NONE);
}


void Layer::name(const std::string& string) {
  if(!(*this)) {
  	throw std::logic_error("CW::Layer::name(): Layer is null");
  }
	SU_RESULT res = SULayerSetName(m_layer, string.c_str());
	assert(res == SU_ERROR_NONE);
}


} /* namespace CW */
