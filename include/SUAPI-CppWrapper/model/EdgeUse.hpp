//
//  EdgeUse.hpp
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

#ifndef EdgeUse_hpp
#define EdgeUse_hpp

#include <vector>
#include <SketchUpAPI/model/edge_use.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class Edge;
class Face;
class Loop;
class Vertex;

/**
 * @brief C++ wrapper for SUEdgeUseRef.
 *
 * An EdgeUse represents the use of an edge in a specific loop/face context.
 * It provides topology information for polygon boundaries. EdgeUse objects
 * are obtained from loops and faces and are always read-only (no
 * Create/Release).
 *
 * The start/end vertices of an EdgeUse may differ from the underlying edge's
 * vertices because the loop may traverse the edge in reverse.
 *
 * @see SUEdgeUseRef
 */
class EdgeUse :public Entity {
  private:
  static SUEdgeUseRef copy_reference(const EdgeUse& other);

  public:
  /**
  * @brief Constructs a NULL EdgeUse object.
  */
  EdgeUse();

  /**
  * @brief Constructs an EdgeUse from an existing SUEdgeUseRef.
  * @param edge_use - the SUEdgeUseRef to wrap.
  * @param attached - always true since EdgeUse objects are owned by the model.
  */
  EdgeUse(SUEdgeUseRef edge_use, bool attached = true);

  /**
  * @brief Copy constructor.
  */
  EdgeUse(const EdgeUse& other);

  /**
  * @brief Copy assignment operator.
  */
  EdgeUse& operator=(const EdgeUse& other);

  /**
  * @brief Returns the SUEdgeUseRef object.
  */
  SUEdgeUseRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null edge use.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the edge associated with this edge use.
  * @throws std::logic_error if the edge use is null.
  */
  Edge edge() const;

  /**
  * @brief Retrieves the loop that this edge use belongs to.
  * @throws std::logic_error if the edge use is null.
  */
  Loop loop() const;

  /**
  * @brief Retrieves the face that this edge use belongs to.
  * @throws std::logic_error if the edge use is null.
  */
  Face face() const;

  /**
  * @brief Retrieves the partner edge uses (edge uses of the same edge
  *   from adjacent faces).
  * @throws std::logic_error if the edge use is null.
  */
  std::vector<EdgeUse> partners() const;

  /**
  * @brief Retrieves whether this edge use is reversed relative to the
  *   underlying edge's direction.
  * @throws std::logic_error if the edge use is null.
  */
  bool is_reversed() const;

  /**
  * @brief Retrieves the previous edge use in the loop.
  * @throws std::logic_error if the edge use is null.
  */
  EdgeUse previous() const;

  /**
  * @brief Retrieves the next edge use in the loop.
  * @throws std::logic_error if the edge use is null.
  */
  EdgeUse next() const;

  /**
  * @brief Retrieves the start vertex of this edge use.
  * @note May differ from the underlying edge's start vertex if reversed.
  * @throws std::logic_error if the edge use is null.
  */
  Vertex start_vertex() const;

  /**
  * @brief Retrieves the end vertex of this edge use.
  * @note May differ from the underlying edge's end vertex if reversed.
  * @throws std::logic_error if the edge use is null.
  */
  Vertex end_vertex() const;

  /**
  * @brief Retrieves the normal vector at the start vertex.
  * @throws std::logic_error if the edge use is null.
  */
  Vector3D start_vertex_normal() const;

  /**
  * @brief Retrieves the normal vector at the end vertex.
  * @throws std::logic_error if the edge use is null.
  */
  Vector3D end_vertex_normal() const;
};

} /* namespace CW */

#endif /* EdgeUse_hpp */
