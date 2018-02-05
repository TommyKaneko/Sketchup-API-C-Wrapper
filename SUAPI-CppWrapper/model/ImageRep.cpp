//
//  ImageRep.cpp
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


#include "SUAPI-CppWrapper/model/ImageRep.hpp"

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
    SUResult res = SUImageRepRelease(&m_image_rep);
    assert(res == SU_ERROR_NONE);
  }
}


ImageRep& ImageRep::operator=(const ImageRep& other) {
  if (!m_attached && SUIsValid(m_image_rep)) {
    SUResult res = SUImageRepRelease(&m_image_rep);
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
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::copy(): ImageRep is null");
  }
  SUImageRepRef copy_image = SU_INVALID;
  SUResult res = SUImageRepCreate(&copy_image);
  assert(res == SU_ERROR_NONE);
  res = SUImageRepCopy(copy_image, m_image_rep);
  assert(res == SU_ERROR_NONE);
  return ImageRep(copy_image, false);
}


void ImageRep::set_data(size_t width, size_t height, size_t bits_per_pixel, size_t row_padding, const std::vector<SUByte> pixel_data) {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::set_data(): ImageRep is null");
  }
  SUResult res = SUImageRepSetData(m_image_rep, width, height, bits_per_pixel, row_padding, &pixel_data[0]);
  if (res == SU_ERROR_OUT_OF_RANGE) {
    if (width == 0 || height == 0) {
      throw std::invalid_argument("CW::ImageRep::set_data(): given width or height is 0 - it must be greater than 0");
    }
    else {
      throw std::invalid_argument("CW::ImageRep::set_data(): given bits_per_pixel must be 8, 24 or 32");
    }
  }
  assert(res == SU_ERROR_NONE);
}


void ImageRep::load_file(const std::string file_path) {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::load_file(): ImageRep is null");
  }
  SUResult res = SUImageRepLoadFile(m_image_rep, file_path.c_str());
  if (res == SU_ERROR_SERIALIZATION) {
    throw std::invalid_argument("CW::ImageRep::load_file(): Loading file failed. Probably invalid file path given.");
  }
  assert(res == SU_ERROR_NONE);
}


SUResult ImageRep::save_to_file(const std::string file_path) const {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::save_to_file(): ImageRep is null");
  }
  SUResult res = SUImageRepSaveToFile(m_image_rep, file_path.c_str());
  if (res == SU_ERROR_SERIALIZATION) {
    throw std::invalid_argument("CW::ImageRep::save_to_file(): Saving file failed. Probably invalid file path given.");
  }
  else if (res == SU_ERROR_NO_DATA) {
    throw std::logic_error("CW::ImageRep::save_to_file(): Image contains no data to save.");
  }
  assert(res == SU_ERROR_NONE);
  return res;
}


size_t ImageRep::width() const {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::width(): ImageRep is null");
  }
  size_t width;
  size_t height;
  SUResult res = SUImageRepGetPixelDimensions(m_image_rep, &width, &height);
  assert(res == SU_ERROR_NONE);
  return width;
}


size_t ImageRep::height() const {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::height(): ImageRep is null");
  }
  size_t width;
  size_t height;
  SUResult res = SUImageRepGetPixelDimensions(m_image_rep, &width, &height);
  assert(res == SU_ERROR_NONE);
  return height;
}
  

size_t ImageRep::row_padding() const {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::row_padding(): ImageRep is null");
  }
  size_t padding;
  SUResult res =  SUImageRepGetRowPadding(m_image_rep, &padding);
  assert(res == SU_ERROR_NONE);
  return padding;
}


void ImageRep::resize(size_t width, size_t height) {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::resize(): ImageRep is null");
  }
  SUResult res = SUImageRepResize(m_image_rep, width, height);
  if (res == SU_ERROR_OUT_OF_RANGE) {
    throw std::invalid_argument("CW::ImageRep::resize(): width and height must be greater than 0.");
  }
  assert(res == SU_ERROR_NONE);
}


void ImageRep::convert_to_32bits() {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::convert_to_32bits(): ImageRep is null");
  }
  SUResult res = SUImageRepConvertTo32BitsPerPixel(m_image_rep);
  if (res == SU_ERROR_NO_DATA) {
    throw std::logic_error("CW::ImageRep::convert_to_32bits(): Image contains no data.");
  }
  assert(res == SU_ERROR_NONE);
}


size_t ImageRep::data_size() const {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::data_size(): ImageRep is null");
  }
  size_t data_size;
  size_t bits_per_pixel;
  SUResult res = SUImageRepGetDataSize(m_image_rep, &data_size, &bits_per_pixel);
  assert(res == SU_ERROR_NONE);
  return data_size;
}
 
  
size_t ImageRep::bits_per_pixel() const {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::bits_per_pixel(): ImageRep is null");
  }
  size_t data_size;
  size_t bits_per_pixel;
  SUResult res = SUImageRepGetDataSize(m_image_rep, &data_size, &bits_per_pixel);
  assert(res == SU_ERROR_NONE);
  return bits_per_pixel;
}


std::vector<SUByte> ImageRep::pixel_data() const {
  if(!(*this)) {
    throw std::logic_error("CW::ImageRep::pixel_data(): ImageRep is null");
  }
  std::vector<SUByte> pixel_data;
  size_t data_size = this->data_size();
  pixel_data.reserve(this->data_size());
  SUResult res = SUImageRepGetData(m_image_rep, data_size, &pixel_data[0]);
  assert(res == SU_ERROR_NONE);
  return pixel_data;
}
  
} // END namespace CW
