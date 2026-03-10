//
//  DrawingElement.hpp
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

#ifndef DrawingElement_hpp
#define DrawingElement_hpp

#include <SketchUpAPI/model/drawing_element.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"
namespace CW {

// Forward declarations:
class Layer;
class Material;
class BoundingBox3D;

/**
 * @brief DrawingElement wrapper.
 *
 * References a drawing element, which is an abstract base type for some API
 * types. This is a subclass of Entity.
 */
class DrawingElement :public Entity {
  friend class InstancePath;
  protected:
  /**
   * @brief Creates a new DrawingElement object.
   * @param drawing_element SUDrawingElementRef object to assign.
   * @param attached Flag indicating if the entity should be treated as
   *                 attached to another object. @see CW::Entity
   */
  DrawingElement(SUDrawingElementRef drawing_element, bool attached = true);

  /**
   * @brief Copy constructor with an optional parameter for the element
   *        reference.
   *
   * SUDrawingElementRef objects cannot be created from this class, so the Ref
   * object must be passed to this constructor from a derived class object.
   * @param other DrawingElement object from which properties will be copied.
   * @param element_ref SUDrawingElementRef object to assign to the copied
   *                    object.
   */
  DrawingElement(const DrawingElement& other, SUDrawingElementRef element_ref = SU_INVALID);

  /**
   * @brief Constructor representing a null object.
   */
  DrawingElement();

  /**
   * @brief Copy assignment operator.
   */
  DrawingElement& operator=(const DrawingElement& other);

  /**
   * @brief Copies all DrawingElement properties from the given element to
   *        this one.
   * @param element DrawingElement object from which to get properties.
   * @return true on success, false on failure.
   */
  bool copy_properties_from(const DrawingElement& element);

  public:
  /**
   * @brief Returns the wrapped SUDrawingElementRef.
   */
  SUDrawingElementRef ref() const;

  /**
   * @brief Conversion operator to SUDrawingElementRef.
   */
  operator SUDrawingElementRef() const;

  /**
   * @brief Conversion operator to SUDrawingElementRef pointer.
   */
  operator SUDrawingElementRef*();

  /**
   * @brief Retrieves the bounding box of the drawing element.
   * @return BoundingBox3D of this element.
   */
  BoundingBox3D bounds();

  /**
   * @brief Retrieves the casts shadows flag of the drawing element.
   * @return true if the drawing element casts shadows.
   */
  bool casts_shadows() const;

  /**
   * @brief Sets the casts shadows flag of the drawing element.
   * @param casts_shadows The casts shadows flag to set.
   */
  void casts_shadows(bool casts_shadows);

  /**
   * @brief Retrieves the hidden flag of the drawing element.
   * @return true if the drawing element is hidden.
   */
  bool hidden() const;

  /**
   * @brief Sets the hidden flag of the drawing element.
   * @param hidden true to hide the element, false to show it.
   */
  void hidden(bool hidden);

  /**
   * @brief Retrieves the Layer object associated with the drawing element.
   * @return Layer object. Returns a null Layer if no layer is assigned.
   */
  Layer layer() const;

  /**
   * @brief Sets the Layer for the drawing element.
   * @param layer The Layer object to assign. The layer must be attached to
   *              a model.
   * @throws std::logic_error if the layer is valid but not attached to a
   *         model.
   */
  void layer(const Layer& layer);

  /**
   * @brief Retrieves the Material object of the drawing element.
   * @return Material object. Returns a null Material if no front face
   *         material is assigned.
   */
  Material material() const;

  /**
   * @brief Sets the Material of the drawing element.
   * @param material The Material object to assign. If an invalid material
   *                 is given, the material of the element will be removed.
   * @throws std::logic_error if the material is valid but not attached to
   *         a model.
   * @throws std::invalid_argument if the material is owned by a layer or
   *         image.
   */
  void material(const Material& material);

  /**
   * @brief Retrieves the receives shadows flag of the drawing element.
   * @return true if the drawing element receives shadows.
   */
  bool receives_shadows() const;

  /**
   * @brief Sets the receives shadows flag of the drawing element.
   * @param receives_shadows true to receive shadows, false otherwise.
   */
  void receives_shadows(bool receives_shadows);
};

} /* namespace CW */
#endif /* DrawingElement_hpp */
