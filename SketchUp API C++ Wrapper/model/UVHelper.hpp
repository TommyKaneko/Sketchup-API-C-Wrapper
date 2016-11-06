//
//  UVHelper.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
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
  bool m_release_on_destroy;
  
  public:
  UVHelper();
  UVHelper(SUUVHelperRef uv_helper_ref, bool release_on_destroy = false);
  
  operator SUUVHelperRef() const;
  operator SUUVHelperRef*() const;
  
};

} /* namespace CW */

#endif /* UVHelper_hpp */
