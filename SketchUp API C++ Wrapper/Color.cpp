//  Color.cpp
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

#include "Color.hpp"

namespace CW {


Color::Color():
  m_color(SUColor{})
{}

Color::Color(SUColor color_input):
  m_color(color_input)
{}

SUColor Color::ref() const {
  return m_color;
}

Color::operator SUColor() const {
  return ref();
}

bool operator ==(const Color &lhs, const Color &rhs) {
	if (lhs.m_color.alpha == rhs.m_color.alpha &&
      lhs.m_color.red == rhs.m_color.red &&
			lhs.m_color.blue == rhs.m_color.blue &&
			lhs.m_color.green == rhs.m_color.green)	{
  	return true;
  }
  return false;
}


} /* namespace CW */