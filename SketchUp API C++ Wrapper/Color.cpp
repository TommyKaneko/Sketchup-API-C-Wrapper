//
//  Color.cpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 09/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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