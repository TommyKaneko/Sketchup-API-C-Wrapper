//
//  UVHelper.hpp
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

#ifndef UVHelper_hpp
#define UVHelper_hpp

#include <stdio.h>

#include <SketchUpAPI/model/uv_helper.h>

namespace CW {

/*
* UVHelper wrapper
*/
class UVHelper {
	private:
  SUUVHelperRef m_uv_helper;
  bool m_attached;
  
  public:
  UVHelper();
  UVHelper(SUUVHelperRef uv_helper_ref, bool release_on_destroy = false);
  
  operator SUUVHelperRef() const;
  operator SUUVHelperRef*() const;
  
};

} /* namespace CW */

#endif /* UVHelper_hpp */
