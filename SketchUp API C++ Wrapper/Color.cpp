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

} /* namespace CW */