//
//  Image.cpp
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

#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Image.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/ImageRep.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUImageRef Image::create_image(const std::string& file_path) {
  SUImageRef image = SU_INVALID;
  SUResult res = SUImageCreateFromFile(&image, file_path.c_str());
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_NULL_POINTER_INPUT:
    case SU_ERROR_SERIALIZATION:
      throw std::invalid_argument("CW::Image::create_image(): Invalid file path or unable to read file: " + file_path);
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
  return image;
}


SUImageRef Image::create_image(const ImageRep& image_rep) {
  SUImageRef image = SU_INVALID;
  SUResult res = SUImageCreateFromImageRep(&image, image_rep.ref());
  switch (res) {
    case SU_ERROR_NONE:
      break;
    case SU_ERROR_INVALID_INPUT:
      throw std::invalid_argument("CW::Image::create_image(): Invalid ImageRep");
    default:
      assert(res == SU_ERROR_NONE); _unused(res);
  }
  return image;
}


SUImageRef Image::copy_reference(const Image& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  // Create a new image from the original's image rep and copy the transform
  SUImageRepRef image_rep = SU_INVALID;
  SUResult res = SUImageGetImageRep(other.ref(), &image_rep);
  assert(res == SU_ERROR_NONE); _unused(res);

  SUImageRef new_image = SU_INVALID;
  res = SUImageCreateFromImageRep(&new_image, image_rep);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUImageRepRelease(&image_rep);
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy transform
  SUTransformation transform;
  res = SUImageGetTransform(other.ref(), &transform);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUImageSetTransform(new_image, &transform);
  assert(res == SU_ERROR_NONE); _unused(res);

  return new_image;
}


/******************************
** Constructors / Destructor **
*******************************/
Image::Image():
  DrawingElement(SU_INVALID, false)
{}


Image::Image(const std::string& file_path):
  DrawingElement(SUImageToDrawingElement(create_image(file_path)), false)
{}


Image::Image(const ImageRep& image_rep):
  DrawingElement(SUImageToDrawingElement(create_image(image_rep)), false)
{}


Image::Image(SUImageRef image, bool attached):
  DrawingElement(SUImageToDrawingElement(image), attached)
{}


Image::Image(const Image& other):
  DrawingElement(other, SUImageToDrawingElement(copy_reference(other)))
{
}


Image::~Image() {
  // Note: The SketchUp C API does not provide SUImageRelease().
  // Standalone images are cleaned up when added to entities.
}


/*******************
** Public Methods **
********************/
Image& Image::operator=(const Image& other) {
  // Note: The SketchUp C API does not provide SUImageRelease().
  m_entity = SUImageToEntity(copy_reference(other));
  DrawingElement::operator=(other);
  return (*this);
}


SUImageRef Image::ref() const {
  return SUImageFromEntity(m_entity);
}

Image::operator SUImageRef() const {
  return this->ref();
}

Image::operator SUImageRef*() {
  return reinterpret_cast<SUImageRef*>(&m_entity);
}


bool Image::operator!() const {
  return SUIsInvalid(m_entity);
}


ImageRep Image::image_rep() const {
  if (!(*this)) {
    throw std::logic_error("CW::Image::image_rep(): Image is null");
  }
  SUImageRepRef rep = SU_INVALID;
  SUResult res = SUImageGetImageRep(this->ref(), &rep);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ImageRep(rep, false);
}


Transformation Image::transform() const {
  if (!(*this)) {
    throw std::logic_error("CW::Image::transform(): Image is null");
  }
  SUTransformation su_transform;
  SUResult res = SUImageGetTransform(this->ref(), &su_transform);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Transformation(su_transform);
}


void Image::transform(const Transformation& transform) {
  if (!(*this)) {
    throw std::logic_error("CW::Image::transform(): Image is null");
  }
  SUTransformation su_transform = transform.ref();
  SUResult res = SUImageSetTransform(this->ref(), &su_transform);
  assert(res == SU_ERROR_NONE); _unused(res);
}


String Image::file_name() const {
  if (!(*this)) {
    throw std::logic_error("CW::Image::file_name(): Image is null");
  }
  String string;
  SUResult res = SUImageGetFileName(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


std::pair<double, double> Image::dimensions() const {
  if (!(*this)) {
    throw std::logic_error("CW::Image::dimensions(): Image is null");
  }
  double width, height;
  SUResult res = SUImageGetDimensions(this->ref(), &width, &height);
  assert(res == SU_ERROR_NONE); _unused(res);
  return std::make_pair(width, height);
}


ComponentDefinition Image::definition() const {
  if (!(*this)) {
    throw std::logic_error("CW::Image::definition(): Image is null");
  }
  SUComponentDefinitionRef def = SU_INVALID;
  SUResult res = SUImageGetDefinition(this->ref(), &def);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ComponentDefinition(def);
}


Image Image::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::Image::copy(): Image is null");
  }
  // Create a new image from the original's image rep
  SUImageRepRef rep = SU_INVALID;
  SUResult res = SUImageGetImageRep(this->ref(), &rep);
  assert(res == SU_ERROR_NONE); _unused(res);

  SUImageRef new_ref = SU_INVALID;
  res = SUImageCreateFromImageRep(&new_ref, rep);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUImageRepRelease(&rep);
  assert(res == SU_ERROR_NONE); _unused(res);

  // Copy transform
  SUTransformation transform;
  res = SUImageGetTransform(this->ref(), &transform);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUImageSetTransform(new_ref, &transform);
  assert(res == SU_ERROR_NONE); _unused(res);

  Image new_image(new_ref, false);
  new_image.copy_attributes_from(*this);
  return new_image;
}


} /* namespace CW */
