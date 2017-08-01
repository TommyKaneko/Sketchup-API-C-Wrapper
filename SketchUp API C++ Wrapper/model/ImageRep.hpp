//
//  ImageRep.hpp
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
//

#ifndef ImageRep_hpp
#define ImageRep_hpp

#include <stdio.h>
#include <stdio.h>

#include <SketchUpAPI/model/image_rep.h>

#include "Entity.hpp"

namespace CW {

// Forward declarations

/*
* ImageRep wrapper. References an image representation object.
* @since SU 2017, API 5.0
*/
class ImageRep  {
	private:
  SUImageRepRef m_image_rep;
  bool m_attached;
  
  static SUImageRepRef copy_reference(const ImageRep& other);
  public:
  /** Constructor for null ImageRep value */
  ImageRep();
	
  /** Construct ImageRep from existing ImageRep object */
  ImageRep(SUImageRepRef image_rep, bool attached = true);
  
	/** Copy constructor */
  ImageRep(const ImageRep& other);
  
  /** Destructor */
  ~ImageRep();

  /** Copy assignment operator */
  ImageRep& operator=(const ImageRep& other);
  
  /**
  * Returns whether this is a valid object.
  */
  bool operator!() const;
  
  /*
  * The class object can be converted to a SUImageRepRef without loss of data.
  */
  SUImageRepRef ref() const;
  operator SUImageRepRef() const;
  operator SUImageRepRef*();
  
  /**
  * Returns a duplicate ImgaeRep object
  */
  ImageRep copy() const;
  
  /**
  * Sets the image data for the given image. Makes a copy of the data rather than taking ownership.
  */
  void set_data(size_t width, size_t height, size_t bits_per_pixel, size_t row_padding, std::vector<SUByte> pixel_data);
	
  /**
  * Loads an image from a file path.
  * @param file_path - string path to the file.
  */
  void load_file(const std::string file_path);

	/**
  * Saves the image to a file location.
  */
  SU_RESULT save_to_file(const std::string file_path) const;
	
  /**
  * Returns the width of the image in pixels.
  */
  size_t width() const;

  /**
  * Returns the width of the image in pixels.
  */
  size_t height() const;
	
  /**
  * Returns the row padding in pixels.
  */
  size_t row_padding() const;
	
  /**
  * Resizes the width and height of the ImageRep
  */
  void resize(size_t width, size_t height);
  
	/**
  * Converts the image to 32 bits per pixel.
  */
  void convert_to_32bits();

	/**
  * Returns the total size and bits-per-pixel value of an image. This function is useful to determine the size of the buffer necessary to be passed into SUImageRepGetData. The returned data can be used along with the returned bits-per-pixel value and the image dimensions to compute RGBA values at individual pixels of the image.
  */
  size_t data_size() const;
  
  /**
  * Returns the number of bits per pixel in the image.
  */
  size_t bits_per_pixel() const;
  
  /**
  * Returns the pixel data for an image.
  */
  std::vector<SUByte> pixel_data() const;
};

} // END namespace CW


#endif /* ImageRep_hpp */
