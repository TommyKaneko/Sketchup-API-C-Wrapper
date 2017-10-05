//
//  Texture.hpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
  * @return SU_RESULT object with the following possible values:
  *			- SU_ERROR_NONE on success
	*			- SU_ERROR_INVALID_INPUT if texture is not a valid object
	*			- SU_ERROR_NULL_POINTER_INPUT if file_path is NULL
	*			- SU_ERROR_SERIALIZATION if image file could not be written to disk
	*
  */
  SU_RESULT save(const std::string& file_path);

};

} // END namespace CW
#endif /* Texture_hpp */
