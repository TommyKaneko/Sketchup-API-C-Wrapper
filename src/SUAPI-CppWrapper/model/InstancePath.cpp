//
//  InstancePath.cpp
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/InstancePath.hpp"

#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"
#include "SUAPI-CppWrapper/String.hpp"

#include <cassert>

namespace CW {

SUInstancePathRef InstancePath::create_instance_path() {
  SUInstancePathRef instance_path_ref = SU_INVALID;
  SUResult res = SUInstancePathCreate(&instance_path_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return instance_path_ref;
}


SUInstancePathRef InstancePath::copy_reference(const InstancePath& other) {
  SUInstancePathRef instance_path_ref = create_instance_path();
  SUResult res = SUInstancePathCreateCopy(&instance_path_ref, other.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
  return instance_path_ref;
}


InstancePath::InstancePath():
  m_instance_path(create_instance_path())
{}


InstancePath::InstancePath(SUInstancePathRef instance_path):
  m_instance_path(instance_path)
{}


InstancePath::InstancePath(const InstancePath& other):
  InstancePath(copy_reference(other))
{}


InstancePath::~InstancePath() {
  SUResult res = SUInstancePathRelease(&m_instance_path);
  assert(res == SU_ERROR_NONE); _unused(res);
}


InstancePath& InstancePath::operator=(const InstancePath& other) {
  SUResult res = SUInstancePathRelease(&m_instance_path);
  assert(res == SU_ERROR_NONE); _unused(res);
  m_instance_path = copy_reference(other);
  return *this;
}


SUInstancePathRef InstancePath::ref() const {
  return m_instance_path;
}


InstancePath::operator SUInstancePathRef() const {
  return this->ref();
}


InstancePath::operator SUInstancePathRef*() {
  return &m_instance_path;
}


InstancePath& InstancePath::push(const ComponentInstance& instance) {
  SUResult res = SUInstancePathPushInstance(m_instance_path, instance.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


InstancePath& InstancePath::pop() {
  SUResult res = SUInstancePathPopInstance(m_instance_path);
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


InstancePath& InstancePath::set_leaf(const Entity& entity) {
  SUResult res = SUInstancePathSetLeaf(m_instance_path, entity.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
  return *this;
}


size_t InstancePath::depth() {
  size_t depth = 0;
  SUResult res = SUInstancePathGetPathDepth(m_instance_path, &depth);
  assert(res == SU_ERROR_NONE); _unused(res);
  return depth;
}


size_t InstancePath::full_depth() {
  size_t depth = 0;
  SUResult res = SUInstancePathGetFullDepth(m_instance_path, &depth);
  assert(res == SU_ERROR_NONE); _unused(res);
  return depth;
}


Transformation InstancePath::total_transformation() {
  SUTransformation transform;
  SUResult res = SUInstancePathGetTransform(m_instance_path, &transform);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Transformation(transform);
}


Transformation InstancePath::transformation_at_depth(size_t depth) {
  SUTransformation transform;
  SUResult res = SUInstancePathGetTransformAtDepth(m_instance_path, depth, &transform);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Transformation(transform);
}


ComponentInstance InstancePath::instance_at_depth(size_t depth) {
  SUComponentInstanceRef instance = SU_INVALID;
  SUResult res = SUInstancePathGetInstanceAtDepth(m_instance_path, depth, &instance);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ComponentInstance(instance);
}


Entity InstancePath::leaf_entity() {
  SUEntityRef entity = SU_INVALID;
  SUResult res = SUInstancePathGetLeafAsEntity(m_instance_path, &entity);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Entity(entity);
}


DrawingElement InstancePath::leaf() {
  SUDrawingElementRef element = SU_INVALID;
  SUResult res = SUInstancePathGetLeaf(m_instance_path, &element);
  assert(res == SU_ERROR_NONE); _unused(res);
  return DrawingElement(element);
}


bool InstancePath::valid() {
  bool valid;
  SUResult res = SUInstancePathIsValid(m_instance_path, &valid);
  assert(res == SU_ERROR_NONE); _unused(res);
  return valid;
}


bool InstancePath::empty() {
  bool empty;
  SUResult res = SUInstancePathIsEmpty(m_instance_path, &empty);
  assert(res == SU_ERROR_NONE); _unused(res);
  return empty;
}


bool InstancePath::contains(const Entity& entity) {
  bool contains;
  SUResult res = SUInstancePathContains(m_instance_path, entity.ref(), &contains);
  assert(res == SU_ERROR_NONE); _unused(res);
  return contains;
}


String InstancePath::persistent_id() {
  SUStringRef pid = SU_INVALID;
  SUResult res = SUStringCreate(&pid);
  assert(res == SU_ERROR_NONE);
  res = SUInstancePathGetPersistentID(m_instance_path, &pid);
  assert(res == SU_ERROR_NONE); _unused(res);
  return String(pid);
}


String InstancePath::persistent_id_at_depth(size_t depth) {
  SUStringRef pid = SU_INVALID;
  SUResult res = SUStringCreate(&pid);
  assert(res == SU_ERROR_NONE);
  res = SUInstancePathGetPersistentIDAtDepth(m_instance_path, depth, &pid);
  assert(res == SU_ERROR_NONE); _unused(res);
  return String(pid);
}


} /* namespace CW */

