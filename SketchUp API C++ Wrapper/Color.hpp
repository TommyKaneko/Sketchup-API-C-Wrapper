//
//  Color.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 09/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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
  Color(SUColor color_input);
  
  /*
  * Return the SUColor object.
  */
  SUColor ref();
  
};

} /* namespace CW */

#endif /* Color_hpp */
