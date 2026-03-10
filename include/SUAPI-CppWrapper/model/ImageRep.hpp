//
//  ImageRep.hpp
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

#ifndef ImageRep_hpp
#define ImageRep_hpp

#include <SketchUpAPI/model/image_rep.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations

/**
 * @brief Wrapper for SUImageRepRef.
 *
 * Represents an image representation object for working with pixel data,
 * loading and saving image files, and performing image operations.
 *
 * @since SketchUp 2017, API 5.0
 * @see SUImageRepRef in the SketchUp C API
 */
class ImageRep  {

  friend class RubyAPI;
  friend class Texture;

  private:
  SUImageRepRef m_image_rep;
  bool m_attached;

  /**
   * @brief Copies the underlying SUImageRepRef.
   * @param other The ImageRep to copy.
   * @return The copied SUImageRepRef.
   */
  static SUImageRepRef copy_reference(const ImageRep& other);

  public:
  /**
   * @brief Constructs a null ImageRep.
   */
  ImageRep();

  /**
   * @brief Constructs an ImageRep from an existing SUImageRepRef.
   * @param image_rep The native image representation reference.
   * @param attached  Whether the image rep is already owned by another object.
   */
  ImageRep(SUImageRepRef image_rep, bool attached = true);

  /**
   * @brief Copy constructor.
   * @param other The ImageRep to copy.
   */
  ImageRep(const ImageRep& other);

  /**
   * @brief Destructor. Releases the image rep if not attached.
   */
  ~ImageRep();

  /**
   * @brief Copy assignment operator.
   * @param other The ImageRep to assign from.
   * @return Reference to this object.
   */
  ImageRep& operator=(const ImageRep& other);

  /**
   * @brief Returns true if the ImageRep is invalid.
   * @return True if invalid.
   */
  bool operator!() const;

  /**
   * @brief Returns the wrapped SUImageRepRef.
   * @return The raw SUImageRepRef.
   */
  SUImageRepRef ref() const;

  /** @brief Implicit conversion to SUImageRepRef. */
  operator SUImageRepRef() const;

  /** @brief Implicit conversion to SUImageRepRef*. */
  operator SUImageRepRef*();

  /**
   * @brief Returns a duplicate ImageRep.
   * @return A new unattached ImageRep copy.
   * @see SUImageRepCopy
   */
  ImageRep copy() const;

  /**
   * @brief Sets the image pixel data. Makes a copy of the data.
   * @param width          The width in pixels.
   * @param height         The height in pixels.
   * @param bits_per_pixel The number of bits per pixel.
   * @param row_padding    The row padding in bytes.
   * @param pixel_data     The pixel data.
   * @see SUImageRepSetData
   */
  void set_data(size_t width, size_t height, size_t bits_per_pixel, size_t row_padding, std::vector<SUByte> pixel_data);

  /**
   * @brief Loads an image from a file path.
   * @param file_path The path to the image file.
   * @see SUImageRepLoadFile
   */
  void load_file(const std::string file_path);

  /**
   * @brief Saves the image to a file.
   * @param file_path The destination file path.
   * @return SU_ERROR_NONE on success, or an error code.
   * @see SUImageRepSaveToFile
   */
  SUResult save_to_file(const std::string file_path) const;

  /**
   * @brief Returns the width of the image in pixels.
   * @return The pixel width.
   * @see SUImageRepGetPixelDimensions
   */
  size_t width() const;

  /**
   * @brief Returns the height of the image in pixels.
   * @return The pixel height.
   * @see SUImageRepGetPixelDimensions
   */
  size_t height() const;

  /**
   * @brief Returns the row padding in bytes.
   * @return The row padding.
   * @see SUImageRepGetRowPadding
   */
  size_t row_padding() const;

  /**
   * @brief Resizes the image to the given dimensions.
   * @param width  The new width in pixels.
   * @param height The new height in pixels.
   * @see SUImageRepResize
   */
  void resize(size_t width, size_t height);

  /**
   * @brief Converts the image to 32 bits per pixel.
   * @see SUImageRepConvertTo32BitsPerPixel
   */
  void convert_to_32bits();

  /**
   * @brief Returns the total data size in bytes.
   *
   * Useful for determining the buffer size needed for pixel data retrieval.
   *
   * @return The data size in bytes.
   * @see SUImageRepGetDataSize
   */
  size_t data_size() const;

  /**
   * @brief Returns the number of bits per pixel.
   * @return The bits per pixel value.
   * @see SUImageRepGetDataSize
   */
  size_t bits_per_pixel() const;

  /**
   * @brief Returns the pixel data for the image.
   * @return A vector of pixel data bytes.
   * @see SUImageRepGetData
   */
  std::vector<SUByte> pixel_data() const;
};

} // END namespace CW


#endif /* ImageRep_hpp */
