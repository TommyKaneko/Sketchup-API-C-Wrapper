//
//  Face.hpp
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

#ifndef Face_hpp
#define Face_hpp

#include <vector>

#include "SUAPI-CppWrapper/model/DrawingElement.hpp"
#include "SUAPI-CppWrapper/model/TextureWriter.hpp"
#include "SUAPI-CppWrapper/model/UVHelper.hpp"

#include <SketchUpAPI/model/face.h>
#include <SketchUpAPI/model/material.h>
#include <SketchUpAPI/model/loop.h>

namespace CW {

enum FacePointClass {
  PointUnknown, // (indicates an error),
  PointInside, // (point is on the face, not in a hole),
  PointOnVertex, // (point touches a vertex),
  PointOnEdge, // (point is on an edge),
  PointOutside, // (point outside the face or in a hole),
  PointNotOnPlane // (point off the face's plane).
};

class Point3D;
class Plane3D;
class Vector3D;
class Material;
class Vertex;
class Loop;
class LoopInput;
class Edge;
class MaterialPositionInput;

/**
 * @brief C++ wrapper for the SketchUp C API's SUFaceRef object.
 *
 * A Face represents a planar polygon in 3D space. A face is bounded by an
 * outer loop and may contain inner loops (holes).
 */
class Face :public DrawingElement {
  private:
  /**
   * @brief Creates a SUFaceRef from an array of points representing the outer loop.
   * @param outer_points vector of points for the vertices in the outer loop.
   * @return SUFaceRef object. If there was an error, SU_INVALID will be returned.
   */
  static SUFaceRef create_face(std::vector<Point3D>& outer_points);
  static SUFaceRef create_face(std::vector<Point3D>& outer_points, LoopInput& loop_input);

  /**
   * @brief Creates a SUFaceRef derived from an existing Face object.
   * @param face Face object to derive the new SUFaceRef from.
   * @return the existing SUFaceRef if attached; otherwise a new copy.
   */
  static SUFaceRef copy_reference(const Face& face);

  public:
  /**
   * @brief Constructor for creating a null Face object.
   */
  Face();

  /**
   * @brief Constructs a Face from an array of points representing the outer loop.
   * @param outer_loop vector of Point3D objects for the outer loop vertices.
   */
  Face(std::vector<Point3D>& outer_loop);

  /**
   * @brief Constructs a Face from an array of points and a LoopInput describing edge properties.
   * @param outer_loop vector of Point3D objects for the outer loop vertices.
   * @param loop_input LoopInput object with edge properties.
   */
  Face(std::vector<Point3D>& outer_loop, LoopInput& loop_input);

  /**
   * @brief Constructs a Face object from a pre-existing SUFaceRef.
   * @param face the SUFaceRef to wrap.
   * @param attached true if the face is already owned by a model (default: true).
   */
  Face(SUFaceRef face, bool attached = true);

  /** @brief Copy constructor */
  Face(const Face& other);

  /** @brief Destructor */
  ~Face();

  /** @brief Copy assignment operator */
  Face& operator=(const Face& other);

  /**
   * @brief Returns the SUFaceRef object for the Face.
   */
  SUFaceRef ref() const;

  /**
   * @brief The class object can be converted to a SUFaceRef without loss of data.
   */
  operator SUFaceRef() const;

  /**
   * @brief Returns whether the Face wraps a valid SUFaceRef.
   */
  operator bool() const;

  /**
   * @brief NOT operator. Checks if the Face is invalid.
   * @return true if the face is invalid.
   */
  bool operator!() const;

  /**
  * @brief Creates a detached copy of this face.
  * @throws std::logic_error if the face is null.
  * @return A new detached Face with the same geometry and materials.
  */
  Face copy() const;

  /**
   * @brief Computes the area of the face, taking into account inner loops and openings.
   * @return area in square inches (SketchUp internal units).
   * @throws std::logic_error if the face is null.
   */
  double area() const;

  /**
   * @brief Adds an inner loop (hole) to the face.
   * @param points vector of points representing the vertices of the inner loop.
   * @param loop_input LoopInput object with edge properties for the inner loop.
   * @throws std::logic_error if the face is null.
   * @throws std::invalid_argument if points and loop_input have mismatched sizes, or if the input is invalid.
   */
  void add_inner_loop(std::vector<Point3D>& points, LoopInput &loop_input);

  /**
   * @brief Retrieves the material assigned to the back side of the face.
   * @return Material object, or an invalid Material if none is assigned.
   * @throws std::logic_error if the face is null.
   */
  Material back_material() const;

  /**
   * @brief Sets the material assigned to the back side of the face.
   * @param material the Material to assign.
   * @return the Material that was set.
   * @throws std::logic_error if the face is null.
   */
  Material back_material(const Material& material);

  /**
   * @brief Determines where a point sits relative to the face.
   * @param point the Point3D to classify.
   * @return FacePointClass enum value indicating the point's relationship to the face.
   * @throws std::logic_error if the face is null.
   * @throws std::invalid_argument if the point is null.
   */
  FacePointClass classify_point(const Point3D& point);

  /**
   * @brief Retrieves all edges that bound the face, including edges of inner loops.
   * @return vector of Edge objects.
   * @throws std::logic_error if the face is null.
   */
  std::vector<Edge> edges();

  /**
   * @brief Gets the number of inner loops that bound the face.
   * @return number of inner loops.
   * @throws std::logic_error if the face is null.
   */
  size_t num_inner_loops() const;

  /**
   * @brief Retrieves all inner loops that bound the face.
   * @return vector of Loop objects representing the inner loops.
   * @throws std::logic_error if the face is null.
   */
  std::vector<Loop> inner_loops() const;

  /**
   * @brief Retrieves all loops that bound the face. The first Loop is the outer loop.
   * @return vector of Loop objects.
   * @throws std::logic_error if the face is null.
   */
  std::vector<Loop> loops() const;

  /**
   * @brief Retrieves the 3D normal vector to the face in the front direction.
   * @return Vector3D normal to the face.
   * @throws std::logic_error if the face is null.
   */
  Vector3D normal() const;

  /**
   * @brief Retrieves the outer loop that bounds the face.
   * @return Loop object representing the outer loop.
   * @throws std::logic_error if the face is null.
   */
  Loop outer_loop() const;

  /**
   * @brief Retrieves the plane of this face.
   * @return Plane3D object representing the face's plane.
   * @throws std::logic_error if the face is null.
   */
  Plane3D plane() const;

  #if SketchUpAPI_VERSION_MAJOR >= 2021
  /**
   * @brief Retrieves the texture projection direction for the given side of the face.
   * @param frontside true for front side, false for back side.
   * @return Vector3D representing the projection direction.
   * @throws std::logic_error if the face is null or has no projected texture.
   */
  Vector3D get_texture_projection(const bool frontside) const;

  /**
   * @brief Checks if the texture is positioned on the given side of the face.
   * @param front true for front side, false for back side.
   * @return true if the texture is positioned.
   * @throws std::logic_error if the face is null.
   */
  bool is_texture_positioned(bool front = true) const;
  bool is_texture_positioned_front() const;
  bool is_texture_positioned_back() const;

  /**
   * @brief Checks if the texture is projected on the given side of the face.
   * @param front true for front side, false for back side.
   * @return true if the texture is projected.
   * @throws std::logic_error if the face is null.
   */
  bool is_texture_projected(bool front = true) const;
  bool is_texture_projected_front() const;
  bool is_texture_projected_back() const;

  /**
   * @brief Gets the position of a material on the face.
   * @since SketchUp 2021.1, API v9.1
   * @param front true for front side, false for back side.
   * @return MaterialPositionInput describing the texture positioning.
   * @throws std::logic_error if the face is null or has no material.
   */
  MaterialPositionInput material_position(bool front = true) const;
  MaterialPositionInput material_position_front() const;
  MaterialPositionInput material_position_back() const;

  /**
   * @brief Positions a material on the face.
   * @since SketchUp 2021.1, API v9.1
   * @param material_input the MaterialPositionInput describing how to position the texture.
   * @param front true for front side, false for back side.
   * @throws std::logic_error if the face is null or the material has no texture.
   * @throws std::invalid_argument if the material_input parameters are invalid.
   */
  void material_position(const MaterialPositionInput& material_input, bool front = true);
  void material_position_front(const MaterialPositionInput& material_input);
  void material_position_back(const MaterialPositionInput& material_input);

  #endif
  /**
   * @brief Reverses the face's orientation, meaning the front becomes the back.
   * @return reference to this Face after reversal.
   * @throws std::logic_error if the face is null.
   */
  Face& reverse();

  /**
   * @brief Retrieves all vertices that bound the face.
   * @return vector of Vertex objects.
   * @throws std::logic_error if the face is null.
   */
  std::vector<Vertex> vertices() const;

};

} /* namespace CW */
#endif /* Face_hpp */
