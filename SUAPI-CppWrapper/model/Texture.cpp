//
//  Texture.cpp
//
// Sketchup C++ Wrapper for C API
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

#include "SUAPI-CppWrapper/model/Texture.hpp"
#include <cassert>
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
