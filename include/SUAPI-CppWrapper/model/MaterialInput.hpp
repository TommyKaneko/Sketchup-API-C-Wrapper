//
//  MaterialInput.hpp
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

#ifndef MaterialInput_hpp
#define MaterialInput_hpp

#include <SketchUpAPI/model/geometry_input.h>

namespace CW {

// Forward declarations
class Material;

/**
 * @brief Wrapper for SUMaterialInput, representing material data
 *        to be applied to a face during geometry creation.
 *
 * @deprecated Since SketchUp 2021.2 (API 9.1), use MaterialPositionInput instead.
 * @see MaterialPositionInput
 */
class MaterialInput {
  private:
  SUMaterialInput m_material_input;

  /**
  * Creates a SUMaterialInput struct, with default values derived from the Material object.
  */
  static SUMaterialInput create_material_input(Material material);

  public:
  //MaterialInput();

  /**
  * Constructs a MaterialInput from a Material object.
  * @param material the Material to use as input.
  */
  MaterialInput(Material material);

  /**
  * Constructs a MaterialInput from a raw SUMaterialInput struct.
  * @param material_input the raw C API material input struct.
  */
  MaterialInput(SUMaterialInput material_input);

  /**
  * Returns the underlying SUMaterialInput struct.
  * @return the raw C API material input struct.
  */
  SUMaterialInput ref();

  /**
  * Returns the Material associated with this input.
  * @return the Material object.
  */
  Material material() const;
};


#if SketchUpAPI_VERSION_MAJOR >= 2021

/**
 * @brief Wrapper for SUMaterialPositionInput, representing material data
 *        with positioning to be applied to a face during geometry creation.
 *
 * @since SketchUp 2021.2 (API 9.1)
 * @see MaterialInput (deprecated predecessor)
 */
class MaterialPositionInput {
  private:
  SUMaterialPositionInput m_material_input;

  /**
  * Creates a SUMaterialInput struct, with default values derived from the Material object.
  */
  static SUMaterialPositionInput create_material_input(const Material& material);

  public:
  //MaterialInput();

  /**
  * Constructs a MaterialPositionInput from a Material object.
  * @param material the Material to use as input.
  */
  MaterialPositionInput(const Material& material);

  /**
  * Constructs a MaterialPositionInput from a raw SUMaterialPositionInput struct.
  * @param material_input the raw C API material position input struct.
  */
  MaterialPositionInput(SUMaterialPositionInput material_input);

  /**
  * Returns the underlying SUMaterialPositionInput struct.
  * @return the raw C API material position input struct.
  */
  SUMaterialPositionInput ref() const;

  /**
  * Returns the Material associated with this input.
  * @return the Material object.
  */
  Material material() const;

  /**
  * Sets the Material for this input.
  * @param mat the Material to assign.
  */
  void material(const Material& mat);

};

#endif

} /* namespace CW */

#endif /* MaterialInput_hpp */
