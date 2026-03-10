//
//  Layer.hpp
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

#ifndef Layer_hpp
#define Layer_hpp

#include "SUAPI-CppWrapper/model/Entity.hpp"

#include <SketchUpAPI/model/layer.h>

namespace CW {
// Forward declarations
class String;

/**
 * @brief C++ wrapper for SULayerRef.
 *
 * A layer (also known as a Tag in newer SketchUp versions) controls
 * visibility of drawing elements.
 * @see SULayerRef
 */
class Layer :public Entity {
  private:
  /** @brief Creates a new empty SULayerRef via the C API. */
  static SULayerRef create_layer();

  /**
   * @brief Creates a SULayerRef derived from an existing Layer object.
   * @param other  Layer object to derive the new ref from.
   * @return If the Layer is already attached to a model, its existing ref
   *         is returned. Otherwise a new SULayerRef is created (properties
   *         will not be copied in that case).
   */
  static SULayerRef copy_reference(const Layer& other);

  public:
  /** @brief Constructs a null Layer. */
  Layer();

  /**
   * @brief Wraps an existing SULayerRef.
   * @param layer_ref  Existing SULayerRef to wrap.
   * @param attached   true if the ref is already owned by a model.
   */
  Layer(SULayerRef layer_ref, bool attached = true);

  /** @brief Copy constructor. */
  Layer(const Layer& other);

  /** @brief Destructor — releases the layer if not attached. */
  ~Layer();

  /** @brief Copy assignment operator. */
  Layer& operator=(const Layer& other);

  /** @brief Returns the underlying SULayerRef. */
  SULayerRef ref() const;

  /** @brief Implicit conversion to SULayerRef. */
  operator SULayerRef() const;

  /** @brief Returns a pointer to the underlying SULayerRef. */
  operator SULayerRef*();

  /**
   * @brief Checks whether the Layer is invalid (null).
   * @return true if the underlying ref is invalid.
   */
  bool operator!() const;

  /**
   * @brief Returns a detached copy of the Layer.
   * @return A new Layer object not attached to any model.
   * @throws std::logic_error if the Layer is null.
   */
  Layer copy() const;

  /**
   * @brief Retrieves the name of the layer.
   * @return The layer name as a String.
   * @throws std::logic_error if the Layer is null.
   */
  String name() const;

  /**
   * @brief Sets the name of the layer.
   * @param string  The new name for the layer.
   * @throws std::logic_error if the Layer is null.
   */
  void name(const String& string);

  /**
   * @brief Sets the name of the layer.
   * @param string  The new name for the layer.
   * @throws std::logic_error if the Layer is null.
   */
  void name(const std::string& string);

  /** @brief Hash function for use with unordered_map. */
  friend std::hash<CW::Layer>;
};

} /* namespace CW */

namespace std {
  template <> struct hash<CW::Layer>
  {
    size_t operator()(const CW::Layer& k) const
    {
      static const size_t shift = static_cast<size_t>(log2(1 + sizeof(CW::Layer)));
      return reinterpret_cast<size_t>(k.m_entity.ptr) >> shift;
    }
  };

}

#endif /* Layer_hpp */
