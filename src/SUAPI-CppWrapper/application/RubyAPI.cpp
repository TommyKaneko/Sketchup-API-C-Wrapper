//
//  RubyAPI.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2022 Tom Kaneko
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/application/RubyAPI.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/model/ImageRep.hpp"

#include "SUAPI-CppWrapper/application/ruby/utilities.h"

#include <cassert>
#include <stdexcept>

namespace CW {
/**************************
* Public static methods **
***************************/

RUBY_VALUE RubyAPI::entity_to_ruby(const Entity& entity) {
  RUBY_VALUE ruby_entity = SU_INVALID;
  SU_RESULT res = SUEntityToRuby(entity, &ruby_entity);
  assert(res != SU_ERROR_NULL_POINTER_OUTPUT);
  
  if (res == SU_ERROR_INVALID_INPUT) {
    throw std::invalid_argument("CW::RubyAPI::entity_to_ruby(): the entity to convert is not valid");
  }
  else if (res == SU_ERROR_INVALID_ARGUMENT) {
    throw std::invalid_argument("CW::RubyAPI::entity_to_ruby(): the entity is not from a model");
  }
  else if (res == SU_ERROR_NO_DATA) {
    throw std::invalid_argument("CW::RubyAPI::entity_to_ruby(): this method must be called from within SketchUp");
  }
  return ruby_entity;
}
  

Entity RubyAPI::entity_from_ruby(const RUBY_VALUE& ruby_entity) {
  SUEntityRef entity_ref = SU_INVALID;
  SU_RESULT res = SUEntityFromRuby(ruby_entity, &entity_ref);
  assert(res != SU_ERROR_NULL_POINTER_OUTPUT);
  assert(res != SU_ERROR_OVERWRITE_VALID);
  if (res == SU_ERROR_INVALID_ARGUMENT) {
    throw std::invalid_argument("CW::RubyAPI::entity_from_ruby(): the entity has been deleted, or is not a Sketchup::Entity type");
  }
  else if (res == SU_ERROR_NO_DATA) {
    throw std::invalid_argument("CW::RubyAPI::entity_from_ruby(): this method must be called from within SketchUp");
  }
  
  return Entity(entity_ref, true);
}


RUBY_VALUE RubyAPI::imagerep_to_ruby(const ImageRep& imagerep) {
  RUBY_VALUE ruby_imagerep = SU_INVALID;
  SU_RESULT res = SUImageRepToRuby(imagerep, &ruby_imagerep);
  assert(res != SU_ERROR_NULL_POINTER_OUTPUT);
  
  if (res == SU_ERROR_INVALID_INPUT) {
    throw std::invalid_argument("CW::RubyAPI::imagerep_to_ruby(): the ImageRep object is not valid");
  }
  else if (res == SU_ERROR_NO_DATA) {
    throw std::invalid_argument("CW::RubyAPI::imagerep_to_ruby(): this method must be called from within SketchUp");
  }
  return ruby_imagerep;
}


ImageRep RubyAPI::imagerep_from_ruby(const RUBY_VALUE& ruby_imagerep) {
  SUImageRepRef imagerep_ref = SU_INVALID;
  SU_RESULT res = SUImageRepFromRuby(ruby_imagerep, &imagerep_ref);
  assert(res != SU_ERROR_NULL_POINTER_OUTPUT);
  assert(res != SU_ERROR_OVERWRITE_VALID);
  if (res == SU_ERROR_INVALID_ARGUMENT) {
    throw std::invalid_argument("CW::RubyAPI::imagerep_from_ruby(): the object passed as a parameter is not a ImageRep type");
  }
  else if (res == SU_ERROR_NO_DATA) {
    throw std::invalid_argument("CW::RubyAPI::imagerep_from_ruby(): this method must be called from within SketchUp");
  }
  return ImageRep(imagerep_ref, true);
}


} /* namespace CW */
