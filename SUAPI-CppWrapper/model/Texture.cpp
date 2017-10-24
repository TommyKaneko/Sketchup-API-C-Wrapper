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

#include "SUAPI-CppWrapper/model/ImageRep.hpp"

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

} /* namespace CW */
