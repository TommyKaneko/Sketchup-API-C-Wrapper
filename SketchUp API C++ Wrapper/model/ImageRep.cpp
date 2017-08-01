//
//  ImageRep.cpp
//  SketchUp API C++ Wrapper
//
//  Created by Hidetomo Kaneko on 01/08/2017.
//  Copyright © 2017 TommyKaneko. All rights reserved.
//

#include "ImageRep.hpp"

#include <cassert>

namespace CW {

/*************************
* Private Static Functions
**************************/
SUImageRepRef ImageRep::copy_reference(const ImageRep& other) {
  if (other.m_attached || SUIsInvalid(other.m_image_rep)) {
  	return other.m_image_rep;
  }
  ImageRep new_copy = other.copy();
  new_copy.m_attached = true; // Ensures that the object will not be destroyed at the end this scope.
  return new_copy.ref();
}

/***************************
* Constructors / Destructors
****************************/
  
ImageRep::ImageRep():
	m_image_rep(SU_INVALID),
  m_attached(false)
{}
	
ImageRep::ImageRep(SUImageRepRef image_rep, bool attached):
	m_image_rep(image_rep),
  m_attached(attached)
{}
  

ImageRep::ImageRep(const ImageRep& other):
	ImageRep(copy_reference(other), other.m_attached)
{}
  

ImageRep::~ImageRep() {
	if (SUIsValid(m_image_rep) && !m_attached) {
  	SU_RESULT res = SUImageRepRelease(&m_image_rep);
    assert(res == SU_ERROR_NONE);
  }
}


ImageRep& ImageRep::operator=(const ImageRep& other) {
  if (!m_attached && SUIsValid(m_image_rep)) {
    SU_RESULT res = SUImageRepRelease(&m_image_rep);
    assert(res == SU_ERROR_NONE);
  }
  m_image_rep = copy_reference(other);
	m_attached = other.m_attached;
  return (*this);
}
  

bool ImageRep::operator!() const {
	return SUIsInvalid(m_image_rep);
}


SUImageRepRef ImageRep::ref() const {
	return m_image_rep;
}


ImageRep::operator SUImageRepRef() const {
	return ref();
}


ImageRep::operator SUImageRepRef*() {
	return &m_image_rep;
}


ImageRep ImageRep::copy() const {
	SUImageRepRef copy_image = SU_INVALID;
	SU_RESULT res = SUImageRepCopy(m_image_rep, copy_image);
  assert(res == SU_ERROR_NONE);
  return ImageRep(copy_image, false);
}


void ImageRep::set_data(size_t width, size_t height, size_t bits_per_pixel, size_t row_padding, const std::vector<SUByte> pixel_data) {
  SU_RESULT res = SUImageRepSetData(m_image_rep, width, height, bits_per_pixel, row_padding, &pixel_data[0]);
	assert(res == SU_ERROR_NONE);
}


void ImageRep::load_file(const std::string file_path) {
	SU_RESULT res = SUImageRepLoadFile(m_image_rep, file_path.c_str());
  assert(res == SU_ERROR_NONE);
}


SU_RESULT ImageRep::save_to_file(const std::string file_path) const {
  SU_RESULT res = SUImageRepSaveToFile(m_image_rep, file_path.c_str());
  return res;
}


size_t ImageRep::width() const {
	size_t width;
  size_t height;
	SU_RESULT res = SUImageRepGetPixelDimensions(m_image_rep, &width, &height);
  assert(res == SU_ERROR_NONE);
  return width;
}


size_t ImageRep::height() const {
	size_t width;
  size_t height;
	SU_RESULT res = SUImageRepGetPixelDimensions(m_image_rep, &width, &height);
  assert(res == SU_ERROR_NONE);
  return height;
}
  

size_t ImageRep::row_padding() const {
	size_t padding;
  SU_RESULT res =  SUImageRepGetRowPadding(m_image_rep, &padding);
  assert(res == SU_ERROR_NONE);
  return padding;
}


void ImageRep::resize(size_t width, size_t height) {
  SU_RESULT res = SUImageRepResize(m_image_rep, width, height);
  assert(res == SU_ERROR_NONE);
}


void ImageRep::convert_to_32bits() {
	SU_RESULT res = SUImageRepConvertTo32BitsPerPixel(m_image_rep);
  assert(res == SU_ERROR_NONE);
}


size_t ImageRep::data_size() const {
	size_t data_size;
	size_t bits_per_pixel;
  SU_RESULT res = SUImageRepGetDataSize(m_image_rep, &data_size, &bits_per_pixel);
  assert(res == SU_ERROR_NONE);
  return data_size;
}
 
  
size_t ImageRep::bits_per_pixel() const {
	size_t data_size;
	size_t bits_per_pixel;
  SU_RESULT res = SUImageRepGetDataSize(m_image_rep, &data_size, &bits_per_pixel);
  assert(res == SU_ERROR_NONE);
  return bits_per_pixel;
}


std::vector<SUByte> ImageRep::pixel_data() const {
	std::vector<SUByte> pixel_data;
  size_t data_size = this->data_size();
  pixel_data.reserve(this->data_size());
  SU_RESULT res = SUImageRepGetData(m_image_rep, data_size, &pixel_data[0]);
  assert(res == SU_ERROR_NONE);
  return pixel_data;
}
  
} // END namespace CW
