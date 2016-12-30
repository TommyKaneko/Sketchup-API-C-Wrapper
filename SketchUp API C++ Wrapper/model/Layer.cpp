//
//  Layer.cpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#include "Layer.hpp"

#include <cassert>

namespace CW {

Layer::Layer():
  m_layer(create_layer()),
  m_release_on_destroy(true),
  Entity(SULayerToEntity(m_layer)) // TODO: probably not good here....
{}


Layer::Layer(SULayerRef layer_ref, bool release_on_destroy):
  m_layer(layer_ref),
  m_release_on_destroy(release_on_destroy),
  Entity(SULayerToEntity(layer_ref))
{}


Layer::~Layer() {
  if (m_release_on_destroy) {
    SULayerRelease(&m_layer);
  }
}


SULayerRef Layer::create_layer() {
  SULayerRef layer = SU_INVALID;
  SU_RESULT res = SULayerCreate(&layer);
  assert(res == SU_ERROR_NONE);
  return layer;
}

SULayerRef Layer::ref() {
  m_release_on_destroy = false;
  return m_layer;
}

Layer::operator SULayerRef() {
  return ref();
}

Layer::operator SULayerRef*() {
  return &m_layer;
}

  
} /* namespace CW */