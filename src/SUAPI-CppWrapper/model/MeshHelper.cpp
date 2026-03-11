//
//  MeshHelper.cpp
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

#include "SUAPI-CppWrapper/model/MeshHelper.hpp"

#include <cassert>
#include <stdexcept>
#include <algorithm>

#include "SUAPI-CppWrapper/model/Face.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
MeshHelper::MeshHelper():
  m_mesh(SU_INVALID)
{}


MeshHelper::MeshHelper(const Face& face):
  m_mesh(SU_INVALID)
{
  SUResult res = SUMeshHelperCreate(&m_mesh, face.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


MeshHelper::~MeshHelper() {
  if (SUIsValid(m_mesh)) {
    SUResult res = SUMeshHelperRelease(&m_mesh);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


MeshHelper::MeshHelper(MeshHelper&& other) noexcept:
  m_mesh(other.m_mesh)
{
  other.m_mesh = SU_INVALID;
}


MeshHelper& MeshHelper::operator=(MeshHelper&& other) noexcept {
  if (this != &other) {
    if (SUIsValid(m_mesh)) {
      SUMeshHelperRelease(&m_mesh);
    }
    m_mesh = other.m_mesh;
    other.m_mesh = SU_INVALID;
  }
  return *this;
}


/*******************
** Public Methods **
********************/
SUMeshHelperRef MeshHelper::ref() const {
  return m_mesh;
}


bool MeshHelper::operator!() const {
  return SUIsInvalid(m_mesh);
}


size_t MeshHelper::num_triangles() const {
  if (!(*this)) {
    throw std::logic_error("CW::MeshHelper::num_triangles(): MeshHelper is null");
  }
  size_t count;
  SUResult res = SUMeshHelperGetNumTriangles(m_mesh, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


size_t MeshHelper::num_vertices() const {
  if (!(*this)) {
    throw std::logic_error("CW::MeshHelper::num_vertices(): MeshHelper is null");
  }
  size_t count;
  SUResult res = SUMeshHelperGetNumVertices(m_mesh, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


std::vector<size_t> MeshHelper::vertex_indices() const {
  if (!(*this)) {
    throw std::logic_error("CW::MeshHelper::vertex_indices(): MeshHelper is null");
  }
  size_t num_tris;
  SUResult res = SUMeshHelperGetNumTriangles(m_mesh, &num_tris);
  assert(res == SU_ERROR_NONE);
  size_t index_count = num_tris * 3;
  std::vector<size_t> indices(index_count);
  size_t actual_count;
  res = SUMeshHelperGetVertexIndices(m_mesh, index_count, indices.data(), &actual_count);
  assert(res == SU_ERROR_NONE); _unused(res);
  indices.resize(actual_count);
  return indices;
}


std::vector<Point3D> MeshHelper::vertices() const {
  if (!(*this)) {
    throw std::logic_error("CW::MeshHelper::vertices(): MeshHelper is null");
  }
  size_t count;
  SUResult res = SUMeshHelperGetNumVertices(m_mesh, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUPoint3D> su_points(count);
  size_t actual_count;
  res = SUMeshHelperGetVertices(m_mesh, count, su_points.data(), &actual_count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Point3D> points(actual_count);
  std::transform(su_points.begin(), su_points.begin() + actual_count, points.begin(),
  [](const SUPoint3D& p) { return Point3D(p); });
  return points;
}


std::vector<Point3D> MeshHelper::front_stq_coords() const {
  if (!(*this)) {
    throw std::logic_error("CW::MeshHelper::front_stq_coords(): MeshHelper is null");
  }
  size_t count;
  SUResult res = SUMeshHelperGetNumVertices(m_mesh, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUPoint3D> su_stq(count);
  size_t actual_count;
  res = SUMeshHelperGetFrontSTQCoords(m_mesh, count, su_stq.data(), &actual_count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Point3D> stq(actual_count);
  std::transform(su_stq.begin(), su_stq.begin() + actual_count, stq.begin(),
  [](const SUPoint3D& p) { return Point3D(p); });
  return stq;
}


std::vector<Point3D> MeshHelper::back_stq_coords() const {
  if (!(*this)) {
    throw std::logic_error("CW::MeshHelper::back_stq_coords(): MeshHelper is null");
  }
  size_t count;
  SUResult res = SUMeshHelperGetNumVertices(m_mesh, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUPoint3D> su_stq(count);
  size_t actual_count;
  res = SUMeshHelperGetBackSTQCoords(m_mesh, count, su_stq.data(), &actual_count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Point3D> stq(actual_count);
  std::transform(su_stq.begin(), su_stq.begin() + actual_count, stq.begin(),
  [](const SUPoint3D& p) { return Point3D(p); });
  return stq;
}


std::vector<Vector3D> MeshHelper::normals() const {
  if (!(*this)) {
    throw std::logic_error("CW::MeshHelper::normals(): MeshHelper is null");
  }
  size_t count;
  SUResult res = SUMeshHelperGetNumVertices(m_mesh, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUVector3D> su_normals(count);
  size_t actual_count;
  res = SUMeshHelperGetNormals(m_mesh, count, su_normals.data(), &actual_count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Vector3D> result(actual_count);
  std::transform(su_normals.begin(), su_normals.begin() + actual_count, result.begin(),
  [](const SUVector3D& n) { return Vector3D(n); });
  return result;
}


} /* namespace CW */
