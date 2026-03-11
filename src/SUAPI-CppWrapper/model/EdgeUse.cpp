//
//  EdgeUse.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
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

#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/EdgeUse.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/Edge.hpp"
#include "SUAPI-CppWrapper/model/Face.hpp"
#include "SUAPI-CppWrapper/model/Loop.hpp"
#include "SUAPI-CppWrapper/model/Vertex.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUEdgeUseRef EdgeUse::copy_reference(const EdgeUse& other) {
  // EdgeUse is always attached (read-only), so just return the same ref
  return other.ref();
}


/******************************
** Constructors / Destructor **
*******************************/
EdgeUse::EdgeUse():
  Entity(SU_INVALID, true)
{}


EdgeUse::EdgeUse(SUEdgeUseRef edge_use, bool attached):
  Entity(SUEdgeUseToEntity(edge_use), attached)
{}


EdgeUse::EdgeUse(const EdgeUse& other):
  Entity(other, SUEdgeUseToEntity(copy_reference(other)))
{}


EdgeUse& EdgeUse::operator=(const EdgeUse& other) {
  m_entity = SUEdgeUseToEntity(copy_reference(other));
  Entity::operator=(other);
  return *this;
}


/*******************
** Public Methods **
********************/
SUEdgeUseRef EdgeUse::ref() const {
  return SUEdgeUseFromEntity(m_entity);
}


bool EdgeUse::operator!() const {
  return SUIsInvalid(m_entity);
}


Edge EdgeUse::edge() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::edge(): EdgeUse is null");
  }
  SUEdgeRef edge_ref = SU_INVALID;
  SUResult res = SUEdgeUseGetEdge(this->ref(), &edge_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Edge(edge_ref);
}


Loop EdgeUse::loop() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::loop(): EdgeUse is null");
  }
  SULoopRef loop_ref = SU_INVALID;
  SUResult res = SUEdgeUseGetLoop(this->ref(), &loop_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Loop(loop_ref);
}


Face EdgeUse::face() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::face(): EdgeUse is null");
  }
  SUFaceRef face_ref = SU_INVALID;
  SUResult res = SUEdgeUseGetFace(this->ref(), &face_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Face(face_ref);
}


std::vector<EdgeUse> EdgeUse::partners() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::partners(): EdgeUse is null");
  }
  size_t count = 0;
  SUResult res = SUEdgeUseGetNumPartners(this->ref(), &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<EdgeUse>(0);
  }
  std::vector<SUEdgeUseRef> refs(count, SU_INVALID);
  res = SUEdgeUseGetPartners(this->ref(), count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<EdgeUse> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(EdgeUse(refs[i]));
  }
  return result;
}


bool EdgeUse::is_reversed() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::is_reversed(): EdgeUse is null");
  }
  bool reversed;
  SUResult res = SUEdgeUseIsReversed(this->ref(), &reversed);
  assert(res == SU_ERROR_NONE); _unused(res);
  return reversed;
}


EdgeUse EdgeUse::previous() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::previous(): EdgeUse is null");
  }
  SUEdgeUseRef prev = SU_INVALID;
  SUResult res = SUEdgeUseGetPrevious(this->ref(), &prev);
  assert(res == SU_ERROR_NONE); _unused(res);
  return EdgeUse(prev);
}


EdgeUse EdgeUse::next() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::next(): EdgeUse is null");
  }
  SUEdgeUseRef nxt = SU_INVALID;
  SUResult res = SUEdgeUseGetNext(this->ref(), &nxt);
  assert(res == SU_ERROR_NONE); _unused(res);
  return EdgeUse(nxt);
}


Vertex EdgeUse::start_vertex() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::start_vertex(): EdgeUse is null");
  }
  SUVertexRef vertex = SU_INVALID;
  SUResult res = SUEdgeUseGetStartVertex(this->ref(), &vertex);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vertex(vertex);
}


Vertex EdgeUse::end_vertex() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::end_vertex(): EdgeUse is null");
  }
  SUVertexRef vertex = SU_INVALID;
  SUResult res = SUEdgeUseGetEndVertex(this->ref(), &vertex);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vertex(vertex);
}


Vector3D EdgeUse::start_vertex_normal() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::start_vertex_normal(): EdgeUse is null");
  }
  SUVector3D normal;
  SUResult res = SUEdgeUseGetStartVertexNormal(this->ref(), &normal);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(normal);
}


Vector3D EdgeUse::end_vertex_normal() const {
  if (!(*this)) {
    throw std::logic_error("CW::EdgeUse::end_vertex_normal(): EdgeUse is null");
  }
  SUVector3D normal;
  SUResult res = SUEdgeUseGetEndVertexNormal(this->ref(), &normal);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(normal);
}


} /* namespace CW */
