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

#include "SUAPI-CppWrapper/model/Texture.hpp"
#include <cassert>
#include <stdexcept>
#include <stdio.h>

#include "SUAPI-CppWrapper/model/ImageRep.hpp"
#include "SUAPI-CppWrapper/String.hpp"

namespace CW {
/******************************
** Private Static Methods *****
*******************************/

SUTextureRef Texture::create_texture(ImageRep& image_rep) {
  SUImageRepRef image = image_rep.ref();
  SUTextureRef texture = SU_INVALID;
  SUResult res = SUTextureCreateFromImageRep(&texture, image);
  assert(res == SU_ERROR_NONE);
  return texture;
}

SUTextureRef Texture::create_texture(const std::string file_path, double s_scale, double t_scale) {
  SUTextureRef texture = SU_INVALID;
  SUResult res = SUTextureCreateFromFile(&texture, file_path.c_str(), s_scale,t_scale);
  if (res != SU_ERROR_NONE) {
    return SU_INVALID;
  }
  return texture;
}


SUTextureRef Texture::copy_reference(const Texture& other) {
  return other.m_texture;
}

/******************************
** Constructors / Destructor **
*******************************/
Texture::Texture():
  Entity(),
  m_texture(SU_INVALID)
{}


Texture::Texture(SUTextureRef texture, bool attached):
  Entity(SUTextureToEntity(texture), attached),
  m_texture(texture)
{}


Texture::Texture(const Texture& other):
  Entity(other, SUTextureToEntity(copy_reference(other))),
  m_texture(SUTextureFromEntity(m_entity))
{}


Texture::Texture(const std::string file_path, double s_scale, double t_scale):
  Texture(create_texture(file_path, s_scale, t_scale), false)
{}


Texture::Texture(ImageRep& image):
  Texture(create_texture(image), false)
{}


Texture::~Texture() {

}


Texture& Texture::operator=(const Texture& other) {
  // Simply assign the other vertex to this object.
  m_texture = other.m_texture;
  m_entity = SUTextureToEntity(m_texture);
  Entity::operator=(other);
  return (*this);
}


Texture::operator SUTextureRef() const {
  return ref();
}


Texture::operator SUTextureRef*() {
  return &m_texture;
}


SUTextureRef Texture::ref() const {
  return m_texture;
}

Texture Texture::copy() const {
  // First create the file.
  std::string file_path = "/tmp/tmptexture";
  SUResult res = this->save(file_path);
  assert(res == SU_ERROR_NONE);
  Texture new_texture(file_path, s_scale(), t_scale());
  // Delete the temporary file
  const char *file_path_c = file_path.c_str();
  int ret = std::remove(file_path_c);
  assert(ret == 0);
  // Reset the file name
  new_texture.file_name(this->file_name());
  return new_texture;
  // @developers_notes Method below is efficient and simple, but fails to transfer the scale of the texture, due to the lack of SUTextureSetDimensions() function in the C API.  A workaround method is used for now shown above.
  /*
  ImageRep image_rep = this->image_rep().copy();
  Texture new_texture(image_rep);
  new_texture.file_name(this->file_name());
  return new_texture;
  */
}


bool Texture::alpha_used() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::alpha_used(): Texture is null");
  }
  bool alpha_channel_used;
  SUResult res = SUTextureGetUseAlphaChannel  (m_texture,  &alpha_channel_used);
  assert(res == SU_ERROR_NONE);
  return alpha_channel_used;
}


ImageRep Texture::image_rep() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::alpha_used(): Texture is null");
  }
  SUImageRepRef image_rep = SU_INVALID;
  SUResult res = SUImageRepCreate(&image_rep);
  assert(res == SU_ERROR_NONE);
  res = SUTextureGetImageRep(m_texture, &image_rep);
  assert(res == SU_ERROR_NONE);
  return ImageRep(image_rep);
}


String Texture::file_name() const {
  String name;
  SUStringRef file_ref = name.ref();
  SUResult res = SUTextureGetFileName(m_texture, &file_ref);
  return name;
}


void Texture::file_name(const String& string) const {
  const char* chars = string.std_string().c_str();
  SUResult res = SUTextureSetFileName(m_texture, chars);
  assert(res == SU_ERROR_NONE);
}


size_t Texture::width() const {
  if (!(*this)) {
    throw std::logic_error("CW::Texture::width(): Texture is null");
  }
  size_t width;
  size_t height;
  double s_scale;
  double t_scale;
  SUResult res = SUTextureGetDimensions(m_texture, &width, &height, &s_scale, &t_scale);
  assert(res == SU_ERROR_NONE);
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
  SUResult res = SUTextureGetDimensions(m_texture, &width, &height, &s_scale, &t_scale);
  assert(res == SU_ERROR_NONE);
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
  SUResult res = SUTextureGetDimensions(m_texture, &width, &height, &s_scale, &t_scale);
  assert(res == SU_ERROR_NONE);
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
  SUResult res = SUTextureGetDimensions(m_texture, &width, &height, &s_scale, &t_scale);
  assert(res == SU_ERROR_NONE);
  return t_scale;
}


SUResult Texture::save(const std::string& file_path) const {
  const char *cstr = file_path.c_str();
  SUResult res = SUTextureWriteToFile(m_texture, cstr);
  return res;
}

} /* namespace CW */
