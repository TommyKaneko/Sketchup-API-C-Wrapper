//
//  Classifications.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 23/11/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Classifications_hpp
#define Classifications_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include <SketchUpAPI/model/classifications.h>

namespace CW {

/*
* Entity object wrapper
*/
class Classifications {
	private:
  SUClassificationsRef m_classifications;
  
  public:
  //Classifications();
  Classifications(SUClassificationsRef classifications);
	
};

} /* namespace CW */

#endif /* Classifications_hpp */
