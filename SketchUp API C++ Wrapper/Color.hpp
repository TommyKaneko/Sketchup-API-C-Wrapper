//  Color.hpp
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

#ifndef Color_hpp
#define Color_hpp

#include <stdio.h>

#include <SketchUpAPI/color.h>

namespace CW {

/*
* Color wrapper
*/
class Color {
	private:
  SUColor m_color;
  
  public:
  Color();
  Color(SUColor color_input);
  
  /*
  * Return the SUColor object.
  */
  SUColor ref() const;
  
  operator SUColor() const;
	
  /**
  * Compare equality of the Color object
  */
  friend bool operator ==(const Color &lhs, const Color &rhs);
  
};

} /* namespace CW */

#endif /* Color_hpp */
