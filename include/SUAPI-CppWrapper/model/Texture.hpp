//
//  Texture.hpp
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

#ifndef Texture_hpp
#define Texture_hpp

#include <SketchUpAPI/model/texture.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class ImageRep;
class String;

/**
 * @brief Wrapper for SUTextureRef.
 *
 * Represents a texture image that can be applied to a material. Textures
 * can be created from file paths, pixel data, or ImageRep objects.
 *
 * @see SUTextureRef in the SketchUp C API
 */
class Texture :public Entity {

  friend class Material;

  private:
  /**
   * @brief Creates a texture from an ImageRep.
   * @param image_rep The source ImageRep.
   * @return The created SUTextureRef.
   */
  static SUTextureRef create_texture(ImageRep& image_rep);

  /**
   * @brief Creates a texture from a file path.
   * @param file_path The file path of the source image (UTF-8 encoded).
   * @param s_scale   The scale factor for the s coordinate.
   * @param t_scale   The scale factor for the t coordinate.
   * @return The created SUTextureRef.
   */
  static SUTextureRef create_texture(const std::string file_path, double s_scale = 1.0, double t_scale = 1.0);

  /**
   * @brief Copies the underlying SUTextureRef.
   * @param other The Texture to copy.
   * @return The copied SUTextureRef.
   */
  static SUTextureRef copy_reference(const Texture& other);

  public:
  /**
   * @brief Constructs a null Texture.
   */
  Texture();

  /**
   * @brief Constructs a Texture from an existing SUTextureRef.
   * @param texture  The native texture reference.
   * @param attached Whether the texture is already owned by a material.
   */
  Texture(SUTextureRef texture, bool attached = true);

  /**
   * @brief Creates a new texture from an image file.
   * @param file_path The file path of the source image (UTF-8 encoded).
   * @param s_scale   The scale factor for the s coordinate.
   * @param t_scale   The scale factor for the t coordinate.
   */
  Texture(const std::string file_path, double s_scale = 1.0, double t_scale = 1.0);

  /**
   * @brief Creates a new texture from raw pixel data.
   *
   * @deprecated Use Texture(ImageRep&) instead from SU 2017, API 5.0 onwards.
   *
   * @param width          The width in pixels.
   * @param height         The height in pixels.
   * @param bits_per_pixel The number of bits per pixel.
   * @param pixel_data     The source pixel data.
   */
  Texture (size_t width, size_t height, size_t bits_per_pixel, const SUByte pixel_data[]);

  /**
   * @brief Creates a new texture from an ImageRep.
   * @param image The source ImageRep.
   * @since SketchUp 2017, API 5.0
   */
  Texture(ImageRep& image);

  /**
   * @brief Copy constructor.
   * @param other The Texture to copy.
   */
  Texture(const Texture& other);

  /**
   * @brief Destructor. Releases the texture if not attached.
   */
  ~Texture();

  /**
   * @brief Copy assignment operator.
   * @param other The Texture to assign from.
   * @return Reference to this object.
   */
  Texture& operator=(const Texture& other);

  /**
   * @brief Returns the wrapped SUTextureRef.
   * @return The raw SUTextureRef.
   */
  SUTextureRef ref() const;

  /** @brief Implicit conversion to SUTextureRef. */
  operator SUTextureRef() const;

  /** @brief Implicit conversion to SUTextureRef*. */
  operator SUTextureRef*();

  /**
   * @brief Returns a copy of this texture, not attached to any material.
   *
   * A Texture can only be assigned to one Material, so this method is
   * useful for copying textures to new materials.
   *
   * @return A new unattached Texture copy.
   * @since SketchUp 2017, API 5.0
   */
  Texture copy() const;

  /**
   * @brief Returns whether the alpha channel is used by the texture image.
   * @return True if the alpha channel is used.
   * @see SUTextureGetUseAlphaChannel
   */
  bool alpha_used() const;

  /**
   * @brief Retrieves the ImageRep of this texture.
   * @return The ImageRep object.
   * @since SketchUp 2017, API 5.0
   * @see SUTextureGetImageRep
   */
  ImageRep image_rep() const;

  /**
   * @brief Retrieves the file name of the texture.
   * @return The file name as a String.
   * @see SUTextureGetFileName
   */
  String file_name() const;

  /**
   * @brief Sets the image file name associated with the texture.
   *
   * If the texture was created from a file, this overrides the stored name.
   *
   * @param string The file name to set.
   * @see SUTextureSetFileName
   */
  void file_name(const String& string) const;

  /**
   * @brief Returns the width of the texture in pixels.
   * @return The pixel width.
   * @see SUTextureGetDimensions
   */
  size_t width() const;

  /**
   * @brief Returns the height of the texture in pixels.
   * @return The pixel height.
   * @see SUTextureGetDimensions
   */
  size_t height() const;

  /**
   * @brief Returns the s-axis scale factor of the texture.
   *
   * The s_scale and t_scale values are useful when a face inherits a
   * material from a parent group or component instance, to multiply
   * the result of SUMeshHelperGetFrontSTQCoords or SUUVHelperGetFrontUVQ.
   *
   * @return The s-axis scale factor.
   * @see SUTextureGetDimensions
   */
  double s_scale() const;

  /**
   * @brief Returns the t-axis scale factor of the texture.
   * @return The t-axis scale factor.
   * @see SUTextureGetDimensions
   */
  double t_scale() const;

  #if (SketchUpAPI_VERSION_MAJOR >= 2025)
  /**
   * @brief Sets the dimensions (scale) of the texture.
   * @param s_scale The s-axis scale factor.
   * @param t_scale The t-axis scale factor.
   * @since SketchUp 2025, API 13.0
   * @see SUTextureSetDimensions
   */
  void set_dimensions(double s_scale, double t_scale);

  /**
   * @brief Sets the s-axis scale factor.
   * @param s_scale The s-axis scale factor.
   * @since SketchUp 2025, API 13.0
   */
  void s_scale(double s_scale);

  /**
   * @brief Sets the t-axis scale factor.
   * @param t_scale The t-axis scale factor.
   * @since SketchUp 2025, API 13.0
   */
  void t_scale(double t_scale);

  #endif
  /**
   * @brief Writes the texture as an image to disk, with colorization applied.
   * @param file_path The destination file path.
   * @return SU_ERROR_NONE on success, or an error code.
   * @see SUTextureWriteToFile
   */
  SUResult save(const std::string& file_path) const;

  /**
   * @brief Writes the texture to disk without any colorization.
   * @param file_path The destination file path.
   * @return SU_ERROR_NONE on success, or an error code.
   * @since SketchUp 2019.2, API 7.1
   * @see SUTextureWriteOriginalToFile
   */
  SUResult save_original(const std::string& file_path) const;

};



} // END namespace CW
#endif /* Texture_hpp */
