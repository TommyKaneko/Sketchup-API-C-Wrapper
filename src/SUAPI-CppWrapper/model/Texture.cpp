//
//  Texture.cpp
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Texture.hpp"
#include <cassert>
#include <stdexcept>
#include <stdio.h>

#include "SUAPI-CppWrapper/Initialize.hpp"
#include "SUAPI-CppWrapper/model/ImageRep.hpp"
#include "SUAPI-CppWrapper/String.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace CW {
/******************************
** Private Static Methods *****
*******************************/

SUTextureRef Texture::create_texture(ImageRep& image_rep) {
  if(!image_rep) {
    throw std::logic_error("CW::Texture::create_texture(): ImageRep is null");
  }
  SUImageRepRef image = SU_INVALID;
  if (image_rep.m_attached) {
    ImageRep image_rep_copy = image_rep.copy();
    image = image_rep_copy.ref();
    image_rep_copy.m_attached = true;
  } else {
    image = image_rep.ref();
    image_rep.m_attached = true;
  }
  SUTextureRef texture = SU_INVALID;
  SUResult res = SUTextureCreateFromImageRep(&texture, image);
  assert(res == SU_ERROR_NONE); _unused(res);
  return texture;
}

SUTextureRef Texture::create_texture(const std::string file_path, double s_scale, double t_scale) {
  SUTextureRef texture = SU_INVALID;
  SUResult res = SUTextureCreateFromFile(&texture, file_path.c_str(), s_scale, t_scale);
  if (res != SU_ERROR_NONE) {
    return SU_INVALID;
  }
  // TODO: At time of writing, SUTextureCreateFromFile is broken - it does not set the s_scale and t_scale values.  In SU2025 and over, we can use SUTextureSetDimensions to set these values after creation.
  #if (SketchUpAPI_VERSION_MAJOR >= 2025)
  res = SUTextureSetDimensions(texture, s_scale, t_scale);
  if (res != SU_ERROR_NONE) {
    return SU_INVALID;
  }
  #endif
  return texture;
}


SUTextureRef Texture::copy_reference(const Texture& other) {
  if (!other) {
    return SU_INVALID;
  }
  if (other.m_attached) {
    return other.ref();
  }
  return create_texture(other.file_name().std_string(), other.s_scale(), other.t_scale());
}

/******************************
** Constructors / Destructor **
*******************************/
Texture::Texture():
  Entity()
{}


Texture::Texture(SUTextureRef texture, bool attached):
  Entity(SUTextureToEntity(texture), attached)
{}


Texture::Texture(const Texture& other):
  Entity(other, SUTextureToEntity(copy_reference(other)))
{
  if (!other.m_attached && SUIsValid(other.m_entity)) {
    this->file_name(other.file_name());
  }
}


Texture::Texture(const std::string file_path, double s_scale, double t_scale):
  Texture(create_texture(file_path, s_scale, t_scale), false)
{
  #if defined(DEBUG)
  double s_scale_read;
  double t_scale_read;
  size_t width;
  size_t height;
  SUResult res = SUTextureGetDimensions(this->ref(), &width, &height, &s_scale_read, &t_scale_read);
  std::cout << "Texture::Texture(const std::string file_path, double s_scale, double t_scale): s_scale: " << s_scale_read << " t_scale: " << t_scale_read << "\n";
  #endif
}


Texture::Texture(ImageRep& image):
  Texture(create_texture(image), false)
{}


Texture::~Texture() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUTextureRef texture = this->ref();
    SUResult res = SUTextureRelease(&texture);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


Texture& Texture::operator=(const Texture& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUTextureRef texture = this->ref();
    SUResult res = SUTextureRelease(&texture);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUTextureToEntity(copy_reference(other));

  if (!other.m_attached && SUIsValid(other.m_entity)) {
    this->file_name(other.file_name());
  }
  Entity::operator=(other);
  return (*this);
}


SUTextureRef Texture::ref() const {
  return SUTextureFromEntity(m_entity);
}


Texture::operator SUTextureRef() const {
  return this->ref();
}


Texture Texture::copy() const {
  if(!(*this)) {
    // Texture is null
    return Texture();
  }
  // @developers_notes Due to the lack of SUTextureSetDimensions() function in the C API until SU 2025, API 13.0, we have to use a workaround to copy the texture including its scale values.
#if (SketchUpAPI_VERSION_MAJOR >= 2025)
  // From SU 2025, API 13.0 onwards we can use the efficient method below.
  ImageRep image_rep = this->image_rep().copy();
  Texture copied_texture(image_rep);
  copied_texture.file_name(this->file_name());
  // Texture::set_dimensions uses inverse of scale values - weird but true.  SO we have to invert the values here.
  double si_scale = 1.0/this->s_scale();
  double ti_scale = 1.0/this->t_scale();
  copied_texture.set_dimensions(si_scale, ti_scale);
  return copied_texture;
#else
  // For earlier versions we have to use the workaround method below.
  // First create the file.
  std::string file_path = "/tmp/tmptexture";
  SUResult res = this->save_original(file_path);
  assert(res == SU_ERROR_NONE); _unused(res);
  double s_scale = this->s_scale();
  double t_scale = this->t_scale();
  #if defined(DEBUG)
    std::cout << "Texture::copy() s_scale: " << s_scale << " t_scale: " << t_scale << "\n";
  #endif
  Texture new_texture(file_path, s_scale, t_scale);
  // Delete the temporary file
  const char *file_path_c = file_path.c_str();
  int ret = std::remove(file_path_c);
  assert(ret == 0); _unused(ret);
  // Reset the file name
  new_texture.file_name(this->file_name());
  return new_texture;
#endif
}


bool Texture::alpha_used() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::alpha_used(): Texture is null");
  }
  bool alpha_channel_used;
  SUResult res = SUTextureGetUseAlphaChannel(this->ref(), &alpha_channel_used);
  assert(res == SU_ERROR_NONE); _unused(res);
  return alpha_channel_used;
}


ImageRep Texture::image_rep() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::image_rep(): Texture is null");
  }
  SUImageRepRef image_rep = SU_INVALID;
  SUResult res = SUImageRepCreate(&image_rep); // Creating an image is necessary for some reason (this is an exception to the rule)
  assert(res == SU_ERROR_NONE);
  res = SUTextureGetImageRep(this->ref(), &image_rep);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ImageRep(image_rep, true); // TODO: check if attached should be true
}


String Texture::file_name() const {
  String name;
  SUStringRef file_ref = name.ref();
  SUResult res = SUTextureGetFileName(this->ref(), &file_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return name;
}


void Texture::file_name(const String& string) const {
  SUResult res = SUTextureSetFileName(this->ref(), string.std_string().c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


size_t Texture::width() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::width(): Texture is null");
  }
  size_t width;
  size_t height;
  double s_scale;
  double t_scale;
  SUResult res = SUTextureGetDimensions(this->ref(), &width, &height, &s_scale, &t_scale);
  assert(res == SU_ERROR_NONE); _unused(res);
  return width;
}


size_t Texture::height() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::height(): Texture is null");
  }
  size_t width;
  size_t height;
  double s_scale;
  double t_scale;
  SUResult res = SUTextureGetDimensions(this->ref(), &width, &height, &s_scale, &t_scale);
  assert(res == SU_ERROR_NONE); _unused(res);
  return height;
}


double Texture::s_scale() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::s_scale(): Texture is null");
  }
  size_t width;
  size_t height;
  double s_scale;
  double t_scale;
  SUResult res = SUTextureGetDimensions(this->ref(), &width, &height, &s_scale, &t_scale);
  assert(res == SU_ERROR_NONE); _unused(res);
  return s_scale;
}


double Texture::t_scale() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::t_scale(): Texture is null");
  }
  size_t width;
  size_t height;
  double s_scale;
  double t_scale;
  SUResult res = SUTextureGetDimensions(this->ref(), &width, &height, &s_scale, &t_scale);
  #if defined(DEBUG)
    std::cout << "Texture::"<< this->file_name() << ": s_scale: " << s_scale << " t_scale: " << t_scale << "\n";
  #endif
  assert(res == SU_ERROR_NONE); _unused(res);
  return t_scale;
}

#if (SketchUpAPI_VERSION_MAJOR >= 2025)
void Texture::set_dimensions(double s_scale, double t_scale) {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::set_dimensions(): Texture is null");
  }
  SUResult res = SUTextureSetDimensions(this->ref(), s_scale, t_scale);
  assert(res == SU_ERROR_NONE); _unused(res);
  return;
}


void Texture::s_scale(double s_scale) {
  this->set_dimensions(s_scale, this->t_scale());
  return;
}


void Texture::t_scale(double t_scale) {
  this->set_dimensions(this->s_scale(), t_scale);
  return;
}
#endif


SUResult Texture::save(const std::string& file_path) const {
  const char *cstr = file_path.c_str();
  SUResult res = SUTextureWriteToFile(this->ref(), cstr);
  return res;
}


SUResult Texture::save_original(const std::string& file_path) const {
  const char *cstr = file_path.c_str();
  SUResult res = SUTextureWriteOriginalToFile(this->ref(), cstr);
  return res;
}

} /* namespace CW */
