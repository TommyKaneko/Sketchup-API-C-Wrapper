//
//  Texture.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
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

#include <stdio.h>

#include <SketchUpAPI/model/texture.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class ImageRep;
class String;

/*
* Texture wrapper
*/
class Texture :public Entity {
  private:
  SUTextureRef m_texture;
  
  /**
  * Create texture from ImageRep
  */
  static SUTextureRef create_texture(ImageRep& image_rep);
  
  /**
  * Create texture from file path
  */
  static SUTextureRef create_texture(const std::string file_path, double s_scale = 1.0, double t_scale = 1.0);
  
  static SUTextureRef copy_reference(const Texture& other);
  public:
  /** Constructor for null Texture value */
  Texture();
  
  /** Construct Texture from existing texture object */
  Texture(SUTextureRef texture, bool attached = true);
  
  /**
  * Creates a new texture object from an image file specified by a path.
  * @param file_path - The file path of the source image file. Assumed to be UTF-8 encoded
  * @param s_scale - The scale factor for s coordinate value.
  * @param t_scale - The scale factor for t coordinate value.
  */
  Texture(const std::string file_path, double s_scale = 1.0, double t_scale = 1.0);
  
  /**
  * Creates a new texture object with the specified image data.
  * This function is due to be depreciated - use Texture(ImageRep& image) instead from SU 2017, API 5.0 onwards.
  * @param width - the width in pixels of the texture data
  * @param height - the height in pixels of the texture data
  * @param bits_per_pixel - the number of bits per pixel of the image data
  * @param pixel_data[] - the source of the pixel data
  */
  Texture (size_t width, size_t height, size_t bits_per_pixel, const SUByte pixel_data[]);
  
  /**
  * Creates a new texture object from an image representation object.
  * @since Sketchup 2017, API 5.0
  */
  Texture(ImageRep& image);
    
  /** Copy constructor */
  Texture(const Texture& other);
  
  /** Destructor */
  ~Texture();

  /** Copy assignment operator */
  Texture& operator=(const Texture& other);
  
  /*
  * The class object can be converted to a SUTextureRef without loss of data.
  */
  SUTextureRef ref() const;
  operator SUTextureRef() const;
  operator SUTextureRef*();
  
  /**
  * Returns a copy of the texture object - note that a Texture object can only be assigned to one Material object.  So this method is useful for copying Textures to new Material objects.
  * @since SU 2017, API 5.0
  */
  Texture copy() const;
  
  /**
  * Returns whether the alpha channel is used by the texture image.
  */
  bool alpha_used() const;
  
  /**
  * Returns the ImageRep object of the texture.
  * @since SU 2017, API 5.0
  */
  ImageRep image_rep() const;
  
  /**
  * Returns the file name of the texture
  */
  String file_name() const;

  /**
  * Sets theimage file name associated with the texture object. If the input texture was constructed using SUTextureCreateFromFile the name will already be set, so calling this function will override the texture's file name string.
  */
  void file_name(const String& string) const;
  
  /**
  * Returns the width of the texture in pixels.
  */
  size_t width() const;
  
  /**
  * Returns the height of the texture in pixels.
  */
  size_t height() const;
  
  /**
  * Returns the s_scale of the texture. The s_scale and t_scale values are useful when a face doesn't have a material applied directly, but instead inherit from a parent group or component instance. Then you want use these values to multiply the result of SUMeshHelperGetFrontSTQCoords or SUUVHelperGetFrontUVQ. If the material is applied directly then this would not be needed.
  */
  double s_scale() const;

  /**
  * Returns the t_scale of the texture. The s_scale and t_scale values are useful when a face doesn't have a material applied directly, but instead inherit from a parent group or component instance. Then you want use these values to multiply the result of SUMeshHelperGetFrontSTQCoords or SUUVHelperGetFrontUVQ. If the material is applied directly then this would not be needed.
  */
  double t_scale() const;
  
  /**
  * Writes a texture object as an image to disk.
  * @return SUResult object with the following possible values:
  *      - SU_ERROR_NONE on success
  *      - SU_ERROR_INVALID_INPUT if texture is not a valid object
  *      - SU_ERROR_NULL_POINTER_INPUT if file_path is NULL
  *      - SU_ERROR_SERIALIZATION if image file could not be written to disk
  *
  */
  SUResult save(const std::string& file_path) const;

};

} // END namespace CW
#endif /* Texture_hpp */
