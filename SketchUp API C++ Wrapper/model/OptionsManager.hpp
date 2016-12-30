//
//  OptionsManager.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 22/12/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef OptionsManager_hpp
#define OptionsManager_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include <SketchUpAPI/model/options_manager.h>

namespace CW {

// Forward Declarations:
class Model;


class OptionsManager {
	private:
  SUOptionsManagerRef m_options_manager;
  
  public:
  OptionsManager(SUOptionsManagerRef options_manager);
};

class OptionsProvider {
};

} /* namespace CW */

#endif /* OptionsManager_hpp */
