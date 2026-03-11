//
//  Image.hpp
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

#ifndef Image_hpp
#define Image_hpp

#include <string>

#include <SketchUpAPI/model/image.h>

#include "SUAPI-CppWrapper/model/DrawingElement.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"

namespace CW {

// Forward declarations
class ImageRep;
class ComponentDefinition;
class String;

/**
 * @brief C++ wrapper for SUImageRef.
 *
 * An Image object represents a raster image placed in the SketchUp model.
 * Images are drawing elements and can be transformed, assigned to layers, etc.
 *
 * @see SUImageRef
 * @since SketchUp 2017 M0, API 5.0
 */
class Image :public DrawingElement {
  private:
  static SUImageRef create_image(const std::string& file_path);
  static SUImageRef create_image(const ImageRep& image_rep);
  static SUImageRef copy_reference(const Image& other);

  public:
  /**
  * @brief Constructs a NULL Image object.
  */
  Image();

  /**
  * @brief Constructs an Image from an image file path.
  * @since SketchUp 2017 M0, API 5.0
  * @param file_path - path to the image file on disk.
  * @throws std::invalid_argument if the file path is invalid.
  */
  Image(const std::string& file_path);

  /**
  * @brief Constructs an Image from an ImageRep object.
  * @since SketchUp 2017 M0, API 5.0
  * @param image_rep - the ImageRep to create the image from.
  * @throws std::invalid_argument if the image_rep is invalid.
  */
  Image(const ImageRep& image_rep);

  /**
  * @brief Constructs an Image from an existing SUImageRef.
  * @since SketchUp 2017 M0, API 5.0
  * @param image - SUImageRef object.
  * @param attached - flag indicating whether the image is owned by
  *   a parent object (e.g. SUEntitiesRef).
  */
  Image(SUImageRef image, bool attached = true);

  /** @brief Copy Constructor */
  Image(const Image& other);

  /** @brief Destructor */
  ~Image();

  /** @brief Copy assignment operator */
  Image& operator=(const Image& other);

  /**
  * @brief Returns the SUImageRef object.
  */
  SUImageRef ref() const;
  operator SUImageRef() const;
  operator SUImageRef*();

  /**
  * @brief Returns true if this is an invalid/null image.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the image representation object of this image.
  * @since SketchUp 2017 M0, API 5.0
  * @throws std::logic_error if the image is null.
  */
  ImageRep image_rep() const;

  /**
  * @brief Retrieves the transform of the image (position and scale in model
  *   space).
  * @since SketchUp 2017 M0, API 5.0
  * @throws std::logic_error if the image is null.
  */
  Transformation transform() const;

  /**
  * @brief Sets the transform of the image (position and scale in model space).
  * @since SketchUp 2017 M0, API 5.0
  * @param transform - the new transformation to apply.
  * @throws std::logic_error if the image is null.
  */
  void transform(const Transformation& transform);

  /**
  * @brief Retrieves the file name of the image.
  * @since SketchUp 2017 M0, API 5.0
  * @throws std::logic_error if the image is null.
  */
  String file_name() const;

  /**
  * @brief Retrieves the world-space width and height of the image.
  * @since SketchUp 2017 M0, API 5.0
  * @return a pair containing (width, height) in inches.
  * @throws std::logic_error if the image is null.
  */
  std::pair<double, double> dimensions() const;

  /**
  * @brief Retrieves the component definition of the image. An image is
  *   backed by an internal component definition. This may be of interest
  *   for advanced workflows.
  * @since SketchUp 2021.1, API 9.1
  * @throws std::logic_error if the image is null.
  */
  ComponentDefinition definition() const;

  /**
  * @brief Creates a copy of this image object, duplicating all properties.
  * @return a new unattached Image object with the same properties.
  * @throws std::logic_error if this image is null.
  */
  Image copy() const;
};

} /* namespace CW */

#endif /* Image_hpp */
