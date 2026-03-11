//
//  MeshHelper.hpp
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

#ifndef MeshHelper_hpp
#define MeshHelper_hpp

#include <vector>
#include <SketchUpAPI/model/mesh_helper.h>

#include "SUAPI-CppWrapper/Geometry.hpp"

namespace CW {

// Forward declarations
class Face;
class TextureWriter;
class UVHelper;

/**
 * @brief C++ wrapper for SUMeshHelperRef.
 *
 * MeshHelper is a helper class that tessellates a Face into triangles and
 * provides access to the resulting vertices, normals, vertex indices, and
 * STQ texture coordinates.
 *
 * MeshHelper is a standalone helper object — it does not inherit from Entity
 * and is not part of the model hierarchy. It must be explicitly created and
 * released.
 *
 * @see SUMeshHelperRef
 */
class MeshHelper {
  private:
  SUMeshHelperRef m_mesh;

  public:
  /**
  * @brief Constructs a NULL MeshHelper object.
  */
  MeshHelper();

  /**
  * @brief Constructs a MeshHelper by tessellating a face.
  * @param face - the face to tessellate.
  */
  MeshHelper(const Face& face);

  /** @brief Destructor. Releases the mesh helper. */
  ~MeshHelper();

  // Non-copyable
  MeshHelper(const MeshHelper&) = delete;
  MeshHelper& operator=(const MeshHelper&) = delete;

  // Moveable
  MeshHelper(MeshHelper&& other) noexcept;
  MeshHelper& operator=(MeshHelper&& other) noexcept;

  /**
  * @brief Returns the SUMeshHelperRef object.
  */
  SUMeshHelperRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null mesh helper.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the number of triangles in the tessellated mesh.
  * @throws std::logic_error if the mesh is null.
  */
  size_t num_triangles() const;

  /**
  * @brief Retrieves the number of vertices in the tessellated mesh.
  * @throws std::logic_error if the mesh is null.
  */
  size_t num_vertices() const;

  /**
  * @brief Retrieves the vertex indices for the triangles. Every three
  *   consecutive indices form one triangle.
  * @throws std::logic_error if the mesh is null.
  */
  std::vector<size_t> vertex_indices() const;

  /**
  * @brief Retrieves all vertices of the tessellated mesh.
  * @throws std::logic_error if the mesh is null.
  */
  std::vector<Point3D> vertices() const;

  /**
  * @brief Retrieves the front face STQ texture coordinates.
  *
  * STQ coordinates use Point3D as the container: S maps to x, T maps to y,
  * Q maps to z.
  *
  * @throws std::logic_error if the mesh is null.
  */
  std::vector<Point3D> front_stq_coords() const;

  /**
  * @brief Retrieves the back face STQ texture coordinates.
  *
  * STQ coordinates use Point3D as the container: S maps to x, T maps to y,
  * Q maps to z.
  *
  * @throws std::logic_error if the mesh is null.
  */
  std::vector<Point3D> back_stq_coords() const;

  /**
  * @brief Retrieves all vertex normals of the tessellated mesh.
  * @throws std::logic_error if the mesh is null.
  */
  std::vector<Vector3D> normals() const;
};

} /* namespace CW */

#endif /* MeshHelper_hpp */
