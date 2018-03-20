//
//  Layer.cpp
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

#include "SUAPI-CppWrapper/model/Layer.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"


namespace CW {
/***************************
** Private Static Methods **
****************************/
SULayerRef Layer::create_layer() {
  SULayerRef layer = SU_INVALID;
  SUResult res = SULayerCreate(&layer);
  assert(res == SU_ERROR_NONE);
  return layer;
}

SULayerRef Layer::copy_reference(const Layer& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  SULayerRef new_layer = create_layer();
  return new_layer;
}

/******************************
** Constructors / Destructor **
*******************************/
Layer::Layer():
  Entity(SU_INVALID, false)
{}


Layer::Layer(SULayerRef layer_ref, bool attached):
  Entity(SULayerToEntity(layer_ref), attached)
{}


Layer::Layer(const Layer& other):
  Entity(SULayerToEntity(copy_reference(other)), other.m_attached)
{
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    this->name(other.name());
    // TODO: Set layer material
  }
}


Layer::~Layer() {
  if (!m_attached && SUIsValid(m_entity)) {
    SULayerRef layer = this->ref();
    SUResult res = SULayerRelease(&layer);
    assert(res == SU_ERROR_NONE);
  }
}


/*******************
** Public Methods **
********************/
Layer& Layer::operator=(const Layer& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SULayerRef layer = this->ref();
    SUResult res = SULayerRelease(&layer);
    assert(res == SU_ERROR_NONE);
  }
  m_entity = SULayerToEntity(copy_reference(other));
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    this->name(other.name());
    // TODO: Set layer material
  }
  Entity::operator=(other);
  return (*this);
}


SULayerRef Layer::ref() const {
  return SULayerFromEntity(m_entity);
}

Layer::operator SULayerRef() const {
  return this->ref();
}


bool Layer::operator!() const {
  if (SUIsInvalid(m_entity)) {
    return true;
  }
  String name;
  SUResult res = SULayerGetName(this->ref(), name);
  if (res == SU_ERROR_NULL_POINTER_OUTPUT) {
    return true;
  }
  return false;
}


Layer Layer::copy() const {
  if(!(*this)) {
    return Layer(create_layer(), false);
  }
  Layer new_layer(create_layer(), false);
  new_layer.name(this->name());
  return new_layer;
}


String Layer::name() const {
  if(!(*this)) {
    throw std::logic_error("CW::Layer::name(): Layer is null");
  }
  String string;
  SUResult res = SULayerGetName(this->ref(), string);
  assert(res == SU_ERROR_NONE);
  return string;
}


void Layer::name(const String& string) {
  if(!(*this)) {
    throw std::logic_error("CW::Layer::name(): Layer is null");
  }
  SUResult res = SULayerSetName(this->ref(), string.std_string().c_str());
  assert(res == SU_ERROR_NONE);
}


void Layer::name(const std::string& string) {
  if(!(*this)) {
    throw std::logic_error("CW::Layer::name(): Layer is null");
  }
  SUResult res = SULayerSetName(this->ref(), string.c_str());
  assert(res == SU_ERROR_NONE);
}


} /* namespace CW */
